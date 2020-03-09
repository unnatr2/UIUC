module ripple_adder
(
    input   logic[15:0]     A,
    input   logic[15:0]     B,
    /* ----- */
    output  logic[15:0]     Sum,
    output  logic           CO
);

    /* TODO
     *
     * Insert code here to implement a ripple adder.
     * Your code should be completly combinational (don't use always_ff or always_latch).
     * Feel free to create sub-modules or other files. */

    logic C0, C1, C2;

    four_bit_ra FRA0(.x(A[3:0]),   .y(B[3:0]),   .cin(0),   .s(Sum[3:0]),   .cout(C0));
    four_bit_ra FRA1(.x(A[7:4]),   .y(B[7:4]),   .cin(C0),  .s(Sum[7:4]),   .cout(C1));
    four_bit_ra FRA2(.x(A[11:8]),  .y(B[11:8]),  .cin(C1),  .s(Sum[11:8]),  .cout(C2));
    four_bit_ra FRA3(.x(A[15:12]), .y(B[15:12]), .cin(C2),  .s(Sum[15:12]), .cout(CO));
     
endmodule

module four_bit_ra  
(
    input logic [3:0] x,
    input logic [3:0] y,
    input logic cin,
    /* ----- */
    output logic [3:0] s,
    output logic cout
);
                    
    logic c0, c1, c2;

    full_adder fa0(.x(x[0]), .y(y[0]), .cin(cin), .s(s[0]), .cout(c0));
    full_adder fa1(.x(x[1]), .y(y[1]), .cin(c0),  .s(s[1]), .cout(c1));
    full_adder fa2(.x(x[2]), .y(y[2]), .cin(c1),  .s(s[2]), .cout(c2));
    full_adder fa3(.x(x[3]), .y(y[3]), .cin(c2),  .s(s[3]), .cout(cout));

endmodule

module full_adder 
(
    input x,
    input y,
    input cin,
    /* ----- */
    output logic s,
    output logic cout
);

    assign s = x ^ y ^ cin;
    assign cout = (x & y) | (y & cin) | (cin & x);

endmodule

