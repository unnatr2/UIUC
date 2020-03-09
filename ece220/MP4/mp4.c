// ECE220 MP4
// Developer: Unnat Ramjiayani
// NetID: unnatr2
// This program runs takes two numbers from the user and prints all the semiprime numbers
// between the two numbers. The function "is_prime" checks if the number is prime or not.
// The function "print_semiprimes" checks if the number between the user inputs is a product
// of two prime numbers. If yes, the number is printed. The program also checks for any errors
// like if inputs are negative or if the first input is greater than the second.

#include <stdlib.h>     // Include Standard Library
#include <stdio.h>      // Include Standard I/O

int is_prime(int number);               // Declare function: is_prime
int print_semiprimes(int a, int b);     // Declare function: print_semiprimes

int main(){                             // Function: main
        int a, b;                       // Declare variables
        printf("Input two numbers: ");  // Ask for variables
        scanf("%d %d", &a, &b);         // Get variables from user
        if( a <= 0 || b <= 0 ){         // Check if either a or b are negative or 0
          printf("Inputs should be positive integers\n");       // Print error message
          return 1;                     // Return 1
        }

        if( a > b ){                    // Check if a is greater than b
          printf("The first number should be smaller than or equal to the second number\n");    // Print error message
          return 1;                     // Return 1
        }

        print_semiprimes(a,b);          // Call function: print_semiprimes
        return 0;                       // Return 0
}



/*
 * Function : is_prime
 * Input    : a number
 * Return   : 0 if the number is not prime, else 1
 */
int is_prime(int number){               // Function: is_prime
        int i, result=1;                // Declare variables
        for (i=2;i<number;i++){         // Parse through numbers between 2 and number
                if (number%i==0){       // Check if remainder is 0
                        result=0;       // Is not prime
                }
        }
        return result;                  // Return 0 if not prime, else 1
}


/*
 * Function : print_semiprimes
 * Input    : a, b (a should be smaller than or equal to b)
 * Return   : 0 if there is no semiprime in [a,b], else 1
 */
int print_semiprimes(int a, int b){                                     // Function: print_semiprimes
        int n, fact1, fact2, result=0, check_fact1, check_fact2;        // Declare variables
        for (n=a;n<=b;n++){                                             // Parse through numbers between a and b
                for (fact1=2;fact1<=(n-1);fact1++){                     // Parse through numbers between 2 and n-1
                        check_fact1=is_prime(fact1);                    // Check if fact1 is prime, save in check_fact1
                        if (check_fact1==1&&(n%fact1)==0){              // Check if check_fact1 is prime and a factor of n
                                fact2=n/fact1;                          // Fact2 is n divided by fact1
                                check_fact2=is_prime(fact2);            // Check if fact2 is prime, save in check_fact2
                                if (check_fact2==1){                    // Check if fact2 is prime
                                        printf("%d ",n);                // Print n
                                        result=1;                       // Has semiprimes
                                        break;                          // Break
                                }
                        }
                }
        }
        printf("\n");                                                   // Print new line
        return result;                                                  // Return 0 if no semiprimes in [a,b], else 1
}
