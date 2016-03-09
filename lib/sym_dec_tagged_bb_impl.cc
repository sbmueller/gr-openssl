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
#include "sym_dec_tagged_bb_impl.h"

namespace gr {
    namespace crypto {

        sym_dec_tagged_bb::sptr
        sym_dec_tagged_bb::make(sym_ciph_desc &desc, const std::string &packet_len_tag)
        {
            return gnuradio::get_initial_sptr
                    (new sym_dec_tagged_bb_impl(desc, packet_len_tag));
        }

        /*
         * The private constructor
         */
        sym_dec_tagged_bb_impl::sym_dec_tagged_bb_impl(sym_ciph_desc &ciph_desc, const std::string &packet_len_tag)
                : gr::tagged_stream_block("sym_dec_tagged_bb",
                                          gr::io_signature::make(1, 1, sizeof(unsigned char)),
                                          gr::io_signature::make(1, 1, sizeof(unsigned char)), packet_len_tag)
        {
            sym_ciph_desc *desc = &ciph_desc;
            d_ciph = desc->get_evp_ciph();
            d_iv.assign(d_ciph->iv_len, 0);
            d_key = desc->get_key();
            d_padding = desc->get_padding();

            d_ciph_ctx = EVP_CIPHER_CTX_new();

            //initialize decryption if iv is not needed
            d_have_iv = false || (d_ciph->iv_len == 0);
            if (d_have_iv) {
                init_ctx();
            }

            d_iv_tagkey = pmt::mp("iv");
            d_final_tagkey = pmt::mp("final");
        }


        void
        sym_dec_tagged_bb_impl::init_ctx()
        {
            if (1 != EVP_DecryptInit_ex(d_ciph_ctx, d_ciph, NULL, &d_key[0], &d_iv[0])) {
                ERR_print_errors_fp(stdout);
            }
            if (1 != EVP_CIPHER_CTX_set_padding(d_ciph_ctx, d_padding)) {
                ERR_print_errors_fp(stdout);
            }
        }

        sym_dec_tagged_bb_impl::~sym_dec_tagged_bb_impl()
        {
            d_key.assign(d_ciph->key_len, 0);
            EVP_CIPHER_CTX_free(d_ciph_ctx);
        }

        int
        sym_dec_tagged_bb_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
        {
            return ninput_items[0] + d_ciph->block_size;
        }

        int
        sym_dec_tagged_bb_impl::work(int noutput_items,
                                     gr_vector_int &ninput_items,
                                     gr_vector_const_void_star &input_items,
                                     gr_vector_void_star &output_items)
        {
            const unsigned char *in = (const unsigned char *) input_items[0];
            unsigned char *out = (unsigned char *) output_items[0];

            std::vector<gr::tag_t> t_iv, t_final;
            get_tags_in_window(t_iv, 0, 0, ninput_items[0], d_iv_tagkey);
            get_tags_in_window(t_final, 0, 0, ninput_items[0], d_final_tagkey);

            //new IV
            if (t_iv.size()) {
                pmt::pmt_t p = t_iv.front().value;
                if (pmt::is_u8vector(p) && pmt::length(p) == d_ciph->iv_len) {

                    //final flag was not set before new iv->fails cause cannot prepend old packet data to new packet
                    if(d_have_iv){
                        int nout=0;
                        if (1 != EVP_DecryptFinal_ex(d_ciph_ctx, out, &nout)) {
                            ERR_print_errors_fp(stdout);
                        }
                        if(nout){
                            throw std::runtime_error("sym_dec_tagged_bb packet boundary failure, iv tag before final tag?");
                        }
                    }

                    d_iv = u8vector_elements(p);
                    d_have_iv = true;
                    //initialize decryption for new iv
                    init_ctx();
                }
            }
            //error if decryption needs iv
            if (!d_have_iv) {
                throw std::runtime_error("ERROR decryption without iv\n");
            }
            //decrypt
            int nout = 0;
            if (1 != EVP_DecryptUpdate(d_ciph_ctx, out, &nout, in, ninput_items[0])) {
                ERR_print_errors_fp(stdout);
            }

            if(t_final.size()){
                int tmp=0;
                if (1 != EVP_DecryptFinal_ex(d_ciph_ctx, out+nout, &tmp)) {
                    ERR_print_errors_fp(stdout);
                }
                nout=nout+tmp;
                d_have_iv = false || (d_ciph->iv_len == 0);
            }

            //printf("nin: %i, nout: %i, noutput_work: %i\n", ninput_items[0], nout, noutput_items);

            return nout;
        }

    } /* namespace crypto */
} /* namespace gr */

