#include <stdio.h>

int main(void)
{
    int count;
    alarm(1);
    for (count = 0; 1; ++count)
    {
        printf("%d ", count);
    }
    return 0;
}
