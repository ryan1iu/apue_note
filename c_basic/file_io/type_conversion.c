#include <stdio.h>

int main()
{
    // 隐式类型转换的基本原则是小精度向大精度转换

    // int 与 float 都是 32 位， 谁的精度更高
    
    int i = 10;
    float f = 10.2;
    float f2 = i + f;
    printf("%f", f2);
    return 0;
}
