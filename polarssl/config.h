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
#define POLARSSL_CTR_DRBG_C
#define POLARSSL_ENTROPY_C
#define POLARSSL_PK_C
#define POLARSSL_MD_C
#define POLARSSL_RSA_C
#define POLARSSL_BIGNUM_C
#define POLARSSL_OID_C
#define POLARSSL_PK_PARSE_C
#define POLARSSL_FS_IO
#define POLARSSL_ASN1_PARSE_C
#define POLARSSL_PEM_PARSE_C
#define POLARSSL_BASE64_C
#define POLARSSL_PKCS1_V15

#include "polarssl/check_config.h"

#endif /* POLARSSL_CONFIG_H */
