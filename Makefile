all:
	gcc imgcompress.c -o imgcompress
	gcc decompress.c -I. -o decompress

clean:
	rm -f imgcompress
