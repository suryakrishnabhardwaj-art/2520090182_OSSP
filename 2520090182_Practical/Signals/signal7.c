#include <stdio.h>
int main()
{
int *p = NULL;
printf("Before error\n");
*p = 10;
printf("After error\n");
return 0;
}
