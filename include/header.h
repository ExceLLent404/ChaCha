#ifndef _HEADER_H_
#define _HEADER_H_

#include <stdint.h>
#include <stddef.h>

/* The functions for Salsa20 cipher */
void salsa_quarterround(uint32_t *y0, uint32_t *y1,
			 uint32_t *y2, uint32_t *y3);
void salsa_rowround(uint32_t y[16]);
void salsa_columnround(uint32_t x[16]);
void salsa_doubleround(uint32_t x[16]);
void salsa_hash(uint8_t x[64]);
void salsa_expand(uint8_t k[32], uint8_t n[16], uint8_t seq[64]);
void salsa_encrypt(uint8_t key[32], uint8_t nonce[8], uint8_t* m,
					 size_t size, uint8_t* c);
void salsa_decrypt(uint8_t key[32], uint8_t nonce[8], uint8_t* c,
					 size_t size, uint8_t* m);

/* The functions for ChaCha cipher */
void chacha_quarterround(uint32_t *a, uint32_t *b,
			 uint32_t *c, uint32_t *d);
void chacha_rowround(uint32_t y[16]);
void chacha_columnround(uint32_t x[16]);
void chacha_doubleround(uint32_t x[16]);
void chacha_hash(uint8_t x[64]);
void chacha_expand(uint8_t k[32], uint8_t n[16], uint8_t seq[64]);
void chacha_encrypt(uint8_t key[32], uint8_t nonce[8], uint8_t* m,
					 size_t size, uint8_t* c);
void chacha_decrypt(uint8_t key[32], uint8_t nonce[8], uint8_t* c,
					 size_t size, uint8_t* m);

/* The functions for both ciphers */
uint32_t littleendian(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3);
void wordtobyte(uint8_t* byte, uint32_t word);

/* The function for checking ciphers */
void salsa_check();

#endif
