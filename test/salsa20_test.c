#include <stdio.h>
#include <stdint.h>
#include <time.h>

#define SIZE_OF_DATA 64
#define LOAD_NUMBER 1000

int salsa_qr_check()
{
	int i;

	/* ----------------------- test #1 ------------------------ */
	uint32_t qr_initial1[4] = {
		0x00000000, 0x00000000, 0x00000000, 0x00000000
	};
	uint32_t qr_expectation1[4] = {
		0x00000000, 0x00000000, 0x00000000, 0x00000000
	};
	salsa_quarterround(&qr_initial1[0], &qr_initial1[1],
				&qr_initial1[2], &qr_initial1[3]);
	for (i = 0; i < 4; i++)
		if (qr_initial1[i] != qr_expectation1[i])
			return 1;
	/* ----------------------- test #2 ------------------------ */
	uint32_t qr_initial2[4] = {
		0x00000001, 0x00000000, 0x00000000, 0x00000000
	};
	uint32_t qr_expectation2[4] = {
		0x08008145, 0x00000080, 0x00010200, 0x20500000
	};
	salsa_quarterround(&qr_initial2[0], &qr_initial2[1],
				&qr_initial2[2], &qr_initial2[3]);
	for (i = 0; i < 4; i++)
		if (qr_initial2[i] != qr_expectation2[i])
			return 1;
	/* ----------------------- test #3 ------------------------ */
	uint32_t qr_initial3[4] = {
		0x00000000, 0x00000001, 0x00000000, 0x00000000
	};
	uint32_t qr_expectation3[4] = {
		0x88000100, 0x00000001, 0x00000200, 0x00402000
	};
	salsa_quarterround(&qr_initial3[0], &qr_initial3[1],
				&qr_initial3[2], &qr_initial3[3]);
	for (i = 0; i < 4; i++)
		if (qr_initial3[i] != qr_expectation3[i])
			return 1;
	/* ----------------------- test #4 ------------------------ */
	uint32_t qr_initial4[4] = {
		0x00000000, 0x00000000, 0x00000001, 0x00000000
	};
	uint32_t qr_expectation4[4] = {
		0x80040000, 0x00000000, 0x00000001, 0x00002000
	};
	salsa_quarterround(&qr_initial4[0], &qr_initial4[1],
				&qr_initial4[2], &qr_initial4[3]);
	for (i = 0; i < 4; i++)
		if (qr_initial4[i] != qr_expectation4[i])
			return 1;
	/* ----------------------- test #5 ------------------------ */
	uint32_t qr_initial5[4] = {
		0x00000000, 0x00000000, 0x00000000, 0x00000001
	};
	uint32_t qr_expectation5[4] = {
		0x00048044, 0x00000080, 0x00010000, 0x20100001
	};
	salsa_quarterround(&qr_initial5[0], &qr_initial5[1],
				&qr_initial5[2], &qr_initial5[3]);
	for (i = 0; i < 4; i++)
		if (qr_initial5[i] != qr_expectation5[i])
			return 1;
	/* ----------------------- test #6 ------------------------ */
	uint32_t qr_initial6[4] = {
		0xe7e8c006, 0xc4f9417d, 0x6479b4b2, 0x68c67137
	};
	uint32_t qr_expectation6[4] = {
		0xe876d72b, 0x9361dfd5, 0xf1460244, 0x948541a3
	};
	salsa_quarterround(&qr_initial6[0], &qr_initial6[1],
				&qr_initial6[2], &qr_initial6[3]);
	for (i = 0; i < 4; i++)
		if (qr_initial6[i] != qr_expectation6[i])
			return 1;
	/* ----------------------- test #7 ------------------------ */
	uint32_t qr_initial7[4] = {
		0xd3917c5b, 0x55f1c407, 0x52a58a7a, 0x8f887a3b
	};
	uint32_t qr_expectation8[4] = {
		0x3e2f308c, 0xd90a8f36, 0x6ab2a923, 0x2883524c
	};
	salsa_quarterround(&qr_initial7[0], &qr_initial7[1],
				&qr_initial7[2], &qr_initial7[3]);
	for (i = 0; i < 4; i++)
		if (qr_initial7[i] != qr_expectation8[i])
			return 1;
	/* -------------------------------------------------------- */
	return 0;
}

