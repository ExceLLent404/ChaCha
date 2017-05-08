#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "../include/chacha20.h"
#include "../include/chacha20_triple_matrix.h"
#include "../include/chacha20_refo.h"

static void
usage(const char *name)
{
	fprintf(stderr,
	        "Usage: %s [OPTION...]\n"
	        "  -f FILENAME  Use file for tests\n"
	        "               Can't be used together with -s option\n"
	        "  -s STR       Use string STR for tests\n"
	        "               Can't be used together with -f option\n"
	        "  -h           Show this usage and exit\n",
	        name);

	exit(EXIT_FAILURE);
}

static void
parse_opts(int argc, char **argv, char **pfilename, char **pstr, int *pquiet)
{
	if (argc < 2)
		usage(argv[0]);

	int opt;

	*pfilename = NULL;
	*pstr = NULL;
	*pquiet = 0;

	while ((opt = getopt(argc, argv, "f:s:qh")) != -1)
		switch (opt) {
		case 'f':
			*pfilename = optarg;
			break;
		case 's':
			*pstr = optarg;
			break;
		case 'q':
			*pquiet = 1;
			break;
		case 'h':
			usage(argv[0]);
			break;
		default:
			usage(argv[0]);
			break;
		}

	if ((*pfilename != NULL && *pstr != NULL) ||
	        (*pfilename == NULL && *pstr == NULL))
		usage(argv[0]);
}

#define CHACHA20_IV_SIZE    8
#define CHACHA20_BLOCK_SIZE 64
#define MEMORY_BLOCK_SIZE   (CHACHA20_BLOCK_SIZE * 3)
#define MEMORY_ALIGNMENT    64

static size_t
fit_size_to_blocks(size_t size)
{
	size_t count, rem;

	count = size / MEMORY_BLOCK_SIZE;
	rem = size % MEMORY_BLOCK_SIZE;
	count += rem == 0 ? 0 : 1;
	return count * MEMORY_BLOCK_SIZE;
}

static void *
proper_alloc(size_t size)
{
	void *p;

	size = fit_size_to_blocks(size);
	if (posix_memalign(&p, MEMORY_ALIGNMENT, size)) {
		fprintf(stderr, "Can't allocate %u bytes in memory\n", size);
		exit(EXIT_FAILURE);
	}

	printf("Allocated %u bytes pointed by %p\n", size, p);

	return p;
}

static size_t
load_file(const char *filename, char **out)
{
	int fd;
	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr,
		        "Error during opening file %s\n", filename);
		exit(EXIT_FAILURE);
	}

	off_t off;
	off = lseek(fd, (off_t) 0, SEEK_END);
	if (off < 0) {
		fprintf(stderr,
		        "Error during processing file %s\n", filename);
		exit(EXIT_FAILURE);
	}

	size_t filesize;
	filesize = (size_t) off;

	*out = proper_alloc(filesize);

	off = lseek(fd, (off_t) 0, SEEK_SET);
	if (off < 0) {
		fprintf(stderr,
		        "Error during processing file %s\n", filename);
		exit(EXIT_FAILURE);
	}

	ssize_t rb;
	rb = read(fd, *out, filesize);
	if (rb < 0) {
		fprintf(stderr,
		        "Error during loading file %s\n", filename);
		exit(EXIT_FAILURE);
	}

	return (size_t) rb;
}

static int
rand_bytes(unsigned char *str, size_t size)
{
	int i;

	for (i = 0; i < size; i++)
		str[i] = rand() % 256;

	return 1;
}

static void
print_hex(uint8_t *a, size_t size)
{
	int i;

	for (i = 0; i < size; i++)
		printf("%02X", a[i]);
}

static double
test_chacha20_refo(const unsigned char *plain, size_t len,
        const unsigned char *key, size_t key_len, unsigned char *iv_enc,
	unsigned char *iv_dec, unsigned char *enc, size_t enc_len,
        unsigned char *dec)
{
	clock_t c;
	ECRYPT_ctx ctx;

	c = clock();

	ECRYPT_keysetup(&ctx, key, key_len, CHACHA20_IV_SIZE);
	ECRYPT_ivsetup(&ctx, iv_enc);
	ECRYPT_encrypt_bytes(&ctx, plain, enc, len);

	ECRYPT_keysetup(&ctx, key, key_len, CHACHA20_IV_SIZE);
	ECRYPT_ivsetup(&ctx, iv_dec);
	ECRYPT_decrypt_bytes(&ctx, enc, dec, len);

	return ((double) clock() - c) / CLOCKS_PER_SEC;
}

static double
test_chacha20_asmi(unsigned char *plain, size_t len,
        unsigned char *key, size_t key_len, unsigned char *iv_enc,
	unsigned char *iv_dec, unsigned char *enc, size_t enc_len,
        unsigned char *dec)
{
	clock_t c;

	len = fit_size_to_blocks(len);

	c = clock();

	chacha_encrypt(key, iv_enc, plain, len, enc);
	chacha_decrypt(key, iv_dec, enc, len, dec);

	return ((double) clock() - c) / CLOCKS_PER_SEC;
}

static double
test_chacha20_asmi3l(unsigned char *plain, size_t len,
        unsigned char *key, size_t key_len, unsigned char *iv_enc,
	unsigned char *iv_dec, unsigned char *enc, size_t enc_len,
        unsigned char *dec)
{
	clock_t c;

	len = fit_size_to_blocks(len);

	c = clock();

	chacha_encrypt_i(key, iv_enc, plain, len, enc);
	chacha_decrypt_i(key, iv_dec, enc, len, dec);

	return ((double) clock() - c) / CLOCKS_PER_SEC;
}

