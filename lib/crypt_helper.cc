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
#include <crypto/crypt_helper.h>

namespace gr {
  namespace crypto {

    crypt_helper::crypt_helper()
    {
    }

    crypt_helper::~crypt_helper()
    {
    }

    void crypt_helper::gen_rand_bytes(unsigned char *b, int len){
        RAND_bytes(b, len);
    }
    void crypt_helper::read_key_file(const std::string filename, unsigned char *key, int keylen){
        std::ifstream keyfile(filename.c_str(),std::ios::in | std::ios::binary);
        if (!(keyfile.is_open())) { throw std::runtime_error("key file read error"); }
        keyfile.read((char *) &key[0], keylen);
        keyfile.close();
    }
    void crypt_helper::write_key_file(const std::string filename, unsigned char *key, int keylen){
        std::ofstream keyfile(filename.c_str(), std::ios::out | std::ios::binary);
        if (!(keyfile.is_open())) { throw std::runtime_error("key file write error"); }
        keyfile.write((char *) &key[0], keylen);
        keyfile.close();
    }

  } /* namespace crypto */
} /* namespace gr */

