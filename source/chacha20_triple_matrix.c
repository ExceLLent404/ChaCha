#include <stdint.h>
#include <stddef.h>

/* Loop version */
void chacha_doubleround_asm_triple(uint32_t y[48]) {
	asm volatile(
		"vldm.32 %[y]!, {q0-q7}\n\t"
		"vldm.32 %[y], {q8-q11}\n\t"
		"sub %[y], #128\n\t"
		/* Now q0-q3 (q4-q7 and q8-q11 too) are:
		*   y[0],   y[4],  y[8],   y[12]
		*   y[1],   y[5],  y[9],   y[13]
		*   y[2],   y[6],  y[10],  y[14]
		*   y[3],   y[7],  y[11],  y[15]
		*  q0 = a, q1 = b  q2 = c, q3 = d
		*/

		"mov r0, #10\n\t" // loop counter

		"loop_start:\n\t"
		"subs r0, #1\n\t"
		"bmi loop_end\n\t"

		/* a += b; */
		"vadd.i32 q0, q1\n\t"
		"vadd.i32 q4, q5\n\t"
		"vadd.i32 q8, q9\n\t"

		/* e = d ^ a; */
		"veor q12, q3, q0\n\t"
		"veor q13, q7, q4\n\t"
		"veor q14, q11, q8\n\t"

		/* d = e <<< 16; */
		"vshl.i32 q3, q12, #16\n\t"
		"vshl.i32 q7, q13, #16\n\t"
		"vshl.i32 q11, q14, #16\n\t"
		"vsri.32 q3, q12, #16\n\t"
		"vsri.32 q7, q13, #16\n\t"
		"vsri.32 q11, q14, #16\n\t"

		/* c += d; */
		"vadd.i32 q2, q3\n\t"
		"vadd.i32 q6, q7\n\t"
		"vadd.i32 q10, q11\n\t"

		/* e = b ^ c; */
		"veor q12, q1, q2\n\t"
		"veor q13, q5, q6\n\t"
		"veor q14, q9, q10\n\t"

		/* b = e <<< 12; */
		"vshl.i32 q1, q12, #12\n\t"
		"vshl.i32 q5, q13, #12\n\t"
		"vshl.i32 q9, q14, #12\n\t"
		"vsri.32 q1, q12, #20\n\t"
		"vsri.32 q5, q13, #20\n\t"
		"vsri.32 q9, q14, #20\n\t"

		/* a += b; */
		"vadd.i32 q0, q1\n\t"
		"vadd.i32 q4, q5\n\t"
		"vadd.i32 q8, q9\n\t"

		/* e = d ^ a; */
		"veor q12, q3, q0\n\t"
		"veor q13, q7, q4\n\t"
		"veor q14, q11, q8\n\t"

		/* d = e <<< 8; */
		"vshl.i32 q3, q12, #8\n\t"
		"vshl.i32 q7, q13, #8\n\t"
		"vshl.i32 q11, q14, #8\n\t"
		"vsri.32 q3, q12, #24\n\t"
		"vsri.32 q7, q13, #24\n\t"
		"vsri.32 q11, q14, #24\n\t"

		/* c += d; */
		"vadd.i32 q2, q3\n\t"
		"vadd.i32 q6, q7\n\t"
		"vadd.i32 q10, q11\n\t"

		/* e = b ^ c; */
		"veor q12, q1, q2\n\t"
		"veor q13, q5, q6\n\t"
		"veor q14, q9, q10\n\t"

		/* b = e <<< 7; */
		"vshl.i32 q1, q12, #7\n\t"
		"vshl.i32 q5, q13, #7\n\t"
		"vshl.i32 q9, q14, #7\n\t"
		"vsri.32 q1, q12, #25\n\t"
		"vsri.32 q5, q13, #25\n\t"
		"vsri.32 q9, q14, #25\n\t"

		/* Diagonalround data allocation:
		   Current state:
		*   y[0],   y[4],  y[8],   y[12]
		*   y[1],   y[5],  y[9],   y[13]
		*   y[2],   y[6],  y[10],  y[14]
		*   y[3],   y[7],  y[11],  y[15]
		   Required state:
		*   y[0],   y[5],  y[10],  y[15]
		*   y[1],   y[6],  y[11],  y[12]
		*   y[2],   y[7],  y[8],   y[13]
		*   y[3],   y[4],  y[9],   y[14]
		*  q0 = a, q1 = b  q2 = c, q3 = d
		*/
		"vext.32 q1, q1, q1, #1\n\t"
		"vext.32 q5, q5, q5, #1\n\t"
		"vext.32 q9, q9, q9, #1\n\t"
		"vext.32 q2, q2, q2, #2\n\t"
		"vext.32 q6, q6, q6, #2\n\t"
		"vext.32 q10, q10, q10, #2\n\t"
		"vext.32 q3, q3, q3, #3\n\t"
		"vext.32 q7, q7, q7, #3\n\t"
		"vext.32 q11, q11, q11, #3\n\t"

		/* a += b; */
		"vadd.i32 q0, q1\n\t"
		"vadd.i32 q4, q5\n\t"
		"vadd.i32 q8, q9\n\t"

		/* e = d ^ a; */
		"veor q12, q3, q0\n\t"
		"veor q13, q7, q4\n\t"
		"veor q14, q11, q8\n\t"

		/* d = e <<< 16; */
		"vshl.i32 q3, q12, #16\n\t"
		"vshl.i32 q7, q13, #16\n\t"
		"vshl.i32 q11, q14, #16\n\t"
		"vsri.32 q3, q12, #16\n\t"
		"vsri.32 q7, q13, #16\n\t"
		"vsri.32 q11, q14, #16\n\t"

		/* c += d; */
		"vadd.i32 q2, q3\n\t"
		"vadd.i32 q6, q7\n\t"
		"vadd.i32 q10, q11\n\t"

		/* e = b ^ c; */
		"veor q12, q1, q2\n\t"
		"veor q13, q5, q6\n\t"
		"veor q14, q9, q10\n\t"

		/* b = e <<< 12; */
		"vshl.i32 q1, q12, #12\n\t"
		"vshl.i32 q5, q13, #12\n\t"
		"vshl.i32 q9, q14, #12\n\t"
		"vsri.32 q1, q12, #20\n\t"
		"vsri.32 q5, q13, #20\n\t"
		"vsri.32 q9, q14, #20\n\t"

		/* a += b; */
		"vadd.i32 q0, q1\n\t"
		"vadd.i32 q4, q5\n\t"
		"vadd.i32 q8, q9\n\t"

		/* e = d ^ a; */
		"veor q12, q3, q0\n\t"
		"veor q13, q7, q4\n\t"
		"veor q14, q11, q8\n\t"

		/* d = e <<< 8; */
		"vshl.i32 q3, q12, #8\n\t"
		"vshl.i32 q7, q13, #8\n\t"
		"vshl.i32 q11, q14, #8\n\t"
		"vsri.32 q3, q12, #24\n\t"
		"vsri.32 q7, q13, #24\n\t"
		"vsri.32 q11, q14, #24\n\t"

		/* c += d; */
		"vadd.i32 q2, q3\n\t"
		"vadd.i32 q6, q7\n\t"
		"vadd.i32 q10, q11\n\t"

		/* e = b ^ c; */
		"veor q12, q1, q2\n\t"
		"veor q13, q5, q6\n\t"
		"veor q14, q9, q10\n\t"

		/* b = e <<< 7; */
		"vshl.i32 q1, q12, #7\n\t"
		"vshl.i32 q5, q13, #7\n\t"
		"vshl.i32 q9, q14, #7\n\t"
		"vsri.32 q1, q12, #25\n\t"
		"vsri.32 q5, q13, #25\n\t"
		"vsri.32 q9, q14, #25\n\t"

		/* Inverse data allocation */
		"vext.32 q1, q1, q1, #3\n\t"
		"vext.32 q5, q5, q5, #3\n\t"
		"vext.32 q9, q9, q9, #3\n\t"
		"vext.32 q2, q2, q2, #2\n\t"
		"vext.32 q6, q6, q6, #2\n\t"
		"vext.32 q10, q10, q10, #2\n\t"
		"vext.32 q3, q3, q3, #1\n\t"
		"vext.32 q7, q7, q7, #1\n\t"
		"vext.32 q11, q11, q11, #1\n\t"

		"b loop_start\n\t"

		"loop_end:\n\t"

		"vstm.32 %[y]!, {q0-q7}\n\t"
		"vstm.32 %[y], {q8-q11}\n\t"
		"sub %[y], #128\n\t"
		:
		: [y] "r" (y)
		: "r0", "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7",
		  "q8", "q9", "q10", "q11", "q12", "q13", "q14"
	);
}

