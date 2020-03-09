// MP#3 | ECE220
// Developer: Unnat Ramjiyani
// NetID: unnatr2

// The following code takes the row index of the Pascal's Triangle as an
// input. It the computes the coeffecient and then prints the row in a
// new line.

#include <stdio.h>                      // include Standard I/O
#include <stdlib.h>                     // include Standard Library

int main()                              // Function: MAIN
{
        unsigned long row,n,k,i,sum;    // Define variables

        printf("Enter a row index: ");  // Ask user for row index
        scanf("%lu",&row);              // Input character from user

        n=row;                          // Assign row to n
        for (k=0;k<=n;k++)              // Loop through coefficients of row
        {
                sum=1;                  // Assign sum to 1
                for (i=1;i<=k;i++)      // loop though multiplication to find result
                {
                        sum*=((n-i)+1); // Multiply sum and numerator
                        sum/=i;         // Divide sum and denominator
                }
                printf("%lu ",sum);     // Print coefficients
        }
        printf("\n");                   // Print new line
        return 0;                       // Return 0, finish MAIN
}
