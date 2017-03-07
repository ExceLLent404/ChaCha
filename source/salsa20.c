#include <stdint.h>
#include <stddef.h>
#include "../include/header.h"

void salsa_quarterround(uint32_t *y0, uint32_t *y1,
			 uint32_t *y2, uint32_t *y3)
{
	*y1 ^= ((*y0 + *y3) << 7 | (*y0 + *y3) >> 32 - 7);
	*y2 ^= ((*y1 + *y0) << 9 | (*y1 + *y0) >> 32 - 9);
	*y3 ^= ((*y2 + *y1) << 13 | (*y2 + *y1) >> 32 - 13);
	*y0 ^= ((*y3 + *y2) << 18 | (*y3 + *y2) >> 32 - 18);
}

void salsa_rowround(uint32_t y[16])
{
	salsa_quarterround(&y[0], &y[1], &y[2], &y[3]);
	salsa_quarterround(&y[5], &y[6], &y[7], &y[4]);
	salsa_quarterround(&y[10], &y[11], &y[8], &y[9]);
	salsa_quarterround(&y[15], &y[12], &y[13], &y[14]);
}

void salsa_columnround(uint32_t x[16])
{
	salsa_quarterround(&x[0], &x[4], &x[8], &x[12]);
	salsa_quarterround(&x[5], &x[9], &x[13], &x[1]);
	salsa_quarterround(&x[10], &x[14], &x[2], &x[6]);
	salsa_quarterround(&x[15], &x[3], &x[7], &x[11]);
}

void salsa_doubleround(uint32_t x[16])
{
	salsa_columnround(x);
	salsa_rowround(x);
}

void salsa_hash(uint8_t seq[64])
{
	int i;
	uint32_t x[16], z[16];

	for (i = 0; i < 16; i++) {
		x[i] = littleendian(seq[i * 4], seq[i * 4 + 1],
				 seq[i * 4 + 2], seq[i * 4 + 3]);
		z[i] = littleendian(seq[i * 4], seq[i * 4 + 1],
				 seq[i * 4 + 2], seq[i * 4 + 3]);
	}
	for (i = 0; i < 10; i++)
		salsa_doubleround(z);
	for (i = 0; i < 16; i++) {
		x[i] += z[i];
		wordtobyte(&seq[i * 4], x[i]);
	}
}

void salsa_expand(uint8_t k[32], uint8_t n[16], uint8_t seq[64])
{
	// 'o' is 'sigma'
	/*uint8_t o0[4] = {101, 120, 112, 97},
		o1[4] = {110, 100, 32, 51},
		o2[4] = {50, 45, 98, 121},
		o3[4] = {116, 101, 32, 107};*/
	int i;

	/*
	constant  key      key     key
	  key   constant  input   input
	 input   input   constant  key
	  key     key      key   constant
	*/
	// "expand 32-byte k"
	seq[0] = 101;
	seq[1] = 120;
	seq[2] = 112;
	seq[3] = 97;
	for (i = 0; i < 16; i++)
		seq[i + 4] = k[i];
	seq[20] = 110;
	seq[21] = 100;
	seq[22] = 32;
	seq[23] = 51;
	for (i = 0; i < 16; i++)
		seq[i + 24] = n[i];
	seq[40] = 50;
	seq[41] = 45;
	seq[42] = 98;
	seq[43] = 121;
	for (i = 0; i < 16; i++)
		seq[i + 44] = k[i + 16];
	seq[60] = 116;
	seq[61] = 101;
	seq[62] = 32;
	seq[63] = 107;
	salsa_hash(seq);
}

void salsa_encrypt(uint8_t key[32], uint8_t nonce[8], uint8_t* m, 
					size_t size, uint8_t* c)
{
	uint8_t out[64];
	uint8_t n[16];
	uint8_t number[8] = {0};
	int i, j, k;

	for (i = 0; i < 8; i++) {
		n[i] = nonce[i];
		n[i + 8] = number[i];
	}

	for (i = 0; i < size / 64 ; i++) {
		salsa_expand(key, n, out);
		for (k = 0; k < 64; k++)
			c[i * 64 + k] = m[i * 64 + k] ^ out[k];
		/* number++ */
		n[8]++;
		for (j = 0; j < 7; j++)
			if (!n[j + 8])
				n[j + 9]++;
			else
				break;
	}
	if (j = size % 64) {
		salsa_expand(key, n, out);
		for (k = 0; k < j; k++);
			c[i * 64 + k] = m[i * 64 + k] ^ out[k];
	}
}

void salsa_decrypt(uint8_t key[32], uint8_t nonce[8], uint8_t* c,
					 size_t size, uint8_t* m)
{
	salsa_encrypt(key, nonce, c, size, m);
}