int salsa_rr_check()
{
	int i;

	/* ----------------------- test #1 ------------------------ */
	uint32_t rr_initial1[16] = {
		0x00000001, 0x00000000, 0x00000000, 0x00000000,
		0x00000001, 0x00000000, 0x00000000, 0x00000000,
		0x00000001, 0x00000000, 0x00000000, 0x00000000,
		0x00000001, 0x00000000, 0x00000000, 0x00000000
	};
	uint32_t rr_expectation1[16] = {
		0x08008145, 0x00000080, 0x00010200, 0x20500000,
		0x20100001, 0x00048044, 0x00000080, 0x00010000,
		0x00000001, 0x00002000, 0x80040000, 0x00000000,
		0x00000001, 0x00000200, 0x00402000, 0x88000100
	};
	salsa_rowround(rr_initial1);
	for (i = 0; i < 16; i++)
		if (rr_initial1[i] != rr_expectation1[i])
			return 1;
	/* ----------------------- test #2 ------------------------ */
	uint32_t rr_initial2[16] = {
		0x08521bd6, 0x1fe88837, 0xbb2aa576, 0x3aa26365,
		0xc54c6a5b, 0x2fc74c2f, 0x6dd39cc3, 0xda0a64f6,
		0x90a2f23d, 0x067f95a6, 0x06b35f61, 0x41e4732e,
		0xe859c100, 0xea4d84b7, 0x0f619bff, 0xbc6e965a
	};
	uint32_t rr_expectation2[16] = {
		0xa890d39d, 0x65d71596, 0xe9487daa, 0xc8ca6a86,
		0x949d2192, 0x764b7754, 0xe408d9b9, 0x7a41b4d1,
		0x3402e183, 0x3c3af432, 0x50669f96, 0xd89ef0a8,
		0x0040ede5, 0xb545fbce, 0xd257ed4f, 0x1818882d
	};
	salsa_rowround(rr_initial2);
	for (i = 0; i < 16; i++)
		if (rr_initial2[i] != rr_expectation2[i])
			return 1;
	/* -------------------------------------------------------- */
	return 0;
}

int salsa_cr_check()
{
	int i;

	/* ----------------------- test #1 ------------------------ */
	uint32_t cr_initial1[16] = {
		0x00000001, 0x00000000, 0x00000000, 0x00000000,
		0x00000001, 0x00000000, 0x00000000, 0x00000000,
		0x00000001, 0x00000000, 0x00000000, 0x00000000,
		0x00000001, 0x00000000, 0x00000000, 0x00000000
	};
	uint32_t cr_expectation1[16] = {
		0x10090288, 0x00000000, 0x00000000, 0x00000000,
		0x00000101, 0x00000000, 0x00000000, 0x00000000,
		0x00020401, 0x00000000, 0x00000000, 0x00000000,
		0x40a04001, 0x00000000, 0x00000000, 0x00000000
	};
	salsa_columnround(cr_initial1);
	for (i = 0; i < 16; i++)
		if (cr_initial1[i] != cr_expectation1[i])
			return 1;
	/* ----------------------- test #2 ------------------------ */
	uint32_t cr_initial2[16] = {
		0x08521bd6, 0x1fe88837, 0xbb2aa576, 0x3aa26365,
		0xc54c6a5b, 0x2fc74c2f, 0x6dd39cc3, 0xda0a64f6,
		0x90a2f23d, 0x067f95a6, 0x06b35f61, 0x41e4732e,
		0xe859c100, 0xea4d84b7, 0x0f619bff, 0xbc6e965a
	};
	uint32_t cr_expectation2[16] = {
		0x8c9d190a, 0xce8e4c90, 0x1ef8e9d3, 0x1326a71a,
		0x90a20123, 0xead3c4f3, 0x63a091a0, 0xf0708d69,
		0x789b010c, 0xd195a681, 0xeb7d5504, 0xa774135c,
		0x481c2027, 0x53a8e4b5, 0x4c1f89c5, 0x3f78c9c8
	};
	salsa_columnround(cr_initial2);
	for (i = 0; i < 16; i++)
		if (cr_initial2[i] != cr_expectation2[i])
			return 1;
	/* -------------------------------------------------------- */
	return 0;
}

