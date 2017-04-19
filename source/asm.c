#include <stdint.h>

void salsa_rowround_asm(uint32_t y[16])
{
	/* Required state:
	*   y[0],  y[1],   y[2],   y[3]
	*   y[5],  y[6],   y[7],   y[4]
	*   y[10], y[11],  y[8],   y[9]
	*   y[15], y[12],  y[13],  y[14]
	*  q0 = a, q1 = b  q2 = c, q3 = d
	*/
	asm(
		"vldm.32 %[y], {q0, q1, q2, q3}\n\t"
		/* Now q0-q3 are:
		*   y[0],   y[4],  y[8],   y[12]
		*   y[1],   y[5],  y[9],   y[13]
		*   y[2],   y[6],  y[10],  y[14]
		*   y[3],   y[7],  y[11],  y[15]
		*  q0 = a, q1 = b  q2 = c, q3 = d
		*/
		"vext.32 q1, q1, q1, #1\n\t"        // Shifting
		"vext.32 q2, q2, q2, #2\n\t"
		"vext.32 q3, q3, q3, #3\n\t"
		/* Now q0-q3 are:
		*   y[0],   y[5],  y[10],  y[15]
		*   y[1],   y[6],  y[11],  y[12]
		*   y[2],   y[7],  y[8],   y[13]
		*   y[3],   y[4],  y[9],   y[14]
		*  q0 = a, q1 = b  q2 = c, q3 = d
		*/
		"vtrn.32 q0, q1\n\t"                // Transpose
		"vtrn.32 q2, q3\n\t"
		"vswp.32 d1, d4\n\t"
		"vswp.32 d3, d6\n\t"
		/* Now q0-q3 are as we need */
		"vadd.i32 q4, q0, q3\n\t"           // a + d;
		"vshl.i32 q5, q4, #7\n\t"           // (a + d) <<< 7;
		"vsri.32 q5, q4, #25\n\t"
		"veor q1, q5\n\t"                   // b ^= ...;
		"vadd.i32 q4, q1, q0\n\t"           // b + a;
		"vshl.i32 q5, q4, #9\n\t"           // (b + a) <<< 9;
		"vsri.32 q5, q4, #23\n\t"
		"veor q2, q5\n\t"                   // c ^= ...;
		"vadd.i32 q4, q2, q1\n\t"           // c + b;
		"vshl.i32 q5, q4, #13\n\t"          // (c + b) <<< 13;
		"vsri.32 q5, q4, #19\n\t"
		"veor q3, q5\n\t"                   // d ^= ...;
		"vadd.i32 q4, q3, q2\n\t"           // d + c;
		"vshl.i32 q5, q4, #18\n\t"          // (d + c) <<< 18;
		"vsri.32 q5, q4, #14\n\t"
		"veor q0, q5\n\t"                   // a ^= ...;
		/* Inverse data allocation */
		"vswp.32 d1, d4\n\t"
		"vswp.32 d3, d6\n\t"
		"vtrn.32 q0, q1\n\t"
		"vtrn.32 q2, q3\n\t"
		"vext.32 q1, q1, q1, #3\n\t"
		"vext.32 q2, q2, q2, #2\n\t"
		"vext.32 q3, q3, q3, #1\n\t"
		"vstm.32 %[y], {q0, q1, q2, q3}\n\t" 
		:
		: [y] "r" (y)
		: "q0", "q1", "q2", "q3", "q4", "q5"
	);
}

