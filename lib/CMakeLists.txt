# Copyright 2011,2012 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
include_directories(${OPENSSL_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND crypto_sources
	generate_key.cc
	crypt_helper.cc
    sym_enc_tagged_bb_impl.cc
	sym_dec_tagged_bb_impl.cc
    sym_ciph_desc.cc)


set(crypto_sources "${crypto_sources}" PARENT_SCOPE)
if(NOT crypto_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT crypto_sources)

add_library(gnuradio-crypto SHARED ${crypto_sources})
target_link_libraries(gnuradio-crypto ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-crypto PROPERTIES DEFINE_SYMBOL "gnuradio_crypto_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-crypto PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-crypto
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})

list(APPEND test_crypto_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_crypto.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_crypto.cc
)

add_executable(test-crypto ${test_crypto_sources})

target_link_libraries(
  test-crypto
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  ${OPENSSL_LIBRARIES}
  gnuradio-crypto
)

GR_ADD_TEST(test_crypto test-crypto)
