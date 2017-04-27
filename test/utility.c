#include <stdint.h>
#include <stddef.h>

void printbytes(uint8_t *byte, size_t size)
{
	int i, j;

	for (i = 0; i < size / 16; i++) {
		for (j = 0; j < 16; j++)
			printf("%03d, ", byte[i * 16 + j]);
		// printf("\n");
	}
	if (size % 16) {
		for (j = 0; j < size % 16; j++)
			printf("%03d, ", byte[i * 16 + j]);
		// printf("\n");
	}
}
