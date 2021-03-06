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


#ifndef INCLUDED_CRYPTO_GENERATE_KEY_H
#define INCLUDED_CRYPTO_GENERATE_KEY_H

#include <crypto/api.h>
#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <stdint.h>
#include <fstream>
#include <stdexcept>

namespace gr {
    namespace crypto {

        /*!
         * \brief generate keys
         *
         *
         */
        class CRYPTO_API key
        {
        public:
            static std::vector<uint8_t> make_pass_key(int keylen, const std::string &password,
                                                      const unsigned char *salt, int saltlen, int hashrounds);
            static std::vector<uint8_t> make_pass_key(int keylen, const std::string &password);
            static std::vector<uint8_t> make_rand_key(int keylen);

        private:

        };

    } // namespace crypto
} // namespace gr

#endif /* INCLUDED_CRYPTO_GENERATE_KEY_H */

