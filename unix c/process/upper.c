#include <stdio.h>

int main()
{
    int ch;
    while ((ch = getchar()) != EOF)
    {
        putchar(toupper(ch));
    }
    return 0;
}
