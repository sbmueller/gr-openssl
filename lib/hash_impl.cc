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
#include "hash_impl.h"

namespace gr {
    namespace crypto {

        hash::sptr
        hash::make(const std::string &hash_name)
        {
            return gnuradio::get_initial_sptr
                    (new hash_impl(hash_name));
        }


        hash_impl::hash_impl(const std::string &hash_name)
                : gr::block("hash",
                            gr::io_signature::make(0, 0, 0),
                            gr::io_signature::make(0, 0, 0))
        {
            message_port_register_out(pmt::mp("pdus"));
            message_port_register_in(pmt::mp("pdus"));
            set_msg_handler(pmt::mp("pdus"), boost::bind(&hash_impl::msg_handler, this, _1));


            ERR_load_ERR_strings();
            OpenSSL_add_all_digests();
            OPENSSL_config(NULL);

            d_md = EVP_get_digestbyname(hash_name.c_str());
            if (d_md == NULL) {
                throw std::runtime_error("cipher not found\n");
            }
            printf("loaded hash function: %s, block-size: %i\n", hash_name.c_str(), d_md->block_size);

            d_md_ctx = EVP_MD_CTX_create();
            if (1 != EVP_DigestInit_ex(d_md_ctx, d_md, NULL)) {
                ERR_print_errors_fp(stdout);
            }

        }


        hash_impl::~hash_impl()
        {
            EVP_MD_CTX_cleanup(d_md_ctx);
        }

        void
        hash_impl::msg_handler(pmt::pmt_t msg)
        {
            //check PDU
            if (!pmt::is_pair(msg)) {
                throw std::runtime_error("hash block received non PDU input");
            }

            pmt::pmt_t meta = pmt::car(msg);
            pmt::pmt_t data = pmt::cdr(msg);

            if (pmt::is_null(meta)) {
                meta = pmt::make_dict();
            }
            else if (!pmt::is_dict(meta)) {
                throw std::runtime_error("hash block received non PDU input");
            }

            //data to hash
            if (pmt::is_u8vector(data) && pmt::length(data) != 0) {

                size_t inlen = pmt::length(data);
                const unsigned char *in = u8vector_elements(data, inlen);
                d_out_buffer.reserve(d_md->block_size);

                if (1 != EVP_DigestUpdate(d_md_ctx, in, inlen)) {
                    ERR_print_errors_fp(stdout);
                }
                unsigned int nout = 0;
                if (1 != EVP_DigestFinal_ex(d_md_ctx, &d_out_buffer[0], &nout)) {
                    ERR_print_errors_fp(stdout);
                }
                if (1 != EVP_DigestInit_ex(d_md_ctx, d_md, NULL)) {
                    ERR_print_errors_fp(stdout);
                }
                data = pmt::init_u8vector(nout, d_out_buffer);
            }
            else {
                throw std::runtime_error("hash block: pdu data field not byte vector with data");
            }
            message_port_pub(pmt::mp("pdus"), pmt::cons(meta, data));
        }


    } /* namespace crypto */
} /* namespace gr */

