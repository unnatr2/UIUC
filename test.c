#include <stdlib.h>
#include <stdio.h>

int main()
{
        int a,b,c,d;
        srand(12321);
        a=(rand()%7)+1;
        b=(rand()%7)+1;
        c=(rand()%7)+1;
        d=(rand()%7)+1;
        printf("%d%d%d%d",a,b,c,d);
        return 1;
}
