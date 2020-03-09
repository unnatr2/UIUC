module register #(N=16) (
    input logic Clk,
    input logic [N-1:0] data_in,
    input logic load_enable,
    input logic clear,
    output logic [N-1:0] data_out
);
    logic [N-1:0] next_data;
    
    always_ff @ (posedge Clk)begin
        data_out <= next_data;
    end

    always_comb begin
        next_data = data_out;
        if (load_enable) begin
            next_data = data_in;
        end else if (clear) begin
            next_data = {N{1'b0}};
        end
    end

endmodule