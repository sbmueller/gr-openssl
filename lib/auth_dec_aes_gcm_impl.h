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

#ifndef INCLUDED_CRYPTO_AUTH_DEC_AES_GCM_IMPL_H
#define INCLUDED_CRYPTO_AUTH_DEC_AES_GCM_IMPL_H

#include <crypto/auth_dec_aes_gcm.h>
#include <stdexcept>
#include <stdint.h>
#include <openssl/evp.h>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/rand.h>

namespace gr {
  namespace crypto {

    class auth_dec_aes_gcm_impl : public auth_dec_aes_gcm
    {
     private:
        const EVP_CIPHER *d_ciph;
        EVP_CIPHER_CTX *d_ciph_ctx;
        unsigned long d_pdu_ctr;

        std::vector<uint8_t> d_key;
        std::vector<uint8_t> d_iv;
        int d_ivlen;
        bool d_have_iv;

        std::vector<uint8_t> d_out_buffer;

        pmt::pmt_t d_final_id;
        pmt::pmt_t d_iv_id;
        pmt::pmt_t d_auth_tag_id;
        pmt::pmt_t d_aad_id;

        void init_ctx();


     public:
      auth_dec_aes_gcm_impl(std::vector<uint8_t> key, int keylen, int ivlen);
      ~auth_dec_aes_gcm_impl();


        void msg_handler(pmt::pmt_t msg);

    };

  } // namespace crypto
} // namespace gr

#endif /* INCLUDED_CRYPTO_AUTH_DEC_AES_GCM_IMPL_H */

