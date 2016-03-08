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


#ifndef INCLUDED_CRYPTO_HASH_H
#define INCLUDED_CRYPTO_HASH_H

#include <crypto/api.h>
#include <gnuradio/block.h>
#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/err.h>

namespace gr {
  namespace crypto {

    /*!
     * \brief calculates the hash of a pdu binary data
     * \ingroup crypto
     *
     */
    class CRYPTO_API hash : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<hash> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of crypto::hash.
       *
       * To avoid accidental use of raw pointers, crypto::hash's
       * constructor is in a private implementation
       * class. crypto::hash::make is the public interface for
       * creating new instances.
       */
      static sptr make(const std::string &hash_name);
    };

  } // namespace crypto
} // namespace gr

#endif /* INCLUDED_CRYPTO_HASH_H */

