#include <stdint.h>

void salsa_rowround_asm(uint32_t y[16])
{
	//  y[0],  y[1],   y[2],   y[3]
	//  y[5],  y[6],   y[7],   y[4]
	//  y[10], y[11],  y[8],   y[9]
	//  y[15], y[12],  y[13],  y[14]
	// q0 = a, q1 = b  q2 = c, q3 = d
	/*
	* b ^= ((a + d) << 7 | (a + d) >> 32 - 7);
	* c ^= ((b + a) << 9 | (b + a) >> 32 - 9);
	* d ^= ((c + b) << 13 | (c + b) >> 32 - 13);
	* a ^= ((d + c) << 18 | (d + c) >> 32 - 18);
	*/
	asm(
		"vld4.32 {q0[0], q1[0], q2[0], q3[0]}, %[y]\n\t"
		"vld4.32 {q3[1], q0[1], q1[1], q2[1]}, %[y]\n\t"
		"vld4.32 {q2[2], q3[2], q0[2], q1[2]}, %[y]\n\t"
		"vld4.32 {q1[3], q2[3], q3[3], q0[3]}, %[y]\n\t"
		"vadd.i32 q4, q0, q3\n\t"            // a + d;
		"vshl.i32 q4, q4, #7\n\t"            // (a + d) << 7;
		"vadd.i32 q5, q0, q3\n\t"            // a + d;
		"vshl.i32 q5, q5, #25\n\t"           // (a + d) >> 25;
		"vorr q4, q4, q5\n\t"                // () | ()
		"veor q1, q1, q4\n\t"                // b ^= ...;
		"vadd.i32 q4, q1, q0\n\t"            // b + a;
		"vshl.i32 q4, q4, #9\n\t"            // (b + a) << 9;
		"vadd.i32 q5, q1, q0\n\t"            // b + a;
		"vshl.i32 q5, q5, #23\n\t"           // (b + a) >> 23;
		"vorr q4, q4, q5\n\t"                // () | ()
		"veor q2, q2, q4\n\t"                // c ^= ...;
		"vadd.i32 q4, q2, q1\n\t"            // c + b;
		"vshl.i32 q4, q4, #13\n\t"           // (c + b) << 13;
		"vadd.i32 q5, q2, q1\n\t"            // c + b;
		"vshl.i32 q5, q5, #19\n\t"           // (c + b) >> 19;
		"vorr q4, q4, q5\n\t"                // () | ()
		"veor q3, q3, q4\n\t"                // d ^= ...;
		"vadd.i32 q4, q3, q2\n\t"            // d + c;
		"vshl.i32 q4, q4, #18\n\t"           // (d + c) << 18;
		"vadd.i32 q5, q3, q2\n\t"            // d + c;
		"vshl.i32 q5, q5, #14\n\t"           // (d + c) >> 14;
		"vorr q4, q4, q5\n\t"                // () | ()
		"veor q0, q0, q4\n\t"                // a ^= ...;
		"vstm.32 %[y], {q0, q1, q2, q3}\n\t" 
		:
		: [y] "r" (y)
		: "q0", "q1", "q2", "q3", "q4", "q5"
	);
}

void salsa_columnround_asm(uint32_t x[16])
{
	//  x[0],   x[4],  x[8],   x[12]
	//  x[5],   x[9],  x[13],  x[1]
	//  x[10],  x[14], x[2],   x[6]
	//  x[15],  x[3],  x[7],   x[11]
	// q0 = a, q1 = b  q2 = c, q3 = d
	/*
	* b ^= ((a + d) << 7 | (a + d) >> 32 - 7);
	* c ^= ((b + a) << 9 | (b + a) >> 32 - 9);
	* d ^= ((c + b) << 13 | (c + b) >> 32 - 13);
	* a ^= ((d + c) << 18 | (d + c) >> 32 - 18);
	*/
	asm(
		"vld4.32 {q0[0], q3[1], q2[2], q1[3]}, %[y]\n\t"
		"vld4.32 {q1[0], q0[1], q3[2], q2[3]}, %[y]\n\t"
		"vld4.32 {q2[0], q1[1], q0[2], q3[3]}, %[y]\n\t"
		"vld4.32 {q3[0], q2[1], q1[2], q0[3]}, %[y]\n\t"
		"vadd.i32 q4, q0, q3\n\t"            // a + d;
		"vshl.i32 q4, q4, #7\n\t"            // (a + d) << 7;
		"vadd.i32 q5, q0, q3\n\t"            // a + d;
		"vshl.i32 q5, q5, #25\n\t"           // (a + d) >> 25;
		"vorr q4, q4, q5\n\t"                // () | ()
		"veor q1, q1, q4\n\t"                // b ^= ...;
		"vadd.i32 q4, q1, q0\n\t"            // b + a;
		"vshl.i32 q4, q4, #9\n\t"            // (b + a) << 9;
		"vadd.i32 q5, q1, q0\n\t"            // b + a;
		"vshl.i32 q5, q5, #23\n\t"           // (b + a) >> 23;
		"vorr q4, q4, q5\n\t"                // () | ()
		"veor q2, q2, q4\n\t"                // c ^= ...;
		"vadd.i32 q4, q2, q1\n\t"            // c + b;
		"vshl.i32 q4, q4, #13\n\t"           // (c + b) << 13;
		"vadd.i32 q5, q2, q1\n\t"            // c + b;
		"vshl.i32 q5, q5, #19\n\t"           // (c + b) >> 19;
		"vorr q4, q4, q5\n\t"                // () | ()
		"veor q3, q3, q4\n\t"                // d ^= ...;
		"vadd.i32 q4, q3, q2\n\t"            // d + c;
		"vshl.i32 q4, q4, #18\n\t"           // (d + c) << 18;
		"vadd.i32 q5, q3, q2\n\t"            // d + c;
		"vshl.i32 q5, q5, #14\n\t"           // (d + c) >> 14;
		"vorr q4, q4, q5\n\t"                // () | ()
		"veor q0, q0, q4\n\t"                // a ^= ...;
		"vstm.32 %[y], {q0, q1, q2, q3}\n\t" 
		:
		: [y] "r" (y)
		: "q0", "q1", "q2", "q3", "q4", "q5"
	);
}