int salsa_dr_check()
{
	int i;

	/* ----------------------- test #1 ------------------------ */
	uint32_t dr_initial1[16] = {
		0x00000001, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000,
		0x00000000, 0x00000000, 0x00000000, 0x00000000
	};
	uint32_t dr_expectation1[16] = {
		0x8186a22d, 0x0040a284, 0x82479210, 0x06929051,
		0x08000090, 0x02402200, 0x00004000, 0x00800000,
		0x00010200, 0x20400000, 0x08008104, 0x00000000,
		0x20500000, 0xa0000040, 0x0008180a, 0x612a8020
	};
	salsa_doubleround(dr_initial1);
	for (i = 0; i < 16; i++)
		if (dr_initial1[i] != dr_expectation1[i])
			return 1;
	/* ----------------------- test #2 ------------------------ */
	uint32_t dr_initial2[16] = {
		0xde501066, 0x6f9eb8f7, 0xe4fbbd9b, 0x454e3f57,
		0xb75540d3, 0x43e93a4c, 0x3a6f2aa0, 0x726d6b36,
		0x9243f484, 0x9145d1e8, 0x4fa9d247, 0xdc8dee11,
		0x054bf545, 0x254dd653, 0xd9421b6d, 0x67b276c1
	};
	uint32_t dr_expectation2[16] = {
		0xccaaf672, 0x23d960f7, 0x9153e63a, 0xcd9a60d0,
		0x50440492, 0xf07cad19, 0xae344aa0, 0xdf4cfdfc,
		0xca531c29, 0x8e7943db, 0xac1680cd, 0xd503ca00,
		0xa74b2ad6, 0xbc331c5c, 0x1dda24c7, 0xee928277
	};
	salsa_doubleround(dr_initial2);
	for (i = 0; i < 16; i++)
		if (dr_initial2[i] != dr_expectation2[i])
			return 1;
	/* -------------------------------------------------------- */
	return 0;
}

int salsa_hash_check()
{
	int i;

	/* ----------------------- test #1 ------------------------ */
	uint8_t hash_initial1[64] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	uint8_t hash_expectation1[64] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	};
	salsa_hash(hash_initial1);
	for (i = 0; i < 64; i++)
		if (hash_initial1[i] != hash_expectation1[i])
			return 1;
	/* ----------------------- test #2 ------------------------ */
	uint8_t hash_initial2[64] = {
		211, 159, 13, 115, 76, 55, 82, 183,
			3, 117, 222, 37, 191, 187, 234, 136,
		49, 237, 179, 48, 1, 106, 178, 219, 
			175, 199, 166, 48, 86, 16, 179, 207,
		31, 240, 32, 63, 15, 83, 93, 161,
			116, 147, 48, 113, 238, 55, 204, 36,
		79, 201, 235, 79, 3, 81, 156, 47,
			203, 26, 244, 243, 88, 118, 104, 54
	};
	uint8_t hash_expectation2[64] = {
		109, 42, 178, 168, 156, 240, 248, 238,
			168, 196, 190, 203, 26, 110, 170, 154,
		29, 29, 150, 26, 150, 30, 235, 249,
			190, 163, 251, 48, 69, 144, 51, 57,
		118, 40, 152, 157, 180, 57, 27, 94,
			107, 42, 236, 35, 27, 111, 114, 114,
		219, 236, 232, 135, 111, 155, 110, 18,
			24, 232, 95, 158, 179, 19, 48, 202
	};
	salsa_hash(hash_initial2);
	for (i = 0; i < 64; i++)
		if (hash_initial2[i] != hash_expectation2[i])
			return 1;
	/* ----------------------- test #3 ------------------------ */
	uint8_t hash_initial3[64] = {
		88, 118, 104, 54, 79, 201, 235, 79,
			3, 81, 156, 47, 203, 26, 244, 243,
		191, 187, 234, 136, 211, 159, 13, 115,
			76, 55, 82, 183, 3, 117, 222, 37,
		86, 16, 179, 207, 49, 237, 179, 48,
			1, 106, 178, 219, 175, 199, 166, 48,
		238, 55, 204, 36, 31, 240, 32, 63,
			15, 83, 93, 161, 116, 147, 48, 113
	};
	uint8_t hash_expectation3[64] = {
		179, 19, 48, 202, 219, 236, 232, 135,
			111, 155, 110, 18, 24, 232, 95, 158,
		26, 110, 170, 154, 109, 42, 178, 168,
			156, 240, 248, 238, 168, 196, 190, 203,
		69, 144, 51, 57, 29, 29, 150, 26,
			150, 30, 235, 249, 190, 163, 251, 48,
		27, 111, 114, 114, 118, 40, 152, 157,
			180, 57, 27, 94, 107, 42, 236, 35
	};
	salsa_hash(hash_initial3);
	for (i = 0; i < 64; i++)
		if (hash_initial3[i] != hash_expectation3[i])
			return 1;
	/* ----------------------- test #4 ------------------------ */
	uint8_t hash_initial4[64] = {
		6, 124, 83, 146, 38, 191, 9, 50,
			4, 161, 47, 222, 122, 182, 223, 185,
		75, 27, 0, 216, 16, 122, 7, 89,
			162, 104, 101, 147, 213, 21, 54, 95,
		225, 253, 139, 176, 105, 132, 23, 116,
			76, 41, 176, 207, 221, 34, 157, 108,
		94, 94, 99, 52, 90, 117, 91, 220,
			146, 190, 239, 143, 196, 176, 130, 186
	};
	uint8_t hash_expectation4[64] = {
		8, 18, 38, 199, 119, 76, 215, 67,
			173, 127, 144, 162, 103, 212, 176, 217, 
		192, 19, 233, 33, 159, 197, 154, 160,
			128, 243, 219, 65, 171, 136, 135, 225, 
		123, 11, 68, 86, 237, 82, 20, 155,
			133, 189, 9, 83, 167, 116, 194, 78, 
		122, 127, 195, 185, 185, 204, 188, 90,
			245, 9, 183, 248, 226, 85, 245, 104
	};
	for (i = 0; i < 1000000; i++)
		salsa_hash(hash_initial4);
	for (i = 0; i < 64; i++)
		if (hash_initial4[i] != hash_expectation4[i])
			return 1;
	/* -------------------------------------------------------- */
	return 0;
}

