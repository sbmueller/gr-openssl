#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2016 Kristian Maier.
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
import crypto_swig as crypto
import binascii
import numpy

class qa_sym_enc_tagged_bb (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    #encrypt, decrypt and compare one block
    def test_001_t (self):
        cipher_name = "aes-128-cbc"
        key = bytearray("aaaaaaaaaaaaaaaa")
        plain=bytearray("testesttesttestt")

        print "Key:   \t{0}, hex: \t{1}".format(key,binascii.hexlify(key))
        print "plain: \t{0}, hex: \t{1}".format(plain,binascii.hexlify(plain))

        cipher_desc = crypto.sym_ciph_desc(cipher_name, False, key)
        src = blocks.vector_source_b(plain)
        tagger = blocks.stream_to_tagged_stream(1, 1, 16, "packet_len")
        enc = crypto.sym_enc_tagged_bb(cipher_desc, "packet_len")
        snk_enc = blocks.vector_sink_b()
        dec = crypto.sym_dec_tagged_bb(cipher_desc, "packet_len")
        snk = blocks.vector_sink_b()

        self.tb.connect(src, tagger, enc, dec, snk)
        self.tb.connect(enc, snk_enc)
        self.tb.run()

        encrypted = bytearray(snk_enc.data())
        decrypted = bytearray(snk.data())

        print "Encrypted hex: {0}".format(binascii.hexlify(encrypted))
        print "Decrypted: \t{0}, hex: \t{1}".format(decrypted, binascii.hexlify(decrypted))

        self.assertEqual(plain, decrypted)


    #with more random data
    def test_002_t (self):

        cipher_name = "aes-256-cbc"
        key = bytearray("numpy.random.randint(0, 256, 16).tolist()")
        plain=bytearray(numpy.random.randint(0, 256, 16*10000).tolist())

        cipher_desc = crypto.sym_ciph_desc(cipher_name, False, key)

        src = blocks.vector_source_b(plain)
        tagger = blocks.stream_to_tagged_stream(1, 1, 160, "packet_len")
        enc = crypto.sym_enc_tagged_bb(cipher_desc, "packet_len")
        snk_enc = blocks.vector_sink_b()
        dec = crypto.sym_dec_tagged_bb(cipher_desc, "packet_len")
        snk = blocks.vector_sink_b()

        self.tb.connect(src, tagger, enc, dec, snk)
        self.tb.connect(enc, snk_enc)
        self.tb.run()

        encrypted = bytearray(snk_enc.data())
        decrypted = bytearray(snk.data())

        self.assertEqual(plain, decrypted)



if __name__ == '__main__':
    gr_unittest.run(qa_sym_enc_tagged_bb)#, "qa_sym_enc_tagged_bb.xml")
