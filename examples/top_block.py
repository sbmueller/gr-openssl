#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Sat Mar  5 03:38:06 2016
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
        self.samp_rate = samp_rate = 100000

        ##################################################
        # Blocks
        ##################################################
        self.crypto_sym_ciph_desc_0 = crypto.sym_ciph_desc("aes-256-ecb", False, "/home/maier/meinkey.bin", True, [])
        self.qtgui_time_raster_sink_x_0_0_1_0_1_0_1 = qtgui.time_raster_sink_b(
        	samp_rate,
        	16*4,
        	8*8,
        	([]),
        	([]),
        	"decrypt",
        	1,
        	)
        
        self.qtgui_time_raster_sink_x_0_0_1_0_1_0_1.set_update_time(0.00010)
        self.qtgui_time_raster_sink_x_0_0_1_0_1_0_1.set_intensity_range(0, 256)
        self.qtgui_time_raster_sink_x_0_0_1_0_1_0_1.enable_grid(True)
        self.qtgui_time_raster_sink_x_0_0_1_0_1_0_1.enable_axis_labels(True)
        
        labels = ["", "", "", "", "",
                  "", "", "", "", ""]
        colors = [0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_time_raster_sink_x_0_0_1_0_1_0_1.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_raster_sink_x_0_0_1_0_1_0_1.set_line_label(i, labels[i])
            self.qtgui_time_raster_sink_x_0_0_1_0_1_0_1.set_color_map(i, colors[i])
            self.qtgui_time_raster_sink_x_0_0_1_0_1_0_1.set_line_alpha(i, alphas[i])
        
        self._qtgui_time_raster_sink_x_0_0_1_0_1_0_1_win = sip.wrapinstance(self.qtgui_time_raster_sink_x_0_0_1_0_1_0_1.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_time_raster_sink_x_0_0_1_0_1_0_1_win)
        self.qtgui_time_raster_sink_x_0_0_1_0_1_0_0 = qtgui.time_raster_sink_b(
        	samp_rate,
        	16*4,
        	8*8,
        	([]),
        	([]),
        	"cipher",
        	1,
        	)
        
        self.qtgui_time_raster_sink_x_0_0_1_0_1_0_0.set_update_time(0.010)
        self.qtgui_time_raster_sink_x_0_0_1_0_1_0_0.set_intensity_range(0, 256)
        self.qtgui_time_raster_sink_x_0_0_1_0_1_0_0.enable_grid(True)
        self.qtgui_time_raster_sink_x_0_0_1_0_1_0_0.enable_axis_labels(True)
        
        labels = ["", "", "", "", "",
                  "", "", "", "", ""]
        colors = [0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_time_raster_sink_x_0_0_1_0_1_0_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_raster_sink_x_0_0_1_0_1_0_0.set_line_label(i, labels[i])
            self.qtgui_time_raster_sink_x_0_0_1_0_1_0_0.set_color_map(i, colors[i])
            self.qtgui_time_raster_sink_x_0_0_1_0_1_0_0.set_line_alpha(i, alphas[i])
        
        self._qtgui_time_raster_sink_x_0_0_1_0_1_0_0_win = sip.wrapinstance(self.qtgui_time_raster_sink_x_0_0_1_0_1_0_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_time_raster_sink_x_0_0_1_0_1_0_0_win)
        self.qtgui_number_sink_0 = qtgui.number_sink(
            gr.sizeof_char,
            0,
            qtgui.NUM_GRAPH_HORIZ,
            1
        )
        self.qtgui_number_sink_0.set_update_time(0.10)
        self.qtgui_number_sink_0.set_title("")
        
        labels = ["", "", "", "", "",
                  "", "", "", "", ""]
        units = ["", "", "", "", "",
                 "", "", "", "", ""]
        colors = [("black", "black"), ("black", "black"), ("black", "black"), ("black", "black"), ("black", "black"),
                  ("black", "black"), ("black", "black"), ("black", "black"), ("black", "black"), ("black", "black")]
        factor = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        for i in xrange(1):
            self.qtgui_number_sink_0.set_min(i, -128)
            self.qtgui_number_sink_0.set_max(i, 127)
            self.qtgui_number_sink_0.set_color(i, colors[i][0], colors[i][1])
            if len(labels[i]) == 0:
                self.qtgui_number_sink_0.set_label(i, "Data {0}".format(i))
            else:
                self.qtgui_number_sink_0.set_label(i, labels[i])
            self.qtgui_number_sink_0.set_unit(i, units[i])
            self.qtgui_number_sink_0.set_factor(i, factor[i])
        
        self.qtgui_number_sink_0.enable_autoscale(False)
        self._qtgui_number_sink_0_win = sip.wrapinstance(self.qtgui_number_sink_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_number_sink_0_win)
        self.crypto_sym_enc_tagged_bb_0 = crypto.sym_enc_tagged_bb(self.crypto_sym_ciph_desc_0, "packet_len")
        self.crypto_sym_dec_tagged_bb_0 = crypto.sym_dec_tagged_bb(self.crypto_sym_ciph_desc_0, "packet_len")
        self.blocks_vector_source_x_0 = blocks.vector_source_b((65,), True, 1, [])
        self.blocks_stream_to_tagged_stream_0 = blocks.stream_to_tagged_stream(gr.sizeof_char, 1, 16, "packet_len")
        self.blocks_null_sink_0 = blocks.null_sink(gr.sizeof_char*1)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_char*1, "/home/maier/deletememessage", False)
        self.blocks_file_sink_0.set_unbuffered(False)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.blocks_stream_to_tagged_stream_0, 0), (self.crypto_sym_enc_tagged_bb_0, 0))    
        self.connect((self.blocks_vector_source_x_0, 0), (self.blocks_stream_to_tagged_stream_0, 0))    
        self.connect((self.crypto_sym_dec_tagged_bb_0, 0), (self.blocks_null_sink_0, 0))    
        self.connect((self.crypto_sym_dec_tagged_bb_0, 0), (self.qtgui_number_sink_0, 0))    
        self.connect((self.crypto_sym_dec_tagged_bb_0, 0), (self.qtgui_time_raster_sink_x_0_0_1_0_1_0_1, 0))    
        self.connect((self.crypto_sym_enc_tagged_bb_0, 0), (self.blocks_file_sink_0, 0))    
        self.connect((self.crypto_sym_enc_tagged_bb_0, 0), (self.crypto_sym_dec_tagged_bb_0, 0))    
        self.connect((self.crypto_sym_enc_tagged_bb_0, 0), (self.qtgui_time_raster_sink_x_0_0_1_0_1_0_0, 0))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()


    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate


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
