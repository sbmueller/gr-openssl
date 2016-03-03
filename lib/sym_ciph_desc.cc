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
#include <stdexcept>

namespace gr {
  namespace crypto {

    sym_ciph_desc::sym_ciph_desc(const std::string ciph_name, bool padding)
    {
        ERR_load_crypto_strings();
        OpenSSL_add_all_ciphers();
        OPENSSL_config(NULL);

        d_evp_ciph = EVP_get_cipherbyname(ciph_name.c_str());
        if(d_evp_ciph == NULL) throw std::runtime_error("cipher not found");
        d_padding = padding;
    }

    sym_ciph_desc::~sym_ciph_desc()
    {
    }

  } /* namespace crypto */
} /* namespace gr */

