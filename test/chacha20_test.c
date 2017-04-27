#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define SIZE_OF_DATA 64
#define LOAD_NUMBER 1000

int chacha_qr_check()
{
	int i;

	/* ----------------------- test #1 ------------------------ */
	uint32_t qr_initial1[4] = {
		0x11111111, 0x01020304, 0x9b8d6f43, 0x01234567
	};
	uint32_t qr_expectation1[4] = {
		0xea2a92f4, 0xcb1cf8ce, 0x4581472e, 0x5881c4bb
	};
	chacha_quarterround(&qr_initial1[0], &qr_initial1[1],
				&qr_initial1[2], &qr_initial1[3]);
	for (i = 0; i < 4; i++)
		if (qr_initial1[i] != qr_expectation1[i])
			return 1;
	/* ----------------------- test #2 ------------------------ */
	uint32_t qr_initial2[4] = {
		0x516461b1, 0x2a5f714c, 0x53372767, 0x3d631689
	};
	uint32_t qr_expectation2[4] = {
		0xbdb886dc, 0xcfacafd2, 0xe46bea80, 0xccc07c79
	};
	chacha_quarterround(&qr_initial2[0], &qr_initial2[1],
				&qr_initial2[2], &qr_initial2[3]);
	for (i = 0; i < 4; i++)
		if (qr_initial2[i] != qr_expectation2[i])
			return 1;
	/* -------------------------------------------------------- */
	return 0;
}

int chacha_dgr_check()
{
	return 0;
}

int chacha_cr_check()
{
	return 0;
}

int chacha_dr_check()
{
	return 0;
}

int chacha_hash_check()
{
	return 0;
}

int chacha_expand_check()
{
	int i, j;

	/* ----------------------- test #1 ------------------------ */
	uint8_t k[32] = {
		0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
		0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
		0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
		0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
	};
	uint8_t n[16] = {
		0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09,
		0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x00
	};
	uint8_t expand_initial[64];
	uint32_t expand_expectation32[16] = {
		0xe4e7f110, 0x15593bd1, 0x1fdd0f50, 0xc47120a3,
		0xc7f4d1c7, 0x0368c033, 0x9aaa2204, 0x4e6cd4c3,
		0x466482d2, 0x09aa9f07, 0x05d7c214, 0xa2028bd9,
		0xd19c12b5, 0xb94e16de, 0xe883d0cb, 0x4e3c50a2
	};
	uint8_t expand_expectation[64];
	for (i = 0; i < 16; i++)
		for (j = 0; j < 4; j++)
			expand_expectation[i * 4 + j] =
			*((uint8_t *)&expand_expectation32[i] + j);
	chacha_expand(k, n, expand_initial);
	for (i = 0; i < 64; i++)
		if (expand_initial[i] != expand_expectation[i])
			return 1;
	/* -------------------------------------------------------- */
	return 0;
}

void chacha_check()
{
	if(chacha_qr_check()) {
		printf("chacha_quarterround is not correct\n");
		return;
	}
	if(chacha_dgr_check()) {
		printf("chacha_diagonalround is not correct\n");
		return;
	}
	if(chacha_cr_check()) {
		printf("chacha_columnround is not correct\n");
		return;
	}
	if(chacha_dr_check()) {
		printf("chacha_doubleround is not correct\n");
		return;
	}
	if(chacha_hash_check()) {
		printf("chacha_hash is not correct\n");
		return;
	}
	if(chacha_expand_check()) {
		printf("chacha_expand is not correct\n");
		return;
	}
	
	printf("chacha is correct\n");
}

void chacha_test()
{
	uint8_t message[SIZE_OF_DATA] = {
		101, 120, 112, 97, 1, 2, 3, 4,
			5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 110, 100, 32, 49,
			101, 102, 103, 104, 105, 106, 107, 108,
		109, 110, 111, 112, 113, 114, 115, 116,
			54, 45, 98, 121, 1, 2, 3, 4,
		5, 6, 7, 8, 9, 10, 11, 12,
			13, 14, 15, 16, 116, 101, 32, 107
	};
	uint8_t key[32] = {
		1, 2, 3, 4, 5, 6, 7, 8,
		9, 10, 11, 12, 13, 14, 15, 16,
		201, 202, 203, 204, 205, 206, 207, 208,
		209, 210, 211, 212, 213, 214, 215, 216};
	uint8_t nonce[8] = {0};
	uint8_t cipher[SIZE_OF_DATA], plaintext[SIZE_OF_DATA];
	clock_t c;
	double t;
	int i, j, k, l;

	c = clock();
	for (l = 0; l < LOAD_NUMBER; l++)
	for (k = 0; k < SIZE_OF_DATA; k++)
	for (i = 0; i < 256; i++) {
		key[k / 2]++;
		message[k]++;
		chacha_encrypt(key, nonce, message, SIZE_OF_DATA, cipher);
		chacha_decrypt(key, nonce, cipher, SIZE_OF_DATA, plaintext);
		for (j = 0; j < SIZE_OF_DATA; j++)
			if (message[j] != plaintext[j]) {
				printf("error in test #%d on %d-th place"
					 "of chacha_test:\n", i, k);
				printf("original message:\n");
				printbytes(message, SIZE_OF_DATA);
				printf("\n");
				printf("cipher message:\n");
				printbytes(cipher, SIZE_OF_DATA);
				printf("\n");
				printf("plaintext message:\n");
				printbytes(plaintext, SIZE_OF_DATA);
				printf("\n");
				break;
			}
	}
	t = ((double)clock() - c) / CLOCKS_PER_SEC;
	printf("chacha_test time - %f seconds\n", t);
}