void chacha_hash_triple(uint8_t seq[192])
{
	int i;
	uint32_t x[48], z[48];

	for (i = 0; i < 48; i++)
		x[i] = z[i] = *(uint32_t *)(seq + i * 4);
	chacha_doubleround_asm_triple(z); // with loop
	for (i = 0; i < 48; i++) {
		x[i] += z[i];
		*(uint32_t *) (seq + i * 4) = x[i];
	}
}

void chacha_expand_triple(uint8_t k[32], uint8_t n[16],
						 uint8_t seq[192])
{
	uint64_t* block_counter = (uint64_t *)n;
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
		(*block_counter)++;
	}
	chacha_hash_triple(seq);
}

void chacha_encrypt_i(uint8_t key[32], uint8_t nonce[8], uint8_t* m,
					size_t size, uint8_t* c)
{
	uint8_t out[64], out_triple[192];
	uint8_t n[16];
	uint64_t* block_counter = (uint64_t *)n;
	int i, j, k, t;

	for (i = 0; i < 8; i++) {
		n[i] = 0;
		n[i + 8] = nonce[i];
	}

	for (t = 0; t < size / 64 / 3; t++) {
		chacha_expand_triple(key, n, out_triple);
		for (k = 0; k < 64 * 3; k++)
			c[t * 64 * 3 + k] =
			m[t * 64 * 3 + k] ^ out_triple[k];
	}
	j = size % (64 * 3);
	if (j)
		for (i = 0; i < j / 64; i++) {
			chacha_expand(key, n, out);
			for (k = 0; k < 64; k++)
				c[t * 64 * 3 + i * 64 + k] =
				m[t * 64 * 3 + i * 64 + k] ^ out[k];
			(*block_counter)++;
		}
	j = size % 64;
	if (j) {
		chacha_expand(key, n, out);
		for (k = 0; k < j; k++)
			c[t * 64 * 3 + i * 64 + k] =
			m[t * 64 * 3 + i * 64 + k] ^ out[k];
	}
}

void chacha_decrypt_i(uint8_t key[32], uint8_t nonce[8], uint8_t* c,
					 size_t size, uint8_t* m)
{
	chacha_encrypt_i(key, nonce, c, size, m);
}
