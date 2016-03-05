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
        keyfilename = "test_key.deleteme"

        #key = numpy.random.randint(0, 256, 16).tolist()
        key = bytearray("aaaaaaaaaaaaaaaa")
        plain=bytearray("testesttesttestt")

        print "\nTest 01"
        print "Key: \t{0}, hex: \t{1}".format(key,binascii.hexlify(key))
        print "plain: \t{0}, hex: \t{1}".format(plain,binascii.hexlify(plain))

        self.write_bytes_to_file(key, keyfilename)

        cipher_desc = crypto.sym_ciph_desc(cipher_name, False, keyfilename)
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
        keyfilename = "test_key.deleteme"

        #key = numpy.random.randint(0, 256, 16).tolist()
        print "\nTest03"
        key = bytearray("aaaaaaaaaaaaaaaa")
        plain=bytearray(numpy.random.randint(0, 256, 16*10000).tolist())

        self.write_bytes_to_file(key, keyfilename)

        cipher_desc = crypto.sym_ciph_desc(cipher_name, False, keyfilename)

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


    def write_bytes_to_file(self, b, filename):
        f = open(filename, "wb")
        f.write(b)
        f.close()
        # check data


if __name__ == '__main__':
    gr_unittest.run(qa_sym_enc_tagged_bb)#, "qa_sym_enc_tagged_bb.xml")
