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
#include <crypto/key.h>

namespace gr {
    namespace crypto {

        std::vector<uint8_t>
        key::make_pass_key(int keylen, const std::string &password)
        {
            std::vector<uint8_t> key(keylen, 0);
            if (!PKCS5_PBKDF2_HMAC(password.c_str(), -1, NULL, 0, 20000, EVP_sha256(), keylen, &key[0])) {
                ERR_print_errors_fp(stdout);
            }
            return key;
        }

        std::vector<uint8_t>
        key::make_pass_key(int keylen, const std::string &password,
                                    const unsigned char *salt, int saltlen, int hashrounds)
        {
            std::vector<uint8_t> key(keylen, 0);
            if (!PKCS5_PBKDF2_HMAC(password.c_str(), -1, salt, saltlen, hashrounds, EVP_sha256(), keylen, &key[0])) {
                ERR_print_errors_fp(stdout);
            }
            return key;
        }

        std::vector<uint8_t>
        key::make_rand_key(int keylen)
        {
            std::vector<uint8_t> key(keylen, 0);
            RAND_bytes(&key[0], keylen);
            return key;
        }


    } /* namespace crypto */
} /* namespace gr */

