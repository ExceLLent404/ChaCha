#include <stdint.h>
#include <stddef.h>
#include "../include/header.h"

void chacha_doubleround_triple(uint32_t x[48])
{
	chacha_columnround_asm_triple(x);
	chacha_diagonalround_asm_triple(x);
}

void chacha_hash_i(uint8_t seq[64])
{
	int i, j;
	uint32_t x[16], z[16];

	for (i = 0; i < 16; i++)
		x[i] = z[i] = *(uint32_t *)(seq + i * 4);
	for (i = 0; i < 10; i++)
		chacha_doubleround(z);
	for (i = 0; i < 16; i++) {
		x[i] += z[i];
		for (j = 0; j < 4; j++)
			seq[i * 4 + j] = *((uint8_t *)&x[i] + j);
	}
}

void chacha_hash_triple(uint8_t seq[192])
{
	int i, j;
	uint32_t x[48], z[48];

	for (i = 0; i < 48; i++)
		x[i] = z[i] = *(uint32_t *)(seq + i * 4);
	for (i = 0; i < 10; i++)
		chacha_doubleround_triple(z);
	for (i = 0; i < 48; i++) {
		x[i] += z[i];
		for (j = 0; j < 4; j++)
			seq[i * 4 + j] = *((uint8_t *)&x[i] + j);
	}
}

/* Standart chacha_expand with chacha_hash_i*/
void chacha_expand_i(uint8_t k[32], uint8_t n[16], uint8_t seq[64])
{
	int i;

	
	// constant constant constant constant
	// key      key      key      key
	// key      key      key      key
	// input    input    input    input
	
	// "expand 32-byte k"
	seq[0] = 101;
	seq[1] = 120;
	seq[2] = 112;
	seq[3] = 97;
	seq[4] = 110;
	seq[5] = 100;
	seq[6] = 32;
	seq[7] = 51;
	seq[8] = 50;
	seq[9] = 45;
	seq[10] = 98;
	seq[11] = 121;
	seq[12] = 116;
	seq[13] = 101;
	seq[14] = 32;
	seq[15] = 107;
	for (i = 0; i < 32; i++)
		seq[i + 16] = k[i];
	for (i = 0; i < 16; i++)
		seq[i + 48] = n[i];
	chacha_hash_i(seq);
}

void chacha_expand_triple(uint8_t k[32], uint8_t n[16],
						 uint8_t seq[192])
{
	int i, j;

	
	// constant constant constant constant
	// key      key      key      key
	// key      key      key      key
	// input    input    input    input
	
	for (j = 0; j < 3; j++) {
		// "expand 32-byte k"
		seq[j * 64 + 0] = 101;
		seq[j * 64 + 1] = 120;
		seq[j * 64 + 2] = 112;
		seq[j * 64 + 3] = 97;
		seq[j * 64 + 4] = 110;
		seq[j * 64 + 5] = 100;
		seq[j * 64 + 6] = 32;
		seq[j * 64 + 7] = 51;
		seq[j * 64 + 8] = 50;
		seq[j * 64 + 9] = 45;
		seq[j * 64 + 10] = 98;
		seq[j * 64 + 11] = 121;
		seq[j * 64 + 12] = 116;
		seq[j * 64 + 13] = 101;
		seq[j * 64 + 14] = 32;
		seq[j * 64 + 15] = 107;
		for (i = 0; i < 32; i++)
			seq[j * 64 + i + 16] = k[i];
		for (i = 0; i < 16; i++)
			seq[j * 64 + i + 48] = n[i];
		(*(uint64_t *)n)++; // block_counter++
	}
	chacha_hash_triple(seq);
}

void chacha_encrypt_i(uint8_t key[32], uint8_t nonce[8], uint8_t* m, 
					size_t size, uint8_t* c)
{
	uint8_t out[64], out_triple[192];
	uint8_t n[16];
	uint8_t number[8] = {0};
	int i, j, k, t;

	for (i = 0; i < 8; i++) {
		n[i] = nonce[i];
		n[i + 8] = number[i];
	}

	for (t = 0; t < size / 64 / 3; t++) {
		chacha_expand_triple(key, n, out_triple);
		for (k = 0; k < 64 * 3; k++)
			c[t * 64 * 3 + k] =
			m[t * 64 * 3 + k] ^ out_triple[k];
	}
	if (j = size % (64 * 3))
		for (i = 0; i < j / 64; i++) {
			chacha_expand_i(key, n, out);
			for (k = 0; k < 64; k++)
				c[t * 64 * 3 + i * 64 + k] =
				m[t * 64 * 3 + i * 64 + k] ^ out[k];
			(*(uint64_t *)n)++; // block_counter++
		}
	if (j = size % 64) {
		chacha_expand_i(key, n, out);
		for (k = 0; k < j; k++);
			c[t * 64 * 3 + i * 64 + k] =
			m[t * 64 * 3 + i * 64 + k] ^ out[k];
	}
}

void chacha_decrypt_i(uint8_t key[32], uint8_t nonce[8], uint8_t* c,
					 size_t size, uint8_t* m)
{
	chacha_encrypt_i(key, nonce, c, size, m);
}