static void
test_chacha20(uint8_t *str, size_t size)
{
	double t;
	size_t key_len, enc_len, index;
	unsigned char *key, *iv, *iv_enc, *iv_dec, *enc_refo, *enc, *dec;

	key_len = 256;
	printf("\nTesting ChaCha20 with key of %d bits size\n",
	    key_len);

	key = malloc(sizeof(*key) * key_len / 8);
	iv = malloc(sizeof(*iv) * CHACHA20_IV_SIZE);
	iv_enc = malloc(sizeof(*iv_enc) * CHACHA20_IV_SIZE);
	iv_dec = malloc(sizeof(*iv_dec) * CHACHA20_IV_SIZE);
	enc_len = fit_size_to_blocks(size);
	enc_refo = proper_alloc(enc_len);
	enc = proper_alloc(enc_len);
	dec = proper_alloc(enc_len);
	if (key == NULL || iv == NULL || iv_enc == NULL || iv_dec == NULL) {
		fprintf(stderr, "Can't allocate enough memory for testing\n");
		exit(EXIT_FAILURE);
	}

	if (rand_bytes(key, key_len / 8) != 1) {
		fprintf(stderr, "Can't initialize key with random bytes\n");
		exit(EXIT_FAILURE);
	}

	printf("Using random key: ");
	print_hex(key, key_len / 8);

	if (rand_bytes(iv, CHACHA20_IV_SIZE) != 1) {
		fprintf(stderr, "Can't initialize IV with random bytes\n");
		exit(EXIT_FAILURE);
	}

	printf("\nUsing random IV:  ");
	print_hex(iv, CHACHA20_IV_SIZE);

	printf("\nREFO: ");

	memcpy(iv_enc, iv, CHACHA20_IV_SIZE);
	memcpy(iv_dec, iv, CHACHA20_IV_SIZE);

	t = test_chacha20_refo((const unsigned char *) str, size,
		(const unsigned char *) key, key_len, iv_enc, iv_dec,
		enc_refo, enc_len, dec);

	if (memcmp(str, dec, size) == 0)
		printf("plain and decoded messages are the same");
	else
		printf("plain and decoded messages are not the same");

	printf(", used time: %f seconds\n", t);

	printf("ASMI: ");

	memcpy(iv_enc, iv, CHACHA20_IV_SIZE);
	memcpy(iv_dec, iv, CHACHA20_IV_SIZE);

	t = test_chacha20_asmi(str, size, key, key_len, iv_enc,
		iv_dec, enc, enc_len, dec);

	if (memcmp(str, dec, size) == 0) {
		printf("plain and decoded messages are the same");
		if (memcmp(enc, enc_refo, size) == 0) {
			printf(", encoded message is the same "
			       "as of REFO's one");
		} else {
			printf(", but encoded message is not "
			       "the same as of REFO's one");

			index = 0;
			for (; enc[index] == enc_refo[index]; )
				index++;
			printf("\n\tDifference is in %uth byte"
			       " of %u bytes", index, size);

			printf("\n\tENC (ASMI + index)[96] = ");
			print_hex(enc + index, 96);
			printf("\n\tENC (REFO + index)[96] = ");
			print_hex(enc_refo + index, 96);
			printf("\ntest failed");
		}

	} else {
		printf("plain and decoded messages are not the same");
	}

	printf(", used time: %f seconds\n", t);

	printf("ASMI3L: ");

	memcpy(iv_enc, iv, CHACHA20_IV_SIZE);
	memcpy(iv_dec, iv, CHACHA20_IV_SIZE);

	t = test_chacha20_asmi3l(str, size, key, key_len,
		iv_enc, iv_dec, enc, enc_len, dec);

	if (memcmp(str, dec, size) == 0) {
		printf("plain and decoded messages are the same");
		if (memcmp(enc, enc_refo, size) == 0) {
			printf(", encoded message is the same "
			       "as of REFO's one");
		} else {
			printf(", but encoded message is not "
			       "the same as of REFO's one");

			index = 0;
			for (; enc[index] == enc_refo[index]; )
				index++;
			printf("\n\tDifference is in %uth byte"
			       " of %u bytes", index, size);

			printf("\n\tENC (ASMI + index)[96] = ");
			print_hex(enc + index, 96);
			printf("\n\tENC (REFO + index)[96] = ");
			print_hex(enc_refo + index, 96);
			printf("\ntest failed");
		}

	} else {
		printf("plain and decoded messages are not "
		       "the same");
	}

	printf(", used time: %f seconds\n", t);

	free(key);
	free(iv);
	free(iv_enc);
	free(iv_dec);
	free(enc_refo);
	free(enc);
	free(dec);
}

int main(int argc, char **argv)
{
	char *filename;
	char *str;
	int quiet;
	size_t size;

	parse_opts(argc, argv, &filename, &str, &quiet);

	if (filename != NULL) {
		size = load_file(filename, &str);
		if (!quiet)
			printf("Using filename %s of %u bytes size\n",
			        filename, size);
	} else {
		size = strlen(str);
		if (!quiet)
			printf("Using string \"%s\" with length %u\n",
			        str, size);
	}

	ECRYPT_init();

	test_chacha20((uint8_t *) str, size);

	if (filename != NULL)
		free(str);

	return EXIT_SUCCESS;
}
