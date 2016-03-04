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


#ifndef INCLUDED_CRYPTO_CRYPT_HELPER_H
#define INCLUDED_CRYPTO_CRYPT_HELPER_H

#include <crypto/api.h>
#include <fstream>
#include <stdexcept>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rand.h>

namespace gr {
  namespace crypto {

    /*!
     * \brief <+description+>
     *
     */
    class CRYPTO_API crypt_helper
    {
    public:
      crypt_helper();
      ~crypt_helper();

      static void gen_rand_bytes(unsigned char *b, int len);
      static void read_key_file(const std::string filename, unsigned char *key, int keylen);
      static void write_key_file(const std::string filename, unsigned char *key, int keylen);

    private:
    };

  } // namespace crypto
} // namespace gr

#endif /* INCLUDED_CRYPTO_CRYPT_HELPER_H */

