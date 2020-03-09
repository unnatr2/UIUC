module carry_lookahead_adder
(
    input   logic[15:0]     A,
    input   logic[15:0]     B,
    output  logic[15:0]     Sum,
    output  logic           CO
);

    /* TODO
     *
     * Insert code here to implement a CLA adder.
     * Your code should be completly combinational (don't use always_ff or always_latch).
     * Feel free to create sub-modules or other files. */

    logic C4, C8, C12,
          Pout0, Pout4, Pout8, Pout12,
          Gout0, Gout4, Gout8, Gout12,
          PG, GG;

    carry_lookahead_adder_four cla0(.a(A[3:0]),   .b(B[3:0]),   .cin(0),   .s(Sum[3:0]),   .cout(), .pout(Pout0),  .gout(Gout0));
    carry_lookahead_adder_four cla1(.a(A[7:4]),   .b(B[7:4]),   .cin(C4),  .s(Sum[7:4]),   .cout(), .pout(Pout4),  .gout(Gout4));
    carry_lookahead_adder_four cla2(.a(A[11:8]),  .b(B[11:8]),  .cin(C8),  .s(Sum[11:8]),  .cout(), .pout(Pout8),  .gout(Gout8));
    carry_lookahead_adder_four cla3(.a(A[15:12]), .b(B[15:12]), .cin(C12), .s(Sum[15:12]), .cout(), .pout(Pout12), .gout(Gout12));
    carry_lookahead_unit_four clu(.cin(0), .p0(Pout0), .p1(Pout4), .p2(Pout8), .p3(Pout12), .g0(Gout0), .g1(Gout4), .g2(Gout8), .g3(Gout12), .c1(C4), .c2(C8), .c3(C12), .cout(CO), .pout(PG), .gout(GG));
     
endmodule

module full_adder_pg_one
(
    input logic a,
    input logic b,
    input logic z,
    /* ----- */
    output logic s,
    output logic p,
    output logic g
);

    assign s = a ^ b ^ z;
    assign p = a ^ b;
    assign g = a & b;
    
endmodule


module carry_lookahead_unit_four
(
    input logic cin,
    input logic p0,
    input logic p1,
    input logic p2,
    input logic p3,
    input logic g0,
    input logic g1,
    input logic g2,
    input logic g3,
    /* ----- */
    output logic c1,
    output logic c2,
    output logic c3,
    output logic cout,
    output logic pout,
    output logic gout
);

    //TODO: Change to non-blocking assignment for speed since no interdependence
    assign c1 = (cin & p0) | g0;
    assign c2 = (cin & p0 & p1) | (g0 & p1) | g1;
    assign c3 = (cin & p0 & p1 & p2) | (g0 & p1 & p2) | (g1 & p2) | g2;
    assign cout = (cin & p0 & p1 & p2 & p3) | (g0 & p1 & p2 & p3) | (g1 & p2 & p3) | (g2 & p3) | g3;    
    assign pout = p0 & p1 & p2 & p3;
    assign gout = g3 | (g2 & p3) | (g1 & p3 & p2) | (g0 & p3 & p2 & p1);
    
endmodule

module carry_lookahead_adder_four
(
    input logic [3:0] a,
    input logic [3:0] b,
    input logic cin,
    /* ----- */
    output logic [3:0] s,
    output logic cout,
    output logic pout,
    output logic gout
);

logic c1, c2, c3,
      p0, p1, p2, p3,
      g0, g1, g2, g3;
      
full_adder_pg_one fa0(.a(a[0]), .b(b[0]), .z(cin), .s(s[0]), .p(p0), .g(g0));
full_adder_pg_one fa1(.a(a[1]), .b(b[1]), .z(c1),  .s(s[1]), .p(p1), .g(g1));
full_adder_pg_one fa2(.a(a[2]), .b(b[2]), .z(c2),  .s(s[2]), .p(p2), .g(g2));
full_adder_pg_one fa3(.a(a[3]), .b(b[3]), .z(c3),  .s(s[3]), .p(p3), .g(g3));
carry_lookahead_unit_four lu(.cin(cin), .p0(p0), .p1(p1), .p2(p2), .p3(p3), .g0(g0), .g1(g1), .g2(g2), .g3(g3), .c1(c1), .c2(c2), .c3(c3), .cout(cout), .pout(pout), .gout(gout));

endmodule
