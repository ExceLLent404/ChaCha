#include <stdint.h>
#include <stddef.h>

void chacha_quarterround(uint32_t *a, uint32_t *b,
			 uint32_t *c, uint32_t *d)
{
	*a += *b;
	*d ^= *a;
	*d = *d << 16 | *d >> (32 - 16);
	*c += *d;
	*b ^= *c;
	*b = *b << 12 | *b >> (32 - 12);
	*a += *b;
	*d ^= *a;
	*d = *d << 8 | *d >> (32 - 8);
	*c += *d;
	*b ^= *c;
	*b = *b << 7 | *b >> (32 - 7);
}

void chacha_diagonalround(uint32_t y[16])
{
	chacha_quarterround(&y[0], &y[5], &y[10], &y[15]);
	chacha_quarterround(&y[1], &y[6], &y[11], &y[12]);
	chacha_quarterround(&y[2], &y[7], &y[8], &y[13]);
	chacha_quarterround(&y[3], &y[4], &y[9], &y[14]);
}

void chacha_columnround(uint32_t x[16])
{
	chacha_quarterround(&x[0], &x[4], &x[8], &x[12]);
	chacha_quarterround(&x[1], &x[5], &x[9], &x[13]);
	chacha_quarterround(&x[2], &x[6], &x[10], &x[14]);
	chacha_quarterround(&x[3], &x[7], &x[11], &x[15]);
}

void chacha_doubleround(uint32_t x[16])
{
	chacha_columnround(x);
	chacha_diagonalround(x);
}

void chacha_hash(uint8_t seq[64])
{
	int i;
	uint32_t x[16], z[16];

	for (i = 0; i < 16; i++)
		x[i] = z[i] = *(uint32_t *)(seq + i * 4);
	for (i = 0; i < 10; i++)
		chacha_doubleround(z);
	for (i = 0; i < 16; i++) {
		x[i] += z[i];
		*(uint32_t *) (seq + i * 4) = x[i];
	}
}

void chacha_expand(uint8_t k[32], uint8_t n[16], uint8_t seq[64])
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
	chacha_hash(seq);
}

void chacha_encrypt(uint8_t key[32], uint8_t nonce[8], uint8_t* m,
					size_t size, uint8_t* c)
{
	uint8_t out[64];
	uint8_t n[16];
	uint64_t* block_counter = (uint64_t *)n;
	int i, j, k;

	for (i = 0; i < 8; i++) {
		n[i] = 0;
		n[i + 8] = nonce[i];
	}

	for (i = 0; i < size / 64 ; i++) {
		chacha_expand(key, n, out);
		for (k = 0; k < 64; k++)
			c[i * 64 + k] = m[i * 64 + k] ^ out[k];
		(*block_counter)++;
	}
	j = size % 64;
	if (j) {
		chacha_expand(key, n, out);
		for (k = 0; k < j; k++)
			c[i * 64 + k] = m[i * 64 + k] ^ out[k];
	}
}

void chacha_decrypt(uint8_t key[32], uint8_t nonce[8], uint8_t* c,
					 size_t size, uint8_t* m)
{
	chacha_encrypt(key, nonce, c, size, m);
}
