#include <stdio.h>

union union1 {
	char a;
	char b[3];
};

union convert {
	int i;
	unsigned int j;
};

union Register {
	unsigned int i;
	struct {
		unsigned char byte1;
		unsigned char byte2;
		unsigned char byte3;
		unsigned char byte4;
	} bytes;
};

int main()
{
	// printf("%lu\n", sizeof(union union1));
	union convert a;
	a.i = 0xffffffff;
	/**
     * a.i = -1
     * a.j = 4294967295
     */
	printf("a.i = %d\na.j = %u\n", a.i, a.j);

	union Register r;
	r.i = 0x12345678;
	/*
     * byte1 = 78
     * byte2 = 56
     * byte3 = 34
     * byte4 = 12
     * */
	printf("byte1 = %x\nbyte2 = %x\nbyte3 = %x\nbyte4 = %x\n",
	       r.bytes.byte1, r.bytes.byte2, r.bytes.byte3, r.bytes.byte4);

	return 0;
}
