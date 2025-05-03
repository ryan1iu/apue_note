#include <stdio.h>
#include <stdlib.h>

int main()
{
    auto int i;
    printf("%d\n", i);  // The execution output 32645, not 0. Variables of type `auto` are not initialized to 0 when allocated memory.


    exit(0);
}
