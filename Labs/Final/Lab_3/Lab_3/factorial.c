/* ######################################
# University of Information Technology #
# IT007 Operating System #
# <Your name>, <your Student ID> #
# File: factorial.c #
###################################### */

#include <stdio.h>

int main()
{
    int i, num, j;
    printf("Enter the number: ");
    scanf ("%d", &num );
    
    for (i=1; i<num; i++)
        j=j*i;
    
    printf("The factorial of %d is %d\n", num, j);
    return 0;
}