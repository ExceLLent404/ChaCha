void chacha_rowround(uint32_t y[16])
{
	// chacha_quarterround(&y[0], &y[5], &y[10], &y[15]);
	// chacha_quarterround(&y[1], &y[6], &y[11], &y[12]);
	// chacha_quarterround(&y[2], &y[7], &y[8], &y[13]);
	// chacha_quarterround(&y[3], &y[4], &y[9], &y[14]);
	asm(
		"vld4.32 {d0, d1, d2, d3}, %[y]\n\t"
		"vadd.i32 d0, d0, d1\n\t"              //  a += b;
		"veor d4, d0, d4\n\t"                  //  d ^= a;
		"\n\t"  // d = d << 16 | d >> 32 - 16;
		"vadd.i32 d3, d3, d4\n\t"              // c += *d;
		"veor d2, d2, d3\n\t"                  // b ^= *c;
		"\n\t"  // b = *b << 12 | *b >> 32 - 12;
		"vadd.i32 d0, d0, d1\n\t"              // a += *b;
		"veor d4, d0, d4\n\t"                  // d ^= *a;
		"\n\t"    // d = *d << 8 | *d >> 32 - 8;
		"vadd.i32 d3, d3, d4\n\t"              // c += *d;
		"veor d2, d2, d3\n\t"                  // b ^= *c;
		"\n\t"    // b = *b << 7 | *b >> 32 - 7;
		"vst4.32 {d0, d1, d2, d3}, %[y]\n\t" 
		:
		: [y] "r" (y)
		: "d0", "d1", "d2", "d3"
	);
}
