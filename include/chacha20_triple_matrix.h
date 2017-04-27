#ifndef _CHACHA20_TRIPLE_MATRIX_H_
#define _CHACHA20_TRIPLE_MATRIX_H_

#include <stdint.h>

/* "_i" means improved */
void chacha_encrypt_i(uint8_t key[32], uint8_t nonce[8], uint8_t* m,
					 size_t size, uint8_t* c);
void chacha_decrypt_i(uint8_t key[32], uint8_t nonce[8], uint8_t* c,
					 size_t size, uint8_t* m);

#endif
