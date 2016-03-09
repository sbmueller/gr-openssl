/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "auth_enc_aes_gcm_impl.h"

namespace gr {
    namespace crypto {

        auth_enc_aes_gcm::sptr
        auth_enc_aes_gcm::make(std::vector<uint8_t> key, int keylen, int ivlen)
        {
            return gnuradio::get_initial_sptr
                    (new auth_enc_aes_gcm_impl(key, keylen, ivlen));
        }


        auth_enc_aes_gcm_impl::auth_enc_aes_gcm_impl(std::vector<uint8_t> key, int keylen, int ivlen)
                : gr::block("auth_enc_aes_gcm",
                            gr::io_signature::make(0, 0, 0),
                            gr::io_signature::make(0, 0, 0))
        {

            switch (keylen) {
                case 16:
                    d_ciph = EVP_get_cipherbyname("aes-128-gcm");
                    break;
                case 24:
                    d_ciph = EVP_get_cipherbyname("aes-192-gcm");
                    break;
                case 32:
                    d_ciph = EVP_get_cipherbyname("aes-256-gcm");
                    break;
                default:
                    throw std::runtime_error("auth_enc_aes_gcm: wrong key length");

            }

            d_final_id = pmt::mp("final");
            d_iv_id = pmt::mp("iv");
            d_auth_tag_id =  pmt::mp("auth_tag");
            d_aad_id =  pmt::mp("aad");
            d_pdu_ctr = 0;

            message_port_register_out(pmt::mp("pdus"));
            message_port_register_in(pmt::mp("pdus_data"));
            set_msg_handler(pmt::mp("pdus_data"), boost::bind(&auth_enc_aes_gcm_impl::msg_handler_data, this, _1));

            d_iv.assign(d_ciph->iv_len, 0);
            d_ivlen = ivlen;
            d_key = key;

            init_ciph_ctx();
        }


        auth_enc_aes_gcm_impl::~auth_enc_aes_gcm_impl()
        {
            d_key.assign(d_ciph->key_len, 0);
            EVP_CIPHER_CTX_free(d_ciph_ctx);
        }



        void
        auth_enc_aes_gcm_impl::msg_handler_data(pmt::pmt_t msg)
        {
            //check PDU
            if (!pmt::is_pair(msg)) {
                throw std::runtime_error("auth_enc_aes_gcm received non PDU input");
            }
            pmt::pmt_t meta = pmt::car(msg);
            pmt::pmt_t data = pmt::cdr(msg);

            if (pmt::is_null(meta)) {
                meta = pmt::make_dict();
            }
            else if (!pmt::is_dict(meta)) {
                throw std::runtime_error("auth_enc_aes_gcm received non PDU input");
            }

            //data to encrypt
            if (pmt::is_u8vector(data) && pmt::length(data) != 0) {

                size_t inlen = pmt::length(data);
                const unsigned char *in = u8vector_elements(data, inlen);
                d_out_buffer.reserve(inlen + d_ciph->block_size);

                //encrypt
                int nout = 0;
                if (1 != EVP_EncryptUpdate(d_ciph_ctx, &d_out_buffer[0], &nout, in, inlen)) {
                    ERR_print_errors_fp(stdout);
                }

                data = pmt::init_u8vector(nout, d_out_buffer);

                //add iv to first packet
                if (d_pdu_ctr == 0) {
                    meta = pmt::dict_add(meta, d_iv_id, pmt::init_u8vector(d_ivlen, &d_iv[0]));
                }

                d_pdu_ctr++;

            } else {
                throw std::runtime_error("auth_enc_aes_gcm pdu data field not byte vector with data");
            }


            //add aad data for signing
            if (pmt::dict_has_key(meta, d_aad_id)) {
                pmt::pmt_t aad_data = pmt::dict_ref(meta, d_aad_id, pmt::PMT_F);
                size_t inlen = pmt::length(aad_data);
                const unsigned char *in = u8vector_elements(aad_data, inlen);

                int nout=0;
                if(1 != EVP_EncryptUpdate(d_ciph_ctx, NULL, &nout, in, inlen)){
                    ERR_print_errors_fp(stdout);
                }
                //no output here expected
                if(nout){
                    throw std::runtime_error("auth_enc_aes_gcm something unexpected happened, encryption error ?");
                }
            }

            //final encryption requested, write tag in meta
            if (pmt::dict_has_key(meta, d_final_id)) {

                d_out_buffer.reserve(d_ciph->block_size);
                int nout=0;
                if(1 != EVP_EncryptFinal_ex(d_ciph_ctx, &d_out_buffer[0], &nout)){
                    ERR_print_errors_fp(stdout);
                }
                //no further data output in gcm mode, stream cipher
                if(nout){
                    throw std::runtime_error("auth_enc_aes_gcm something unexpected happened, encryption error ?");
                }

                //get the auth tag
                if(1 != EVP_CIPHER_CTX_ctrl(d_ciph_ctx, EVP_CTRL_GCM_GET_TAG, 16, &d_out_buffer[0])){
                    ERR_print_errors_fp(stdout);
                }

                pmt::pmt_t tag = pmt::dict_add(meta, d_auth_tag_id, pmt::init_u8vector(16, d_out_buffer));

                init_ciph_ctx();

                d_pdu_ctr = 0;
            }

            //publish
            message_port_pub(pmt::mp("pdus"), pmt::cons(meta, data));


        }

        void
        auth_enc_aes_gcm_impl::init_ciph_ctx(){
            //generate random iv
            if (1 != RAND_bytes(&d_iv[0], d_ciph->iv_len)){
                ERR_print_errors_fp(stdout);
            }
            //initialize encryption
            if (1 != EVP_EncryptInit_ex(d_ciph_ctx, d_ciph, NULL, NULL, NULL)) {
                ERR_print_errors_fp(stdout);
            }
            if (1 != EVP_CIPHER_CTX_ctrl(d_ciph_ctx, EVP_CTRL_GCM_SET_IVLEN, d_ivlen, NULL)) {
                ERR_print_errors_fp(stdout);
            }
            if (1 != EVP_EncryptInit_ex(d_ciph_ctx, NULL, NULL, &d_key[0], &d_iv[0])) {
                ERR_print_errors_fp(stdout);
            }
        }


    } /* namespace crypto */
} /* namespace gr */

