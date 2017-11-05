#include "../inc/raw_client.h"

/*Вывод пакета на экран*/
void print_data_hex(const char* data, unsigned int size) {
	int i, j, line;
  int offset = 0;
  int nlines = size / PRINT_BYTES_PER_LINE;

  if(nlines * PRINT_BYTES_PER_LINE < size) {
		nlines++;
	}
	for(i = 0; i < 75; i++) {
		printf("-");
	}
	printf("\n        ");
	for(i = 0; i < PRINT_BYTES_PER_LINE; i++) {
		printf("%02X ", (unsigned int)i);
	}
	printf("   Pack size: %d\n\n", size);
	for(line = 0; line < nlines; line++) 	{
		printf("%04X    ", offset);
		for(j = 0; j < PRINT_BYTES_PER_LINE; j++) {
			if(offset + j >= size) {
				printf("   ");
			} else {
				printf("%02X ", (unsigned char)data[offset + j]);
			}
		}
		printf("   ");
		for(j = 0; j < PRINT_BYTES_PER_LINE; j++)	{
			if(offset + j >= size) {
				printf(" ");
			} else if(isgraph(data[offset + j])) {
				printf("%c", data[offset + j]);
			} else {
				printf(".");
			}
		}
		offset += PRINT_BYTES_PER_LINE;
		printf("\n");
	}
	for(i = 0; i < 75; i++) {
		printf("-");
	}
	printf("\n");
}
