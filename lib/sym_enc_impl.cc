/* -*- c++ -*- */
/* 
 * Copyright 2016 Kristian Maier.
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
#include "sym_enc_impl.h"

namespace gr {
    namespace crypto {

        sym_enc::sptr
        sym_enc::make(sym_ciph_desc &ciph_desc)
        {
            return gnuradio::get_initial_sptr
                    (new sym_enc_impl(ciph_desc));
        }

        /*
         * The private constructor
         */
        sym_enc_impl::sym_enc_impl(sym_ciph_desc &ciph_desc)
                : gr::block("sym_enc",
                            gr::io_signature::make(0, 0, 0),
                            gr::io_signature::make(0, 0, 0))
        {
            d_iv_id = pmt::mp("iv");
            d_final_id = pmt::mp("final");
            d_pdu_ctr = 0;

            message_port_register_out(pmt::mp("pdus"));
            message_port_register_in(pmt::mp("pdus"));
            set_msg_handler(pmt::mp("pdus"), boost::bind(&sym_enc_impl::msg_handler, this, _1));

            sym_ciph_desc *desc = &ciph_desc;
            d_ciph = desc->get_evp_ciph();
            d_iv.assign(d_ciph->iv_len, 0);
            d_key = desc->get_key();
            d_padding = desc->get_padding();


            d_ciph_ctx = EVP_CIPHER_CTX_new();
            init_ctx();
        }

        sym_enc_impl::~sym_enc_impl()
        {
            d_key.assign(d_ciph->key_len, 0);
            EVP_CIPHER_CTX_free(d_ciph_ctx);
        }

        void
        sym_enc_impl::init_ctx(){
            //initialize encryption with random iv
            if (1 != RAND_bytes(&d_iv[0], d_ciph->iv_len)) {
                ERR_print_errors_fp(stdout);
            }
            if (1 != EVP_EncryptInit_ex(d_ciph_ctx, d_ciph, NULL, &d_key[0], &d_iv[0])) {
                ERR_print_errors_fp(stdout);
            }
            if (1 != EVP_CIPHER_CTX_set_padding(d_ciph_ctx, d_padding)) {
                ERR_print_errors_fp(stdout);
            }
        }


        void
        sym_enc_impl::msg_handler(pmt::pmt_t msg)
        {
            //check PDU
            if (!pmt::is_pair(msg)) {
                throw std::runtime_error("sym_enc received non PDU input");
            }
            pmt::pmt_t meta = pmt::car(msg);
            pmt::pmt_t data = pmt::cdr(msg);

            if (pmt::is_null(meta)) {
                meta = pmt::make_dict();
            }
            else if (!pmt::is_dict(meta)) {
                throw std::runtime_error("sym_enc received non PDU input");
            }

            //add start iv to first packet
            if (d_pdu_ctr == 0) {
                meta = pmt::dict_add(meta, d_iv_id, pmt::init_u8vector(d_ciph->iv_len, &d_iv[0]));
            }

            d_pdu_ctr++;

            //data to encrypt
            if (pmt::is_u8vector(data) && pmt::length(data) != 0) {

                size_t inlen = pmt::length(data);
                const unsigned char *in = u8vector_elements(data, inlen);
                d_out_buffer.reserve(inlen + 2 * d_ciph->block_size);

                //encrypt and finalize if final flag detected
                int nout = 0;
                int tmp = 0;
                if (1 != EVP_EncryptUpdate(d_ciph_ctx, &d_out_buffer[0], &tmp, in, inlen)) {
                    ERR_print_errors_fp(stdout);
                }
                nout +=tmp;

                if(pmt::dict_has_key(meta, d_final_id)){
                    if (1 != EVP_EncryptFinal_ex(d_ciph_ctx, &d_out_buffer[nout], &tmp)) {
                        ERR_print_errors_fp(stdout);
                    }
                    nout+=tmp;
                    init_ctx();
                    d_pdu_ctr=0;
                }
                data = pmt::init_u8vector(nout, d_out_buffer);

            } else {
                throw std::runtime_error("sym_enc pdu data field not byte vector with data");
            }

            message_port_pub(pmt::mp("pdus"), pmt::cons(meta, data));

        }


    } /* namespace crypto */
} /* namespace gr */

