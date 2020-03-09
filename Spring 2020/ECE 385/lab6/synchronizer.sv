module synchronizer #(N=16) (
    input logic Clk,
    input logic in,
    output logic out
);

    always_ff @(posedge Clk) begin
        out <= in;
    end
    
endmodule