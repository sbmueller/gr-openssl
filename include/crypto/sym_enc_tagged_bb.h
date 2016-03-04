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


#ifndef INCLUDED_CRYPTO_SYM_ENC_TAGGED_BB_H
#define INCLUDED_CRYPTO_SYM_ENC_TAGGED_BB_H

#include <crypto/api.h>
#include <gnuradio/tagged_stream_block.h>
#include <crypto/sym_ciph_desc.h>

namespace gr {
    namespace crypto {

        /*!
         * \brief symmetric key encryption with block ciphers
         * \ingroup crypto
         *
         */
        class CRYPTO_API sym_enc_tagged_bb : virtual public gr::tagged_stream_block {
        public:
            typedef boost::shared_ptr<sym_enc_tagged_bb> sptr;

            /*!
             * \brief Return a shared_ptr to a new instance of crypto::sym_enc_tagged_bb.
             *
             * To avoid accidental use of raw pointers, crypto::sym_enc_tagged_bb's
             * constructor is in a private implementation
             * class. crypto::sym_enc_tagged_bb::make is the public interface for
             * creating new instances.
             */
            static sptr make(sym_ciph_desc &ciph_desc, const std::string &packet_len_key);
        };

    } // namespace crypto
} // namespace gr

#endif /* INCLUDED_CRYPTO_SYM_ENC_TAGGED_BB_H */