void salsa_columnround_asm(uint32_t x[16])
{
	/* Required state:
	*   x[0],   x[4],  x[8],   x[12]
	*   x[5],   x[9],  x[13],  x[1]
	*   x[10],  x[14], x[2],   x[6]
	*   x[15],  x[3],  x[7],   x[11]
	*  q0 = a, q1 = b  q2 = c, q3 = d
	*/
	asm(
		"vld4.32 {q0, q1, q2, q3}, %[y]\n\t"
		/* Now q0-q3 are:
		*   y[0],   y[1],  y[2],   y[3]
		*   y[4],   y[5],  y[6],   y[7]
		*   y[8],   y[9],  y[10],  y[11]
		*   y[12],  y[13], y[14],  y[15]
		*  q0 = a, q1 = b  q2 = c, q3 = d
		*/
		"vext.32 q1, q1, q1, #1\n\t"        // Shifting
		"vext.32 q2, q2, q2, #2\n\t"
		"vext.32 q3, q3, q3, #3\n\t"
		/* Now q0-q3 are:
		*   y[0],   y[5],  y[10],  y[15]
		*   y[4],   y[9],  y[14],  y[3]
		*   y[8],   y[13], y[2],   y[7]
		*   y[12],  y[1],  y[6],   y[11]
		*  q0 = a, q1 = b  q2 = c, q3 = d
		*/
		"vswp.32 d1, d4\n\t"                // Transpose
		"vswp.32 d3, d6\n\t"
		"vtrn.32 q0, q1\n\t"
		"vtrn.32 q2, q3\n\t"
		/* Now q0-q3 are as we need */
		"vadd.i32 q4, q0, q3\n\t"           // a + d;
		"vshl.i32 q5, q4, #7\n\t"           // (a + d) <<< 7;
		"vsri.32 q5, q4, #25\n\t"
		"veor q1, q5\n\t"                   // b ^= ...;
		"vadd.i32 q4, q1, q0\n\t"           // b + a;
		"vshl.i32 q5, q4, #9\n\t"           // (b + a) <<< 9;
		"vsri.32 q5, q4, #23\n\t"
		"veor q2, q5\n\t"                   // c ^= ...;
		"vadd.i32 q4, q2, q1\n\t"           // c + b;
		"vshl.i32 q5, q4, #13\n\t"          // (c + b) <<< 13;
		"vsri.32 q5, q4, #19\n\t"
		"veor q3, q5\n\t"                   // d ^= ...;
		"vadd.i32 q4, q3, q2\n\t"           // d + c;
		"vshl.i32 q5, q4, #18\n\t"          // (d + c) <<< 18;
		"vsri.32 q5, q4, #14\n\t"
		"veor q0, q5\n\t"                   // a ^= ...;
		/* Inverse data allocation */
		"vtrn.32 q0, q1\n\t"
		"vtrn.32 q2, q3\n\t"
		"vswp.32 d1, d4\n\t"
		"vswp.32 d3, d6\n\t"
		"vext.32 q1, q1, q1, #3\n\t"
		"vext.32 q2, q2, q2, #2\n\t"
		"vext.32 q3, q3, q3, #1\n\t"
		"vst4.32 {q0, q1, q2, q3}, %[y]\n\t" 
		:
		: [y] "r" (y)
		: "q0", "q1", "q2", "q3", "q4", "q5"
	);
}

void chacha_diagonalround_asm(uint32_t y[16])
{
	/* Required state:
	*   y[0],   y[5],  y[10],  y[15]
	*   y[1],   y[6],  y[11],  y[12]
	*   y[2],   y[7],  y[8],   y[13]
	*   y[3],   y[4],  y[9],   y[14]
	*  q0 = a, q1 = b  q2 = c, q3 = d
	*/
	asm(
		"vldm.32 %[y], {q0, q1, q2, q3}\n\t"
		/* Now q0-q3 are:
		*   y[0],   y[4],  y[8],   y[12]
		*   y[1],   y[5],  y[9],   y[13]
		*   y[2],   y[6],  y[10],  y[14]
		*   y[3],   y[7],  y[11],  y[15]
		*  q0 = a, q1 = b  q2 = c, q3 = d
		*/
		"vext.32 q1, q1, q1, #1\n\t"
		"vext.32 q2, q2, q2, #2\n\t"
		"vext.32 q3, q3, q3, #3\n\t"
		/* Now q0-q3 are as we need */
		"vadd.i32 q0, q1\n\t"                // a += b;
		"veor q3, q0\n\t"                    // d ^= a;
		"vshl.i32 q4, q3, #16\n\t"           // d = d <<< 16;
		"vshr.u32 q5, q3, #16\n\t"
		"vorr q3, q4, q5\n\t"
		"vadd.i32 q2, q3\n\t"                // c += d;
		"veor q1, q2\n\t"                    // b ^= c;
		"vshl.i32 q4, q1, #12\n\t"           // b = b <<< 12;
		"vshr.u32 q5, q1, #20\n\t"
		"vorr q1, q4, q5\n\t"
		"vadd.i32 q0, q1\n\t"                // a += b;
		"veor q3, q0\n\t"                    // d ^= a;
		"vshl.i32 q4, q3, #8\n\t"            // d = d <<< 8;
		"vshr.u32 q5, q3, #24\n\t"
		"vorr q3, q4, q5\n\t"
		"vadd.i32 q2, q3\n\t"                // c += d;
		"veor q1, q2\n\t"                    // b ^= c;
		"vshl.i32 q4, q1, #7\n\t"            // b = b <<< 7;
		"vshr.u32 q5, q1, #25\n\t"
		"vorr q1, q4, q5\n\t"
		/* Inverse data allocation */
		"vext.32 q1, q1, q1, #3\n\t"
		"vext.32 q2, q2, q2, #2\n\t"
		"vext.32 q3, q3, q3, #1\n\t"
		"vstm.32 %[y], {q0, q1, q2, q3}\n\t" 
		:
		: [y] "r" (y)
		: "q0", "q1", "q2", "q3", "q4", "q5"
	);
}

