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


#ifndef INCLUDED_CRYPTO_SYM_CIPH_DESC_H
#define INCLUDED_CRYPTO_SYM_CIPH_DESC_H

#include <crypto/api.h>
#include <crypto/crypt_helper.h>
#include <stdexcept>


namespace gr {
    namespace crypto {

        /*!
         * \brief describes a symmetric block cipher
         *
         */
        class CRYPTO_API sym_ciph_desc
        {
        public:
            typedef boost::shared_ptr<sym_ciph_desc> sptr;

            sym_ciph_desc(const std::string ciph_name, bool padding, const std::string keyfilename,
                          bool random_iv, const std::vector<unsigned char> &iv);

            ~sym_ciph_desc();

            const EVP_CIPHER *get_evp_ciph() { return d_evp_ciph; };

            bool get_padding() { return d_padding; };

            bool get_random_iv() { return d_random_iv; };

            void get_start_iv(std::vector<unsigned char> &iv) { iv = d_start_iv; }

            void get_key(std::vector<unsigned char> &key);

        private:
            const EVP_CIPHER *d_evp_ciph;
            bool d_padding;
            bool d_random_iv;
            std::vector<unsigned char> d_start_iv;
            std::string d_keyfilename;
        };

    } // namespace crypto
} // namespace gr

#endif /* INCLUDED_CRYPTO_SYM_CIPH_DESC_H */

