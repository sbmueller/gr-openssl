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
#include "sym_dec_impl.h"

namespace gr {
    namespace crypto {

        sym_dec::sptr
        sym_dec::make(sym_ciph_desc &ciph_desc)
        {
            return gnuradio::get_initial_sptr
                    (new sym_dec_impl(ciph_desc));
        }

        /*
         * The private constructor
         */
        sym_dec_impl::sym_dec_impl(sym_ciph_desc &ciph_desc)
                : gr::block("sym_dec",
                            gr::io_signature::make(0, 0, 0),
                            gr::io_signature::make(0, 0, 0))
        {
            d_iv_id = pmt::mp("iv");

            message_port_register_out(pmt::mp("pdus"));
            message_port_register_in(pmt::mp("pdus"));
            set_msg_handler(pmt::mp("pdus"), boost::bind(&sym_dec_impl::msg_handler, this, _1));

            sym_ciph_desc *desc = &ciph_desc;
            d_ciph = desc->get_evp_ciph();
            d_key.assign(d_ciph->key_len, 0);
            d_iv.assign(d_ciph->iv_len, 0);
            desc->get_key(d_key);
            d_padding = desc->get_padding();

            d_ciph_ctx = EVP_CIPHER_CTX_new();

            //initialize decryption
            d_have_iv = false || (d_ciph->iv_len == 0);
            if (d_have_iv) {
                if (!EVP_DecryptInit_ex(d_ciph_ctx, d_ciph, NULL, &d_key[0], &d_iv[0]))
                    ERR_print_errors_fp(stdout);
                if (!EVP_CIPHER_CTX_set_padding(d_ciph_ctx, d_padding))
                    ERR_print_errors_fp(stdout);
            }

        }

        void
        sym_dec_impl::msg_handler(pmt::pmt_t msg)
        {
            if(!pmt::is_pair(msg)){
                throw std::runtime_error("sym_dec received non PDU input");
            }
            pmt::pmt_t meta = pmt::car(msg);
            pmt::pmt_t data = pmt::cdr(msg);

            if(pmt::is_null(meta)){
                meta = pmt::make_dict();
            }
            else if(!pmt::is_dict(meta)){
                throw std::runtime_error("sym_dec received non PDU input");
            }

            //iv received?
            pmt::pmt_t iv_val = pmt::dict_ref(meta, d_iv_id, pmt::PMT_NIL);
            if (pmt::is_u8vector(iv_val) && pmt::length(iv_val) == d_ciph->iv_len){

                if (d_have_iv) {
                    //decrypt final part
                    int nout = 0;
                    d_out_buffer.reserve(d_ciph->block_size);
                    if (!EVP_DecryptFinal_ex(d_ciph_ctx, &d_out_buffer[0], &nout))
                        ERR_print_errors_fp(stdout);
                    //publish
                    if (nout != 0)
                        message_port_pub(pmt::mp("pdus"), pmt::cons(meta, pmt::init_u8vector(nout, d_out_buffer)));
                }

                size_t iv_len = d_ciph->iv_len;
                const unsigned char *u8tmp = u8vector_elements(iv_val, iv_len);
                d_iv.assign(u8tmp, u8tmp + d_ciph->iv_len);

                d_have_iv = true;

                //remove iv from metadata
                meta = pmt::dict_delete(meta, d_iv_id);

                //initialize encryption with new iv
                if (!EVP_DecryptInit_ex(d_ciph_ctx, d_ciph, NULL, &d_key[0], &d_iv[0]))
                    ERR_print_errors_fp(stdout);
                if (!EVP_CIPHER_CTX_set_padding(d_ciph_ctx, d_padding))
                    ERR_print_errors_fp(stdout);
            }

            if (pmt::is_u8vector(data)) {
                size_t inlen = pmt::length(data);
                const unsigned char *in = u8vector_elements(data, inlen);
                d_out_buffer.reserve(inlen + d_ciph->block_size);

                if (!d_have_iv)
                    throw std::runtime_error("ERROR decryption without iv\n");

                //decrypt
                int nout = 0;
                if (!EVP_DecryptUpdate(d_ciph_ctx, &d_out_buffer[0], &nout, in, inlen))
                    ERR_print_errors_fp(stdout);

                data = pmt::init_u8vector(nout, d_out_buffer);
            }

            //publish
            message_port_pub(pmt::mp("pdus"), pmt::cons(meta, data));


        }


        sym_dec_impl::~sym_dec_impl()
        {
            d_key.assign(d_ciph->key_len, 0);
            EVP_CIPHER_CTX_free(d_ciph_ctx);
        }


    } /* namespace crypto */
} /* namespace gr */