void chacha_columnround_asm(uint32_t y[16])
{
	/* Required state:
	*   y[0],   y[4],  y[8],   y[12]
	*   y[1],   y[5],  y[9],   y[13]
	*   y[2],   y[6],  y[10],  y[14]
	*   y[3],   y[7],  y[11],  y[15]
	*  q0 = a, q1 = b  q2 = c, q3 = d
	*/
	asm(
		"vldm.32 %[y], {q0, q1, q2, q3}\n\t"
		"vadd.i32 q0, q1\n\t"                // a += b;
		"veor q3, q0\n\t"                    // d ^= a;
		"vshl.i32 q4, q3, #16\n\t"           // d = d <<< 16;
		"vshr.u32 q5, q3, #16\n\t"
		"vorr q3, q4, q5\n\t"
		"vadd.i32 q2, q3\n\t"                // c += d;
		"veor q1, q2\n\t"                    // b ^= c;
		"vshl.i32 q4, q1, #12\n\t"           // b = b <<< 12;
		"vshr.u32 q5, q1, #20\n\t"
		"vorr q1, q4, q5\n\t"
		"vadd.i32 q0, q1\n\t"                // a += b;
		"veor q3, q0\n\t"                    // d ^= a;
		"vshl.i32 q4, q3, #8\n\t"            // d = d <<< 8;
		"vshr.u32 q5, q3, #24\n\t"
		"vorr q3, q4, q5\n\t"
		"vadd.i32 q2, q3\n\t"                // c += d;
		"veor q1, q2\n\t"                    // b ^= c;
		"vshl.i32 q4, q1, #7\n\t"            // b = b <<< 7;
		"vshr.u32 q5, q1, #25\n\t"
		"vorr q1, q4, q5\n\t"
		"vstm.32 %[y], {q0, q1, q2, q3}\n\t" 
		:
		: [y] "r" (y)
		: "q0", "q1", "q2", "q3", "q4", "q5"
	);
}

