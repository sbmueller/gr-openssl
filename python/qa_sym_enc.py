#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2016 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks
#import crypto_swig as crypto
import numpy
import crypto
import binascii

class qa_sym_enc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()



    def tearDown (self):
        self.tb = None

    #test correct encryption
    def test_001_t (self):
        cipher_name = "aes-128-cbc"
        keyfilename = "test_key.deleteme"

        key = bytearray(numpy.random.randint(0, 256, 16).tolist())
        plain=bytearray(numpy.random.randint(0, 256, 16).tolist())

        self.write_bytes_to_file(key, keyfilename)

        #TODO: finish :D
        cipher_desc = crypto.sym_ciph_desc(cipher_name, False, keyfilename)
        src = blocks.message_strobe(pmt.init_u8vector(16, plain))
        enc = crypto.sym_enc(cipher_desc)
        dec = crypto.sym_dec(cipher_desc)

        self.tb.connect(src, tagger, enc, dec, snk)
        self.tb.connect(enc, snk_enc)
        self.tb.run()

        encrypted = bytearray(snk_enc.data())
        decrypted = bytearray(snk.data())

        self.assertEqual(plain, decrypted)


    def write_bytes_to_file(self, b, filename):
        f = open(filename, "wb")
        f.write(b)
        f.close()
        # check data


if __name__ == '__main__':
    gr_unittest.run(qa_sym_enc, "qa_sym_enc.xml")
