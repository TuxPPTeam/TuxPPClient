/*
 * Minimal configuration for TLS 1.1 (RFC 4346), implementing only the
 * required ciphersuite: TLS_RSA_WITH_3DES_EDE_CBC_SHA
 *
 * See README.txt for usage instructions.
 */

#ifndef POLARSSL_CONFIG_H
#define POLARSSL_CONFIG_H

/* PolarSSL feature support */
//#define POLARSSL_CIPHER_MODE_CBC

/* PolarSSL modules */
#define POLARSSL_AES_C
#define POLARSSL_SHA512_C
#define POLARSSL_HAVEGE_C
#define POLARSSL_TIMING_C

#include "polarssl/check_config.h"

#endif /* POLARSSL_CONFIG_H */