void chacha_diagonalround_asm_triple(uint32_t y[48])
{
	/* Required state:
	*   y[0],   y[5],  y[10],  y[15]
	*   y[1],   y[6],  y[11],  y[12]
	*   y[2],   y[7],  y[8],   y[13]
	*   y[3],   y[4],  y[9],   y[14]
	*  q0 = a, q1 = b  q2 = c, q3 = d
	*/
	asm(
		"vldm.32 %[y], {q0-q11}\n\t"
		/* Now q0-q3 (q4-q7 and q8-q11 too) are:
		*   y[0],   y[4],  y[8],   y[12]
		*   y[1],   y[5],  y[9],   y[13]
		*   y[2],   y[6],  y[10],  y[14]
		*   y[3],   y[7],  y[11],  y[15]
		*  q0 = a, q1 = b  q2 = c, q3 = d
		*/
		"vext.32 q1, q1, q1, #1\n\t"
		"vext.32 q4, q4, q4, #1\n\t"
		"vext.32 q9, q9, q9, #1\n\t"
		"vext.32 q2, q2, q2, #2\n\t"
		"vext.32 q6, q6, q6, #2\n\t"
		"vext.32 q10, q10, q10, #2\n\t"
		"vext.32 q3, q3, q3, #3\n\t"
		"vext.32 q7, q7, q7, #3\n\t"
		"vext.32 q11, q11, q11, #3\n\t"
		/* Now q0-q3 (q4-q7 and q8-q11 too) are as we need */
		
		/* a += b; */
		"vadd.i32 q0, q1\n\t"
		"vadd.i32 q4, q5\n\t"
		"vadd.i32 q8, q9\n\t"

		/* d ^= a; */
		"veor q3, q0\n\t"
		"veor q7, q4\n\t"
		"veor q11, q8\n\t"

		/* q12(13, 14) = d <<< 16;
		q12(13, 14) is d at the moment */
		"vshl.i32 q12, q3, #16\n\t"
		"vshl.i32 q13, q7, #16\n\t"
		"vshl.i32 q14, q11, #16\n\t"
		"vsri.32 q12, q3, #16\n\t"
		"vsri.32 q13, q7, #16\n\t"
		"vsri.32 q14, q11, #16\n\t"

		/* c += d; */
		"vadd.i32 q2, q12\n\t"
		"vadd.i32 q6, q13\n\t"
		"vadd.i32 q10, q14\n\t"

		/* b ^= c; */
		"veor q1, q2\n\t"
		"veor q5, q6\n\t"
		"veor q9, q10\n\t"

		/* q3(7, 11) = b <<< 12; 
		q3(7, 11) is b at the moment */
		"vshl.i32 q3, q1, #12\n\t"
		"vshl.i32 q7, q5, #12\n\t"
		"vshl.i32 q11, q9, #12\n\t"
		"vsri.32 q3, q1, #20\n\t"
		"vsri.32 q7, q5, #20\n\t"
		"vsri.32 q11, q9, #20\n\t"

		/* a += b; */
		"vadd.i32 q0, q3\n\t"
		"vadd.i32 q4, q7\n\t"
		"vadd.i32 q8, q11\n\t"

		/* d ^= a; */
		"veor q12, q0\n\t"
		"veor q13, q4\n\t"
		"veor q14, q8\n\t"

		/* q1(5, 9) = d <<< 8;
		q1(5, 9) is d at the moment */
		"vshl.i32 q1, q12, #8\n\t"
		"vshl.i32 q5, q13, #8\n\t"
		"vshl.i32 q9, q14, #8\n\t"
		"vsri.32 q1, q12, #24\n\t"
		"vsri.32 q5, q13, #24\n\t"
		"vsri.32 q9, q14, #24\n\t"

		/* c += d; */
		"vadd.i32 q2, q1\n\t"
		"vadd.i32 q6, q5\n\t"
		"vadd.i32 q10, q9\n\t"

		/* b ^= c; */
		"veor q3, q2\n\t"
		"veor q7, q6\n\t"
		"veor q11, q10\n\t"

		/* q12(13, 14) = b <<< 7; 
		q12(13, 14) is b at the moment */
		"vshl.i32 q12, q3, #7\n\t"
		"vshl.i32 q13, q7, #7\n\t"
		"vshl.i32 q14, q11, #7\n\t"
		"vsri.32 q12, q3, #25\n\t"
		"vsri.32 q13, q7, #25\n\t"
		"vsri.32 q14, q11, #25\n\t"

		/* d to q3(7, 11) */
		"vswp.32 q3, q1\n\t"
		"vswp.32 q7, q5\n\t"
		"vswp.32 q11, q9\n\t"

		/* b to q1(5, 9) */
		"vswp.32 q1, q12\n\t"
		"vswp.32 q5, q13\n\t"
		"vswp.32 q9, q14\n\t"

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
		"vstm.32 %[y], {q0-q11}\n\t" 
		:
		: [y] "r" (y)
		: "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7",
		  "q8", "q9", "q10", "q11", "q12", "q13", "q14"
	);
}

