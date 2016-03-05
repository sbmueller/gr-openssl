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
import pmt
from time import sleep


class qa_sym_enc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()



    def tearDown (self):
        self.tb = None

    #test correct encryption and decryption
    def test_001_t (self):
        cipher_name = "aes-256-cbc"
        keyfilename = "test_key.deleteme"
        plainlen = 32;

        key = bytearray(numpy.random.randint(0, 256, 32).tolist())
        plain=bytearray(numpy.random.randint(0, 256, plainlen).tolist())

        self.write_bytes_to_file(key, keyfilename)

        cipher_desc = crypto.sym_ciph_desc(cipher_name, False, keyfilename)
        src = blocks.message_strobe(pmt.init_u8vector(plainlen, plain),50)
        enc = crypto.sym_enc(cipher_desc)
        dec = crypto.sym_dec(cipher_desc)
        snk = blocks.message_debug()

        self.tb.msg_connect(src,"strobe", enc, "plain")
        self.tb.msg_connect(enc, "encrypted", dec, "encrypted")
        self.tb.msg_connect(dec, "decrypted", snk, "store")

        self.tb.start()
        sleep(0.75)
        src.set_period(1000000);
        self.tb.stop()
        self.tb.wait()

        #print pmt.u8vector_elements(snk.get_message(0))
        decrypted = bytearray(pmt.u8vector_elements(snk.get_message(0)))

        self.assertEqual(plain, decrypted)


    def write_bytes_to_file(self, b, filename):
        f = open(filename, "wb")
        f.write(b)
        f.close()
        # check data


if __name__ == '__main__':
    gr_unittest.run(qa_sym_enc)