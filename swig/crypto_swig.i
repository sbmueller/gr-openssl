/* -*- c++ -*- */

#define CRYPTO_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "crypto_swig_doc.i"

%{
#include "crypto/sym_enc_tagged_bb.h"
#include "crypto/key.h"
#include "crypto/sym_ciph_desc.h"
#include "crypto/sym_dec_tagged_bb.h"
#include "crypto/sym_enc.h"
#include "crypto/sym_dec.h"
#include "crypto/hash.h"
#include "crypto/auth_enc_aes_gcm.h"
#include "crypto/auth_dec_aes_gcm.h"
%}

%include "crypto/key.h"
%include "crypto/sym_ciph_desc.h"
%include "crypto/sym_enc_tagged_bb.h"
GR_SWIG_BLOCK_MAGIC2(crypto, sym_enc_tagged_bb);
%include "crypto/sym_dec_tagged_bb.h"
GR_SWIG_BLOCK_MAGIC2(crypto, sym_dec_tagged_bb);
%include "crypto/sym_enc.h"
GR_SWIG_BLOCK_MAGIC2(crypto, sym_enc);
%include "crypto/sym_dec.h"
GR_SWIG_BLOCK_MAGIC2(crypto, sym_dec);
%include "crypto/hash.h"
GR_SWIG_BLOCK_MAGIC2(crypto, hash);
%include "crypto/auth_enc_aes_gcm.h"
GR_SWIG_BLOCK_MAGIC2(crypto, auth_enc_aes_gcm);
%include "crypto/auth_dec_aes_gcm.h"
GR_SWIG_BLOCK_MAGIC2(crypto, auth_dec_aes_gcm);