void chacha_columnround_asm_triple(uint32_t y[48])
{
	/* Required state:
	*   y[0],   y[4],  y[8],   y[12]
	*   y[1],   y[5],  y[9],   y[13]
	*   y[2],   y[6],  y[10],  y[14]
	*   y[3],   y[7],  y[11],  y[15]
	*  q0 = a, q1 = b  q2 = c, q3 = d
	*/
	asm(
		"vldm.32 %[y], {q0-q11}\n\t"

		/* a += b; */
		"vadd.i32 q0, q1\n\t"
		"vadd.i32 q4, q5\n\t"
		"vadd.i32 q8, q9\n\t"

		/* d ^= a; */
		"veor q3, q0\n\t"
		"veor q7, q4\n\t"
		"veor q11, q8\n\t"

		/* q12(13, 14) = d <<< 16;
		q12(13, 14) is d at the moment */
		"vshl.i32 q12, q3, #16\n\t"
		"vshl.i32 q13, q7, #16\n\t"
		"vshl.i32 q14, q11, #16\n\t"
		"vsri.32 q12, q3, #16\n\t"
		"vsri.32 q13, q7, #16\n\t"
		"vsri.32 q14, q11, #16\n\t"

		/* c += d; */
		"vadd.i32 q2, q12\n\t"
		"vadd.i32 q6, q13\n\t"
		"vadd.i32 q10, q14\n\t"

		/* b ^= c; */
		"veor q1, q2\n\t"
		"veor q5, q6\n\t"
		"veor q9, q10\n\t"

		/* q3(7, 11) = b <<< 12; 
		q3(7, 11) is b at the moment */
		"vshl.i32 q3, q1, #12\n\t"
		"vshl.i32 q7, q5, #12\n\t"
		"vshl.i32 q11, q9, #12\n\t"
		"vsri.32 q3, q1, #20\n\t"
		"vsri.32 q7, q5, #20\n\t"
		"vsri.32 q11, q9, #20\n\t"

		/* a += b; */
		"vadd.i32 q0, q3\n\t"
		"vadd.i32 q4, q7\n\t"
		"vadd.i32 q8, q11\n\t"

		/* d ^= a; */
		"veor q12, q0\n\t"
		"veor q13, q4\n\t"
		"veor q14, q8\n\t"

		/* q1(5, 9) = d <<< 8;
		q1(5, 9) is d at the moment */
		"vshl.i32 q1, q12, #8\n\t"
		"vshl.i32 q5, q13, #8\n\t"
		"vshl.i32 q9, q14, #8\n\t"
		"vsri.32 q1, q12, #24\n\t"
		"vsri.32 q5, q13, #24\n\t"
		"vsri.32 q9, q14, #24\n\t"

		/* c += d; */
		"vadd.i32 q2, q1\n\t"
		"vadd.i32 q6, q5\n\t"
		"vadd.i32 q10, q9\n\t"

		/* b ^= c; */
		"veor q3, q2\n\t"
		"veor q7, q6\n\t"
		"veor q11, q10\n\t"

		/* q12(13, 14) = b <<< 7; 
		q12(13, 14) is b at the moment */
		"vshl.i32 q12, q3, #7\n\t"
		"vshl.i32 q13, q7, #7\n\t"
		"vshl.i32 q14, q11, #7\n\t"
		"vsri.32 q12, q3, #25\n\t"
		"vsri.32 q13, q7, #25\n\t"
		"vsri.32 q14, q11, #25\n\t"

		/* d to q3(7, 11) */
		"vswp.32 q3, q1\n\t"
		"vswp.32 q7, q5\n\t"
		"vswp.32 q11, q9\n\t"

		/* b to q1(5, 9) */
		"vswp.32 q1, q12\n\t"
		"vswp.32 q5, q13\n\t"
		"vswp.32 q9, q14\n\t"

		"vstm.32 %[y], {q0-q11}\n\t" 
		:
		: [y] "r" (y)
		: "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7",
		  "q8", "q9", "q10", "q11", "q12", "q13", "q14"
	);
}
