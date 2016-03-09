#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Wed Mar  9 11:50:49 2016
##################################################

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from PyQt4 import Qt
from gnuradio import analog
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import crypto
import sip
import sys


class top_block(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Top Block")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Top Block")
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Variables
        ##################################################
        self.password = password = "beer"
        self.samp_rate = samp_rate = 100000
        self.key = key = crypto.key.make_pass_key(32,password)

        ##################################################
        # Blocks
        ##################################################
        self.crypto_sym_ciph_desc_0 = crypto.sym_ciph_desc("aes-256-ctr", False, (key))
        self.qtgui_time_raster_sink_x_0_3 = qtgui.time_raster_sink_b(
        	samp_rate,
        	25,
        	100,
        	([]),
        	([]),
        	"decrypted",
        	1,
        	)
        
        self.qtgui_time_raster_sink_x_0_3.set_update_time(0.10)
        self.qtgui_time_raster_sink_x_0_3.set_intensity_range(-128, 127)
        self.qtgui_time_raster_sink_x_0_3.enable_grid(False)
        self.qtgui_time_raster_sink_x_0_3.enable_axis_labels(True)
        
        labels = ["", "", "", "", "",
                  "", "", "", "", ""]
        colors = [0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_time_raster_sink_x_0_3.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_raster_sink_x_0_3.set_line_label(i, labels[i])
            self.qtgui_time_raster_sink_x_0_3.set_color_map(i, colors[i])
            self.qtgui_time_raster_sink_x_0_3.set_line_alpha(i, alphas[i])
        
        self._qtgui_time_raster_sink_x_0_3_win = sip.wrapinstance(self.qtgui_time_raster_sink_x_0_3.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_time_raster_sink_x_0_3_win)
        self.qtgui_time_raster_sink_x_0_2 = qtgui.time_raster_sink_b(
        	samp_rate,
        	25,
        	100,
        	([]),
        	([]),
        	"cipher",
        	1,
        	)
        
        self.qtgui_time_raster_sink_x_0_2.set_update_time(0.10)
        self.qtgui_time_raster_sink_x_0_2.set_intensity_range(-128, 127)
        self.qtgui_time_raster_sink_x_0_2.enable_grid(False)
        self.qtgui_time_raster_sink_x_0_2.enable_axis_labels(True)
        
        labels = ["", "", "", "", "",
                  "", "", "", "", ""]
        colors = [0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_time_raster_sink_x_0_2.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_raster_sink_x_0_2.set_line_label(i, labels[i])
            self.qtgui_time_raster_sink_x_0_2.set_color_map(i, colors[i])
            self.qtgui_time_raster_sink_x_0_2.set_line_alpha(i, alphas[i])
        
        self._qtgui_time_raster_sink_x_0_2_win = sip.wrapinstance(self.qtgui_time_raster_sink_x_0_2.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_time_raster_sink_x_0_2_win)
        self.qtgui_time_raster_sink_x_0 = qtgui.time_raster_sink_b(
        	samp_rate,
        	25,
        	100,
        	([]),
        	([]),
        	"plain",
        	1,
        	)
        
        self.qtgui_time_raster_sink_x_0.set_update_time(0.10)
        self.qtgui_time_raster_sink_x_0.set_intensity_range(-128, 127)
        self.qtgui_time_raster_sink_x_0.enable_grid(True)
        self.qtgui_time_raster_sink_x_0.enable_axis_labels(True)
        
        labels = ["", "", "", "", "",
                  "", "", "", "", ""]
        colors = [0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_time_raster_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_raster_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_time_raster_sink_x_0.set_color_map(i, colors[i])
            self.qtgui_time_raster_sink_x_0.set_line_alpha(i, alphas[i])
        
        self._qtgui_time_raster_sink_x_0_win = sip.wrapinstance(self.qtgui_time_raster_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_time_raster_sink_x_0_win)
        self.crypto_sym_enc_0 = crypto.sym_enc(self.crypto_sym_ciph_desc_0)
        self.crypto_sym_dec_0 = crypto.sym_dec(self.crypto_sym_ciph_desc_0)
        self.blocks_throttle_0 = blocks.throttle(gr.sizeof_char*1, samp_rate,True)
        self.blocks_tagged_stream_to_pdu_0 = blocks.tagged_stream_to_pdu(blocks.byte_t, "packet_len")
        self.blocks_stream_to_tagged_stream_0 = blocks.stream_to_tagged_stream(gr.sizeof_char, 1, 320, "packet_len")
        self.blocks_pdu_to_tagged_stream_0_0 = blocks.pdu_to_tagged_stream(blocks.byte_t, "packet_len")
        self.blocks_pdu_to_tagged_stream_0 = blocks.pdu_to_tagged_stream(blocks.byte_t, "packet_len")
        self.blocks_float_to_char_0 = blocks.float_to_char(1, 125)
        self.analog_sig_source_x_0 = analog.sig_source_f(samp_rate, analog.GR_COS_WAVE, 1000, 1, 0)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.blocks_tagged_stream_to_pdu_0, 'pdus'), (self.crypto_sym_enc_0, 'pdus'))    
        self.msg_connect((self.crypto_sym_dec_0, 'pdus'), (self.blocks_pdu_to_tagged_stream_0, 'pdus'))    
        self.msg_connect((self.crypto_sym_enc_0, 'pdus'), (self.blocks_pdu_to_tagged_stream_0_0, 'pdus'))    
        self.msg_connect((self.crypto_sym_enc_0, 'pdus'), (self.crypto_sym_dec_0, 'pdus'))    
        self.connect((self.analog_sig_source_x_0, 0), (self.blocks_float_to_char_0, 0))    
        self.connect((self.blocks_float_to_char_0, 0), (self.blocks_throttle_0, 0))    
        self.connect((self.blocks_pdu_to_tagged_stream_0, 0), (self.qtgui_time_raster_sink_x_0_3, 0))    
        self.connect((self.blocks_pdu_to_tagged_stream_0_0, 0), (self.qtgui_time_raster_sink_x_0_2, 0))    
        self.connect((self.blocks_stream_to_tagged_stream_0, 0), (self.blocks_tagged_stream_to_pdu_0, 0))    
        self.connect((self.blocks_stream_to_tagged_stream_0, 0), (self.qtgui_time_raster_sink_x_0, 0))    
        self.connect((self.blocks_throttle_0, 0), (self.blocks_stream_to_tagged_stream_0, 0))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()


    def get_password(self):
        return self.password

    def set_password(self, password):
        self.password = password
        self.set_key(crypto.self.key.make_pass_key(32,self.password))

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_throttle_0.set_sample_rate(self.samp_rate)
        self.analog_sig_source_x_0.set_sampling_freq(self.samp_rate)

    def get_key(self):
        return self.key

    def set_key(self, key):
        self.key = key
        self.set_key(crypto.self.key.make_pass_key(32,self.password))


def main(top_block_cls=top_block, options=None):

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
