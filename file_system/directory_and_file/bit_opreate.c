
/* 使用位运算判断两个u32_t数据的大小 */
#include <stdio.h>
unsigned compare(unsigned a, unsigned b)
{
	unsigned diff = a - b;
	unsigned sign = diff >> 31;
	return a - sign * diff;
}

/* 使用位运算计算一个u32_t数据二进制位中1的个数 */
int count_ones(unsigned a)
{
	int zero_count, one_count = 0;
	for (int i = 0; i < 32; i++) {
		if ((a & 0x1) > 0) {
			one_count++;
		} else {
			zero_count++;
		}
		a = a >> 1;
	}
	return one_count;
}

int count_ones2(unsigned a)
{
	int count = 0;
	while (a) {
		/* 清除最低位的1 */
		a = a & (a - 1);
		count++;
	}
	return count;
}

int main()
{
	unsigned a = 0xF0F;
	unsigned b = 0xF1F;
	printf("one count is %d\n", count_ones(a));
	printf("one count is %d\n", count_ones2(a));
	printf("bigger is %x\n", compare(a, b));
}
