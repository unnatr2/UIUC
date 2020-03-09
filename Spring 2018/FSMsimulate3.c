/* tab:8
 *
 * FSMsimulate.c - a simple FSM simulator
 *
 * "Copyright (c) 2016 by Steven S. Lumetta."
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written
agreement is
 * hereby granted, provided that the above copyright notice and the
following
 * two paragraphs appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE AUTHOR OR THE UNIVERSITY OF ILLINOIS BE LIABLE
TO
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
 * DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
DOCUMENTATION,
 * EVEN IF THE AUTHOR AND/OR THE UNIVERSITY OF ILLINOIS HAS BEEN ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE AUTHOR AND THE UNIVERSITY OF ILLINOIS SPECIFICALLY DISCLAIM ANY
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE
 * PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER THE AUTHOR NOR
 * THE UNIVERSITY OF ILLINOIS HAS ANY OBLIGATION TO PROVIDE MAINTENANCE,
 * SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Author: Steve Lumetta
 * Version: 2
 * Creation Date: 27 October 2016
 * Filename: FSMsimulate.c
 * History:
 *      SL      1       27 October 2016
 * First written.
 *      SL      2       3 November 2016
 * Removed stdint types.
 */
#include <stdint.h>
#include <stdio.h>
int main () {
    unsigned int S2; /* current FSM state */
    unsigned int S1;
    unsigned int S0;
    unsigned int S2_plus; /* FSM state in next clock cycle */
    unsigned int S1_plus;
    unsigned int S0_plus;
    unsigned int A; /* outputs A and P */
    unsigned int P;
    unsigned int TBits; /* input T as a bit vector (LSB first) */
    unsigned int T; /* input T in the current cycle */
    unsigned int cycle; /* cycle number (discrete time) */
    /* Initialize input bit vector. */
    TBits = 0x44BC;
    /* Force FSM into state 000 initially. */
    S2 = 0;
    S1 = 0;
    S0 = 0;
    /* Print a header for the output table. */
    printf (" cycle | current state | outputs | input | next state\n");
    printf ("number |    S2 S1 S0   |  A   P  |   T   | S2+ S1+ S0+\n");
    printf ("-------+---------------+---------+-------+-------------\n");
    for (cycle = 0; 15 > cycle; cycle = cycle + 1) {
        /*
         * Copy the next bit from the input vector into T, and
         * discard the bit copied from the vector by shifting
         * it to the right.
         */
        T = (TBits & 1);
        TBits = (TBits >> 1);
        /*
         * Calculate the outputs.  Discard all but the low bit from
         * the logic expressions by bitwise ANDing with 1.
         */
        A = ((~(~(~S2|S0)|~(S2|S1)))&1);
        P = ((~(~(~S2|~S1)|~(S2|~S0)))&1);
        /*
         * Calculate the next state.  Discard all but the low bit from
         * the logic expressions by bitwise ANDing with 1.
         */
        S2_plus = ((~(T|T))&1);
        S1_plus = ((~(S2|~S0))&1);
        S0_plus = ((~(~S2|~S1))&1);
        /* Print the current state, outputs, input, and next state. */
        printf (" %3d   |     %d  %d  %d   |  %d   %d  |   %d   |  %d  %d  %d\n",cycle, S2, S1, S0, A, P, T, S2_plus, S1_plus, S0_plus);
        /* Simulate the rising edge of the clock. */
        S2 = S2_plus;
        S1 = S1_plus;
        S0 = S0_plus;
    }
    /* Success! */
    return 0;
}
