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
#include "auth_dec_aes_gcm_impl.h"

namespace gr {
  namespace crypto {

    auth_dec_aes_gcm::sptr
    auth_dec_aes_gcm::make(std::vector<uint8_t> key, int keylen, int ivlen)
    {
      return gnuradio::get_initial_sptr
        (new auth_dec_aes_gcm_impl(key, keylen, ivlen));
    }

    auth_dec_aes_gcm_impl::auth_dec_aes_gcm_impl(std::vector<uint8_t> key, int keylen, int ivlen)
      : gr::block("auth_dec_aes_gcm",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(0,0,0))
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
        message_port_register_in(pmt::mp("pdus"));
        set_msg_handler(pmt::mp("pdus"), boost::bind(&auth_dec_aes_gcm_impl::msg_handler, this, _1));

        d_iv.assign(d_ciph->iv_len, 0);
        d_ivlen = ivlen;
        d_key = key;

        d_ciph_ctx = EVP_CIPHER_CTX_new();
    }

    auth_dec_aes_gcm_impl::~auth_dec_aes_gcm_impl()
    {
    }

    void
    auth_dec_aes_gcm_impl::msg_handler(pmt::pmt_t msg)
    {

        //check PDU
        if (!pmt::is_pair(msg)) {
            throw std::runtime_error("auth_dec_aes_gcm received non PDU input");
        }
        pmt::pmt_t meta = pmt::car(msg);
        pmt::pmt_t data = pmt::cdr(msg);

        if (pmt::is_null(meta)) {
            meta = pmt::make_dict();
        }
        else if (!pmt::is_dict(meta)) {
            throw std::runtime_error("auth_dec_aes_gcm received non PDU input");
        }

        //iv received?
        pmt::pmt_t iv_val = pmt::dict_ref(meta, d_iv_id, pmt::PMT_NIL);
        if (pmt::is_u8vector(iv_val) && pmt::length(iv_val) == d_ciph->iv_len) {

            //auth tag not received before new iv -> error
            if(d_have_iv){
                throw std::runtime_error("auth_dec_aes_gcm auhtentication tag not received");
            }

            d_iv = u8vector_elements(iv_val);
            d_have_iv = true;

            //remove iv from metadata
            meta = pmt::dict_delete(meta, d_iv_id);

            //init dec with new iv
            init_ctx();
        }

        //add aad data for signing
        if (pmt::dict_has_key(meta, d_aad_id) && pmt::is_u8vector(pmt::dict_ref(meta, d_aad_id, pmt::PMT_NIL))) {
            pmt::pmt_t aad_data = pmt::dict_ref(meta, d_aad_id, pmt::PMT_F);
            size_t inlen = pmt::length(aad_data);
            const unsigned char *in = u8vector_elements(aad_data, inlen);

            int nout=0;
            if(1 != EVP_DecryptUpdate(d_ciph_ctx, NULL, &nout, in, inlen)){
                ERR_print_errors_fp(stdout);
            }
            //no output here expected
            if(nout){
                throw std::runtime_error("auth_dec_aes_gcm something unexpected happened, decryption error ?");
            }
        }


        if (pmt::is_u8vector(data) && pmt::length(data) != 0) {
            size_t inlen = pmt::length(data);
            const unsigned char *in = u8vector_elements(data, inlen);
            d_out_buffer.reserve(inlen + 2*d_ciph->block_size);

            if (!d_have_iv) {
                throw std::runtime_error("auth_dec_aes_gcm ERROR decryption without iv\n");
            }

            //decrypt
            int nout = 0;
            int tmp = 0;
            if (1 != EVP_DecryptUpdate(d_ciph_ctx, &d_out_buffer[0], &tmp, in, inlen)) {
                ERR_print_errors_fp(stdout);
            }
            nout=tmp;

            if(pmt::dict_has_key(meta, d_auth_tag_id)){
                if (1 < EVP_DecryptFinal_ex(d_ciph_ctx, &d_out_buffer[nout], &tmp)) {
                    //auth error
                    pmt::dict_add(meta, d_auth_tag_id, pmt::PMT_F);
                    ERR_print_errors_fp(stdout);
                }else{
                    pmt::dict_add(meta, d_auth_tag_id, pmt::PMT_T);
                }
                nout+=tmp;
                //need iv in next msg
                d_have_iv = false;
            }

            data = pmt::init_u8vector(nout, d_out_buffer);
        }else {
            throw std::runtime_error("auth_dec_aes_gcm pdu data field not byte vector with data");
        }

        //publish
        message_port_pub(pmt::mp("pdus"), pmt::cons(meta, data));
    }



      void
      auth_dec_aes_gcm_impl::init_ctx(){
          //initialize encryption
          if (1 != EVP_DecryptInit_ex(d_ciph_ctx, d_ciph, NULL, NULL, NULL)) {
              ERR_print_errors_fp(stdout);
          }
          if (1 != EVP_CIPHER_CTX_ctrl(d_ciph_ctx, EVP_CTRL_GCM_SET_IVLEN, d_ivlen, NULL)) {
              ERR_print_errors_fp(stdout);
          }
          if (1 != EVP_DecryptInit_ex(d_ciph_ctx, NULL, NULL, &d_key[0], &d_iv[0])) {
              ERR_print_errors_fp(stdout);
          }
      }



  } /* namespace crypto */
} /* namespace gr */

