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
