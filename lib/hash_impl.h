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

#ifndef INCLUDED_CRYPTO_HASH_IMPL_H
#define INCLUDED_CRYPTO_HASH_IMPL_H

#include <crypto/hash.h>

namespace gr {
  namespace crypto {

    class hash_impl : public hash
    {
     private:
      const EVP_MD *d_md;
      EVP_MD_CTX *d_md_ctx;
      std::vector<uint8_t> d_out_buffer;

     public:
      hash_impl(const std::string &hash_name);
      ~hash_impl();

      void msg_handler(pmt::pmt_t msg);

    };

  } // namespace crypto
} // namespace gr

#endif /* INCLUDED_CRYPTO_HASH_IMPL_H */

