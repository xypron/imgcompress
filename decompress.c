/*
 * Decompress a file stored as define in an include
 *
 * (c) Heinrich Schuchardt <xypron.glpk@gmx.de>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include "image.h"

#define L 8

struct line {
	size_t addr;
	unsigned char *line;
};

struct img {
	size_t length;
	struct line lines[];
} img = EFI_ST_DISK_IMG;

int main(int argc, char *argv[])
{
	unsigned char *buf;
	size_t i;
	size_t addr;
	size_t len;

	buf = malloc(img.length);
	if (!buf) {
		perror("calloc");
		return EXIT_FAILURE;
	}
	memset(buf, 0, img.length);

	for (i = 0;;++i) {
		if (!img.lines[i].line)
			break;
		addr = img.lines[i].addr;
		len = L;
		if (addr + len > img.length)
			len = img.length - addr;
		memcpy(buf + addr, img.lines[i].line, len);
	}

	fwrite(buf, 1, img.length, stdout);
	free(buf);
	return EXIT_SUCCESS;
}
