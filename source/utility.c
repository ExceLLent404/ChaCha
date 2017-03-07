#include <stdint.h>

uint32_t littleendian(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3)
{
	return (uint32_t)b0 + ((uint32_t)b1 << 8)
		 + ((uint32_t)b2 << 16) + ((uint32_t)b3 << 24);
}

void wordtobyte(uint8_t* byte, uint32_t word)
{
	/* Is it ... */
	/*int i;

	for (i = 0; i < 4; i++)
		byte[i] = word >> 8 * i;*/
	/* ... better than this?*/
	byte[0] = word;
	byte[1] = word >> 8;
	byte[2] = word >> 16;
	byte[3] = word >> 24;
}
