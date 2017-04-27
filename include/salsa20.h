#ifndef _SALSA20_H_
#define _SALSA20_H_

#include <stdint.h>

void salsa_encrypt(uint8_t key[32], uint8_t nonce[8], uint8_t* m,
					 size_t size, uint8_t* c);
void salsa_decrypt(uint8_t key[32], uint8_t nonce[8], uint8_t* c,
					 size_t size, uint8_t* m);

#endif
