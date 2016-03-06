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

#ifndef INCLUDED_CRYPTO_SYM_ENC_IMPL_H
#define INCLUDED_CRYPTO_SYM_ENC_IMPL_H

#include <crypto/sym_enc.h>
#include <crypto/sym_ciph_desc.h>

namespace gr {
  namespace crypto {

    class sym_enc_impl : public sym_enc
    {
     private:
        const EVP_CIPHER *d_ciph;
        EVP_CIPHER_CTX *d_ciph_ctx;
        bool d_padding;
        unsigned long d_pdu_ctr;

        std::vector<unsigned char> d_key;
        std::vector<unsigned char> d_iv;

        std::vector<unsigned char> d_out_buffer;

        pmt::pmt_t d_iv_id;

     public:
      sym_enc_impl(sym_ciph_desc &ciph_desc);
      ~sym_enc_impl();

      void msg_handler(pmt::pmt_t msg);


    };

  } // namespace crypto
} // namespace gr

#endif /* INCLUDED_CRYPTO_SYM_ENC_IMPL_H */

