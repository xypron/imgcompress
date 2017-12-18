/*
 * Convert a disk image to a C define
 *
 * (c) Heinrich Schuchardt <xypron.glpk@gmx.de>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>

#define L 8

int main(int argc, char *argv[]) {
	FILE *file;
	int ret;
	unsigned char *buf;
	size_t count, i, j;

	if (argc != 2) {
		printf("Usage: \n%s FILENAME\n", argv[0]);
		return EXIT_FAILURE;
	}
	file = fopen(argv[1], "r");
	if (!file) {
		perror("fopen");
		return EXIT_FAILURE;
	}
	ret = fseek(file, 0, SEEK_END);
	if (ret < 0) {
		perror("fseek");
		return EXIT_FAILURE;
	}
	count = ftell(file);
	if (!count) {
		fprintf(stderr, "File %s has length 0\n", argv[1]);
		return EXIT_FAILURE;
	}
	rewind(file);
	buf = malloc(count);
	if (!buf) {
		perror("calloc");
		return EXIT_FAILURE;
	}
	count = fread(buf, 1, count, file);
	printf("\\*\n");
	printf(" *  Non-zero %u byte strings of a disk image\n", L);
	printf(" *\n");
	printf(" *  Generated with https://github.com/xypron/imgcompress\n");
	printf(" *\n");
	printf(" *  SPDX-License-Identifier:     GPL-2.0+\n");
	printf(" *\\\n\n");
	printf("#define EFI_ST_DISK_IMG { 0x%08x, { \\\n", count);

	for (i = 0; i < count; i += L) {
		int c = 0;
		for (j = i; j < i + L && j < count; ++j) {
			if (buf[j])
				c = 1;
		}
		if (!c)
			continue;
		printf("\t{0x%08x, \"", i);
		for (j = i; j < i + L && j < count; ++j) {
			printf("\\x%02x", buf[j]);
		}
		printf("\" /* ");
		for (j = i; j < i + L && j < count; ++j) {
			if (buf[j] >= 0x20 && buf[j] <= 0x7e)
				printf("%c", buf[j]);
			else
				printf(".", buf[j]);
		}
		printf(" */ \\\n");
	}
	printf("\t} }\n");

	ret = fclose(file);
	if (ret) {
		perror("fclose");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
