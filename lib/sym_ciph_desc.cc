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
#include <crypto/sym_ciph_desc.h>

namespace gr {
    namespace crypto {

        sym_ciph_desc::sym_ciph_desc(const std::string ciph_name, bool padding, const std::string keyfilename,
                                     bool random_iv, const std::vector<unsigned char> &iv)
        {
            ERR_load_crypto_strings();
            OpenSSL_add_all_ciphers();
            OPENSSL_config(NULL);

            d_evp_ciph = EVP_get_cipherbyname(ciph_name.c_str());
            if (d_evp_ciph == NULL) throw std::runtime_error("cipher not found\n");

            printf("loaded cipher: %s, iv-length: %i, key-length: %i, block-size: %i\n",
                   ciph_name.c_str(), d_evp_ciph->iv_len, d_evp_ciph->key_len, d_evp_ciph->block_size);

            d_padding = padding;
            d_keyfilename = keyfilename;
            d_random_iv = random_iv;

            if (!d_random_iv) {
                if (iv.size() != d_evp_ciph->iv_len)
                    printf("WARNING length(%i) of given iv is wrong\n", (int)iv.size());
                d_start_iv = iv;
            }
        }


        void
        sym_ciph_desc::get_key(std::vector<unsigned char> &key)
        {
            crypt_helper::read_key_file(d_keyfilename, &key[0], d_evp_ciph->key_len);
        }

        sym_ciph_desc::~sym_ciph_desc()
        {
        }

    } /* namespace crypto */
} /* namespace gr */

