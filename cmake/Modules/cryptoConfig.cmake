INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_CRYPTO crypto)

FIND_PATH(
    CRYPTO_INCLUDE_DIRS
    NAMES crypto/api.h
    HINTS $ENV{CRYPTO_DIR}/include
        ${PC_CRYPTO_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    CRYPTO_LIBRARIES
    NAMES gnuradio-crypto
    HINTS $ENV{CRYPTO_DIR}/lib
        ${PC_CRYPTO_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CRYPTO DEFAULT_MSG CRYPTO_LIBRARIES CRYPTO_INCLUDE_DIRS)
MARK_AS_ADVANCED(CRYPTO_LIBRARIES CRYPTO_INCLUDE_DIRS)

