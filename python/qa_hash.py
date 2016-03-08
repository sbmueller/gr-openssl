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
import pmt

class qa_hash (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        hash_func = "md5"
        text = bytearray("ABCDEFGH")
        exp_result = bytearray(["\x47","\x83","\xe7","\x84","\xb4","\xfa","\x2f","\xba","\x9e","\x4d","\x65","\x02","\xdb","\xc6","\x4f","\x8f"])
        src = blocks.vector_source_b(text)
        stts = blocks.stream_to_tagged_stream(1, 1, 8, "packet_len")
        tstpdu = blocks.tagged_stream_to_pdu(blocks.byte_t, "packet_len")
        hash_block = crypto.hash(hash_func)
        snk = blocks.message_debug()

        self.tb.connect(src, stts, tstpdu);
        self.tb.msg_connect(tstpdu, "pdus", hash_block, "pdus")
        self.tb.msg_connect(hash_block, "pdus", snk, "store")

        self.tb.run()

        hash = bytearray(pmt.u8vector_elements(pmt.cdr((snk.get_message(0)))))
        self.assertEqual(exp_result, hash)


if __name__ == '__main__':
    gr_unittest.run(qa_hash, "qa_hash.xml")
