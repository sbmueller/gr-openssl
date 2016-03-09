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
#include "sym_enc_tagged_bb_impl.h"

namespace gr {
    namespace crypto {

        sym_enc_tagged_bb::sptr
        sym_enc_tagged_bb::make(sym_ciph_desc &ciph_desc, const std::string &packet_len_key)
        {
            return gnuradio::get_initial_sptr
                    (new sym_enc_tagged_bb_impl(ciph_desc, packet_len_key));
        }

        /*
         * The private constructor
         */
        sym_enc_tagged_bb_impl::sym_enc_tagged_bb_impl(sym_ciph_desc &ciph_desc, const std::string &packet_len_key)
                : gr::tagged_stream_block("sym_enc_tagged_bb",
                                          gr::io_signature::make(1, 1, sizeof(unsigned char)),
                                          gr::io_signature::make(1, 1, sizeof(unsigned char)), packet_len_key)
        {
            sym_ciph_desc *desc = &ciph_desc;
            d_ciph = desc->get_evp_ciph();
            d_iv.assign(d_ciph->iv_len, 0);
            d_key = desc->get_key();
            d_padding = desc->get_padding();
            d_ctr=0;

            d_ciph_ctx = EVP_CIPHER_CTX_new();
            init_ctx();

            d_iv_tagkey = pmt::mp("iv");
            d_final_tagkey = pmt::mp("final");
        }


        //new random iv and ctx init
        void
        sym_enc_tagged_bb_impl::init_ctx()
        {
            //random iv
            if (1 != RAND_bytes(&d_iv[0], d_ciph->iv_len)) {
                ERR_print_errors_fp(stdout);
            }
            //initialize encryption
            if (1 != EVP_EncryptInit_ex(d_ciph_ctx, d_ciph, NULL, &d_key[0], &d_iv[0])) {
                ERR_print_errors_fp(stdout);
            }
            if (1 != EVP_CIPHER_CTX_set_padding(d_ciph_ctx, d_padding)) {
                ERR_print_errors_fp(stdout);
            }
        }


        sym_enc_tagged_bb_impl::~sym_enc_tagged_bb_impl()
        {
            d_key.assign(d_ciph->key_len, 0);
            EVP_CIPHER_CTX_free(d_ciph_ctx);
        }

        int
        sym_enc_tagged_bb_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
        {
            return ninput_items[0] + d_ciph->block_size;
        }

        int
        sym_enc_tagged_bb_impl::work(int noutput_items,
                                     gr_vector_int &ninput_items,
                                     gr_vector_const_void_star &input_items,
                                     gr_vector_void_star &output_items)
        {
            const unsigned char *in = (const unsigned char *) input_items[0];
            unsigned char *out = (unsigned char *) output_items[0];

            std::vector<gr::tag_t> t_iv, t_final;
            get_tags_in_window(t_iv, 0, 0, ninput_items[0], d_iv_tagkey);
            get_tags_in_window(t_final, 0, 0, ninput_items[0], d_final_tagkey);


            //encrypt
            int nout = 0;
            if (1 != EVP_EncryptUpdate(d_ciph_ctx, out, &nout, in, ninput_items[0])) {
                ERR_print_errors_fp(stdout);
            }

            //new IV should be generated
            if (t_final.size()) {
                int tmp=0;
                if (1 != EVP_EncryptFinal_ex(d_ciph_ctx, out+nout, &tmp)) {
                    ERR_print_errors_fp(stdout);
                }
                nout=nout+tmp;

                init_ctx();
                d_ctr=0;
            }

            //transmit iv on first sample
            if (d_ctr==0) {
                pmt::pmt_t iv_pmt = pmt::init_u8vector(d_ciph->iv_len, d_iv);
                add_item_tag(0, nitems_read(0), d_iv_tagkey, iv_pmt);
            }

            d_ctr++;
            return nout;
        }

    } /* namespace crypto */
} /* namespace gr */