int salsa_expand_check()
{
	int i;

	/* ----------------------- test #1 ------------------------ */
	uint8_t k[32] = {
		1, 2, 3, 4, 5, 6, 7, 8,
		9, 10, 11, 12, 13, 14, 15, 16,
		201, 202, 203, 204, 205, 206, 207, 208,
		209, 210, 211, 212, 213, 214, 215, 216
	};
	uint8_t n[16] = {
		101, 102, 103, 104, 105, 106, 107, 108,
		109, 110, 111, 112, 113, 114, 115, 116
	};
	uint8_t expand_initial[64];
	uint8_t expand_expectation[64] = {
		69, 37, 68, 39, 41, 15, 107, 193,
			255, 139, 122, 6, 170, 233, 217, 98, 
		89, 144, 182, 106, 21, 51, 200, 65,
			239, 49, 222, 34, 215, 114, 40, 126, 
		104, 197, 7, 225, 197, 153, 31, 2,
			102, 78, 76, 176, 84, 245, 246, 184, 
		177, 160, 133, 130, 6, 72, 149, 119,
			192, 195, 132, 236, 234, 103, 246, 74
	};
	salsa_expand(k, n, expand_initial);
	for (i = 0; i < 64; i++)
		if (expand_initial[i] != expand_expectation[i])
			return 1;
	/* -------------------------------------------------------- */
	return 0;
}

void salsa_check()
{
	if(salsa_qr_check()) {
		printf("salsa_quarterround is not correct\n");
		return;
	}
	if(salsa_rr_check()) {
		printf("salsa_rowround is not correct\n");
		return;
	}
	if(salsa_cr_check()) {
		printf("salsa_columnround is not correct\n");
		return;
	}
	if(salsa_dr_check()) {
		printf("salsa_doubleround is not correct\n");
		return;
	}
	if(salsa_hash_check()) {
		printf("salsa_hash is not correct\n");
		return;
	}
	if(salsa_expand_check()) {
		printf("salsa_expand is not correct\n");
		return;
	}
	
	printf("salsa is correct\n");
}

void salsa_test()
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
		209, 210, 211, 212, 213, 214, 215, 216
	};
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
		salsa_encrypt(key, nonce, message, SIZE_OF_DATA, cipher);
		salsa_decrypt(key, nonce, cipher, SIZE_OF_DATA, plaintext);
		for (j = 0; j < SIZE_OF_DATA; j++)
			if (message[j] != plaintext[j]) {
				printf("error in test #%d on %d-th place"
					 "of salsa_test:\n", i, k);
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
	printf("salsa_test time - %f seconds\n", t);
}
