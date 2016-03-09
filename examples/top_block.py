#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Wed Mar  9 10:59:00 2016
##################################################

from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import fec
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import crypto
import numpy


class top_block(gr.top_block):

    def __init__(self):
        gr.top_block.__init__(self, "Top Block")

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 100000
        self.key = key = crypto.key.make_rand_key(32)

        ##################################################
        # Blocks
        ##################################################
        self.crypto_sym_ciph_desc_1 = crypto.sym_ciph_desc("aes-256-ctr", False, (key))
        self.crypto_sym_ciph_desc_0 = crypto.sym_ciph_desc("aes-256-ctr", False, (key))
        self.fec_ber_bf_0 = fec.ber_bf(False, 100, -7.0)
        self.crypto_sym_enc_tagged_bb_0 = crypto.sym_enc_tagged_bb(self.crypto_sym_ciph_desc_0, "packet_len")
        self.crypto_sym_dec_tagged_bb_0 = crypto.sym_dec_tagged_bb(self.crypto_sym_ciph_desc_1, "packet_len")
        self.blocks_throttle_0_0 = blocks.throttle(gr.sizeof_char*1, samp_rate,True)
        self.blocks_tag_debug_0 = blocks.tag_debug(gr.sizeof_char*1, "", ""); self.blocks_tag_debug_0.set_display(True)
        self.blocks_stream_to_tagged_stream_0 = blocks.stream_to_tagged_stream(gr.sizeof_char, 1, 1600, "packet_len")
        self.blocks_null_sink_0 = blocks.null_sink(gr.sizeof_float*1)
        self.analog_random_source_x_0 = blocks.vector_source_b(map(int, numpy.random.randint(0, 256, 100000)), True)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_random_source_x_0, 0), (self.blocks_throttle_0_0, 0))    
        self.connect((self.blocks_stream_to_tagged_stream_0, 0), (self.crypto_sym_enc_tagged_bb_0, 0))    
        self.connect((self.blocks_stream_to_tagged_stream_0, 0), (self.fec_ber_bf_0, 1))    
        self.connect((self.blocks_throttle_0_0, 0), (self.blocks_stream_to_tagged_stream_0, 0))    
        self.connect((self.crypto_sym_dec_tagged_bb_0, 0), (self.fec_ber_bf_0, 0))    
        self.connect((self.crypto_sym_enc_tagged_bb_0, 0), (self.blocks_tag_debug_0, 0))    
        self.connect((self.crypto_sym_enc_tagged_bb_0, 0), (self.crypto_sym_dec_tagged_bb_0, 0))    
        self.connect((self.fec_ber_bf_0, 0), (self.blocks_null_sink_0, 0))    

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0_0.set_sample_rate(self.samp_rate)

    def get_key(self):
        return self.key

    def set_key(self, key):
        self.key = key
        self.set_key(crypto.self.key.make_rand_key(32))


def main(top_block_cls=top_block, options=None):

    tb = top_block_cls()
    tb.start()
    try:
        raw_input('Press Enter to quit: ')
    except EOFError:
        pass
    tb.stop()
    tb.wait()


if __name__ == '__main__':
    main()
