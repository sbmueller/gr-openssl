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
#include "sym_enc_impl.h"


namespace gr {
  namespace crypto {

    sym_enc::sptr
    sym_enc::make(sym_ciph_desc &ciph_desc)
    {
      return gnuradio::get_initial_sptr
        (new sym_enc_impl(ciph_desc));
    }

    /*
     * The private constructor
     */
    sym_enc_impl::sym_enc_impl(sym_ciph_desc &ciph_desc)
      : gr::block("sym_enc",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(0,0,0))
    {}

    /*
     * Our virtual destructor.
     */
    sym_enc_impl::~sym_enc_impl()
    {
    }

   

  } /* namespace crypto */
} /* namespace gr */

