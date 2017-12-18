imgcompress
===========

Convert a disk image to a C define.

Usage
-----

    imgcompress FILENAME > image.h

The output might look like this:

    \*
     *  Non-zero 8 byte strings of a disk image
     *
     *  SPDX-License-Identifier:     GPL-2.0+
     *\
    #define EFI_ST_DISK_IMG { 0x00010000, { \
        {0x000001b8, "\x94\x37\x69\xfc\x00\x00\x00\x00" /* .7i..... */ \
        {0x000001c0, "\x02\x00\x83\x02\x02\x00\x01\x00" /* ........ */ \
        {0x000001c8, "\x00\x00\x7f\x00\x00\x00\x00\x00" /* ........ */ \
        {0x000001f8, "\x00\x00\x00\x00\x00\x00\x55\xaa" /* ......U. */ \
    ...
        {0x00006000, "\x48\x65\x6c\x6c\x6f\x20\x77\x6f" /* Hello wo */ \
        {0x00006008, "\x72\x6c\x64\x21\x0a\x00\x00\x00" /* rld!.... */ \
        } }

Decompression
-------------

File decompress.c shows how the compressed image can be restored.

Let image.h contain an image file created by imgcompress. Then you can
restore the original file with

    make
    ./decompress > FILENAME