void chacha_diagonalround_asm(uint32_t y[16])
{
	//  y[0],   y[5],  y[10],  y[15]
	//  y[1],   y[6],  y[11],  y[12]
	//  y[2],   y[7],  y[8],   y[13]
	//  y[3],   y[4],  y[9],   y[14]
	// q0 = a, q1 = b  q2 = c, q3 = d
	asm(
		"vldm.32 %[y], {q0, q1, q2, q3}\n\t"
		/* now q0-q3 are:
		*   y[0],   y[4],  y[8],   y[12]
		*   y[1],   y[5],  y[9],   y[13]
		*   y[2],   y[6],  y[10],  y[14]
		*   y[3],   y[7],  y[11],  y[15]
		*  q0 = a, q1 = b  q2 = c, q3 = d
		*/
		"vext.32 q1, q1, q1, #1\n\t"
		"vext.32 q2, q2, q2, #2\n\t"
		"vext.32 q3, q3, q3, #3\n\t"
		/* now q0-q3 are as we need */
		"vadd.i32 q0, q0, q1\n\t"             // a += b;
		"veor q3, q0, q3\n\t"                 // d ^= a;
		"vshl.i32 q4, q3, #16\n\t"            // d = d <<< 16;
		"vshr.u32 q5, q3, #16\n\t"
		"vorr q3, q4, q5\n\t"
		"vadd.i32 q2, q2, q3\n\t"             // c += d;
		"veor q1, q1, q2\n\t"                 // b ^= c;
		"vshl.i32 q4, q1, #12\n\t"            // b = b <<< 12;
		"vshr.u32 q5, q1, #20\n\t"
		"vorr q1, q4, q5\n\t"
		"vadd.i32 q0, q0, q1\n\t"             // a += b;
		"veor q3, q0, q3\n\t"                 // d ^= a;
		"vshl.i32 q4, q3, #8\n\t"             // d = d <<< 8;
		"vshr.u32 q5, q3, #24\n\t"
		"vorr q3, q4, q5\n\t"
		"vadd.i32 q3, q3, q3\n\t"             // c += d;
		"veor q2, q2, q3\n\t"                 // b ^= c;
		"vshl.i32 q4, q1, #7\n\t"             // b = b <<< 7;
		"vshr.u32 q5, q1, #25\n\t"
		"vorr q1, q4, q5\n\t"
		"vstm.32 %[y], {q0, q1, q2, q3}\n\t" 
		:
		: [y] "r" (y)
		: "q0", "q1", "q2", "q3", "q4", "q5"
	);
}

void chacha_columnround_asm(uint32_t y[16])
{
	//  y[0],   y[4],  y[8],   y[12]
	//  y[1],   y[5],  y[9],   y[13]
	//  y[2],   y[6],  y[10],  y[14]
	//  y[3],   y[7],  y[11],  y[15]
	// q0 = a, q1 = b  q2 = c, q3 = d
	asm(
		"vldm.32 %[y], {q0, q1, q2, q3}\n\t"
		"vadd.i32 q0, q0, q1\n\t"             // a += b;
		"veor q3, q0, q3\n\t"                 // d ^= a;
		"vshl.i32 q4, q3, #16\n\t"            // d = d <<< 16;
		"vshr.u32 q5, q3, #16\n\t"
		"vorr q3, q4, q5\n\t"
		"vadd.i32 q2, q2, q3\n\t"             // c += d;
		"veor q1, q1, q2\n\t"                 // b ^= c;
		"vshl.i32 q4, q1, #12\n\t"            // b = b <<< 12;
		"vshr.u32 q5, q1, #20\n\t"
		"vorr q1, q4, q5\n\t"
		"vadd.i32 q0, q0, q1\n\t"             // a += b;
		"veor q3, q0, q3\n\t"                 // d ^= a;
		"vshl.i32 q4, q3, #8\n\t"             // d = d <<< 8;
		"vshr.u32 q5, q3, #24\n\t"
		"vorr q3, q4, q5\n\t"
		"vadd.i32 q3, q3, q3\n\t"             // c += d;
		"veor q2, q2, q3\n\t"                 // b ^= c;
		"vshl.i32 q4, q1, #7\n\t"             // b = b <<< 7;
		"vshr.u32 q5, q1, #25\n\t"
		"vorr q1, q4, q5\n\t"
		"vstm.32 %[y], {q0, q1, q2, q3}\n\t" 
		:
		: [y] "r" (y)
		: "q0", "q1", "q2", "q3", "q4", "q5"
	);
}
