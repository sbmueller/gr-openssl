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
            d_iv_key = pmt::mp("iv");
            d_out_port_id = pmt::mp("encrypted");
            d_in_port_id = pmt::mp("plain");

            message_port_register_out(d_out_port_id);
            message_port_register_in(d_in_port_id);
            set_msg_handler(d_in_port_id, boost::bind(&sym_enc_impl::msg_handler, this, _1));

            sym_ciph_desc *desc = &ciph_desc;
            d_ciph = desc->get_evp_ciph();
            d_key.resize(d_ciph->key_len);
            d_iv.resize(d_ciph->iv_len);
            desc->get_key(d_key);
            d_padding = desc->get_padding();

            //random iv
            RAND_bytes(&d_iv[0], d_ciph->iv_len);

            //initialize encryption
            d_ciph_ctx = EVP_CIPHER_CTX_new();
            if (!EVP_EncryptInit_ex(d_ciph_ctx, d_ciph, NULL, &d_key[0], &d_iv[0]))
                ERR_print_errors_fp(stdout);
            if (!EVP_CIPHER_CTX_set_padding(d_ciph_ctx, d_padding))
                ERR_print_errors_fp(stdout);

        }

        sym_enc_impl::~sym_enc_impl()
        {
            d_key.assign(d_ciph->key_len, 0);
            EVP_CIPHER_CTX_free(d_ciph_ctx);
        }

        bool
        sym_enc_impl::start()
        {
            //send first iv instantly
            pmt::pmt_t iv_msg = pmt::cons(d_iv_key, pmt::init_u8vector(d_ciph->iv_len, &d_iv[0]));
            message_port_pub(d_out_port_id, iv_msg);
            return true;
         }

        void
        sym_enc_impl::msg_handler(pmt::pmt_t msg)
        {
            //2 cases:
            //msg==pure pmt u8 vector-->data to encrypt
            //msg==d_new_iv_key ->use new random iv

            //new data to encrypt
            if (pmt::is_u8vector(msg)) {

                size_t inlen = pmt::length(msg);
                const uint8_t *in = u8vector_elements(msg, inlen);
                uint8_t out_buffer[inlen + d_ciph->block_size];

                //encrypt
                int nout = 0;
                if (!EVP_EncryptUpdate(d_ciph_ctx, out_buffer, &nout, in, inlen))
                    ERR_print_errors_fp(stdout);

                //publish
                if(nout != 0)
                    message_port_pub(d_out_port_id, pmt::init_u8vector(nout, out_buffer));

            }
            //new random iv requested
            else if (msg == d_iv_key) {
                RAND_bytes(&d_iv[0], d_ciph->iv_len);

                int nout = 0;
                uint8_t out_buffer[d_ciph->block_size];
                if (!EVP_EncryptFinal_ex(d_ciph_ctx, out_buffer, &nout))
                    ERR_print_errors_fp(stdout);

                //publish
                if(nout!=0)
                    message_port_pub(d_out_port_id, pmt::init_u8vector(nout, out_buffer));

                //initialize encryption with new iv
                if (!EVP_EncryptInit_ex(d_ciph_ctx, d_ciph, NULL, &d_key[0], &d_iv[0]))
                    ERR_print_errors_fp(stdout);
                if (!EVP_CIPHER_CTX_set_padding(d_ciph_ctx, d_padding))
                    ERR_print_errors_fp(stdout);

                //send new iv
                pmt::pmt_t iv_msg = pmt::cons(d_iv_key, pmt::init_u8vector(d_ciph->iv_len, &d_iv[0]));
                message_port_pub(d_out_port_id, iv_msg);
            }
        }


    } /* namespace crypto */
} /* namespace gr */

