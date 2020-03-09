module add_sub(
    // Perform subtraction when SUB is high
    input	logic	       SUB,
    input   logic[8:0]     A,
    input   logic[8:0]     B,
    output  logic[8:0]     Sum
);
    logic CIN;
    logic C0, C1;
    logic [8:0] B_conditional;

    three_bit_ra TRA0(.x(A[2:0]), .y(B_conditional[2:0]), .s(Sum[2:0]), .cin(CIN), .cout(C0));
    three_bit_ra TRA1(.x(A[5:3]), .y(B_conditional[5:3]), .s(Sum[5:3]), .cin(C0), .cout(C1));
    three_bit_ra TRA2(.x(A[8:6]), .y(B_conditional[8:6]), .s(Sum[8:6]), .cin(C1), .cout());

    // Compute subtraction by negating B, else leave it and do addition.
    always_comb begin
        if (SUB) begin
            B_conditional = ~B;
            CIN = 1;
        end else begin
            B_conditional = B;
            CIN = 0;
        end
    end
     
endmodule

module three_bit_ra (
    input logic [2:0] x,
    input logic [2:0] y,
    input logic cin,
    output logic [2:0] s,
    output logic cout
);              
    logic c0, c1;
    full_adder fa0(.x(x[0]), .y(y[0]), .s(s[0]), .cin(cin), .cout(c0));
    full_adder fa1(.x(x[1]), .y(y[1]), .s(s[1]), .cin(c0), .cout(c1));
    full_adder fa2(.x(x[2]), .y(y[2]), .s(s[2]), .cin(c1), .cout(cout));

endmodule

module full_adder (
    input x,
    input y,
    input cin,
    output logic s,
    output logic cout
);
    assign s = x ^ y ^ cin;
    assign cout = (x & y) | (y & cin) | (cin & x);
    
endmodule
