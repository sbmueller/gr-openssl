/* -*- c++ -*- */

#define CRYPTO_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "crypto_swig_doc.i"

%{
#include "crypto/sym_enc_tagged_bb.h"
#include "crypto/sym_ciph_desc.h"
#include "crypto/generate_key.h"
#include "crypto/crypt_helper.h"
#include "crypto/sym_dec_tagged_bb.h"
%}

%include "crypto/sym_ciph_desc.h"		//muss vorne stehen, sonst Fehler
%include "crypto/sym_enc_tagged_bb.h"
GR_SWIG_BLOCK_MAGIC2(crypto, sym_enc_tagged_bb);

%include "crypto/generate_key.h"
%include "crypto/crypt_helper.h"
%include "crypto/sym_dec_tagged_bb.h"
GR_SWIG_BLOCK_MAGIC2(crypto, sym_dec_tagged_bb);
