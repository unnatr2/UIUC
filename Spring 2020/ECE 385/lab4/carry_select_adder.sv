module carry_select_adder
(
    input   logic[15:0]     A,
    input   logic[15:0]     B,
    output  logic[15:0]     Sum,
    output  logic           CO
);

    /* TODO
     *
     * Insert code here to implement a carry select.
     * Your code should be completly combinational (don't use always_ff or always_latch).
     * Feel free to create sub-modules or other files. */

    logic C4, C8, C12;

    four_bit_ra FRA0(.x(A[3:0]), .y(B[3:0]), .cin(0), .s(Sum[3:0]), .cout(C4));
    carry_select_unit_four CSU1(.a(A[7:4]),   .b(B[7:4]),   .cin(C4),  .sum(Sum[7:4]),   .cout(C8));
    carry_select_unit_four CSU2(.a(A[11:8]),  .b(B[11:8]),  .cin(C8),  .sum(Sum[11:8]),  .cout(C12));
    carry_select_unit_four CSU3(.a(A[15:12]), .b(B[15:12]), .cin(C12), .sum(Sum[15:12]), .cout(CO));

endmodule

module carry_select_unit_four
(
    input logic [3:0] a,
    input logic [3:0] b,
    input logic cin,
    output logic [3:0] sum,
    output logic cout
);
    logic c0, c1;
    logic [3:0] withoutCarry, withCarry;
    
    four_bit_ra FRA0(.x(a[3:0]), .y(b[3:0]), .cin(0), .s(withoutCarry[3:0]),  .cout(c0));
    four_bit_ra FRA1(.x(a[3:0]), .y(b[3:0]), .cin(1), .s(withCarry[3:0]),     .cout(c1));
    
    assign cout = (c1 & cin) | c0;

    two_to_one_mux mux(.S0(withoutCarry[3:0]), .S1(withCarry[3:0]), .SEL(cin), .OUT(sum[3:0]));

endmodule

module two_to_one_mux
(
    input logic [3:0] S0,
    input logic [3:0] S1,
    input logic SEL,
    /* ----- */
    output logic [3:0] OUT
);

    assign OUT[0] = (S1[0] & SEL) | (S0[0] & ~SEL);
    assign OUT[1] = (S1[1] & SEL) | (S0[1] & ~SEL);
    assign OUT[2] = (S1[2] & SEL) | (S0[2] & ~SEL);
    assign OUT[3] = (S1[3] & SEL) | (S0[3] & ~SEL);

endmodule