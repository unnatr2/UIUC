module shift_reg_8 (
    input logic [7:0] parallel_in,
    input logic       serial_input,
    input logic       right_shift_enable,
    input logic       clear,
    input logic       load,
    input logic       Clk,
    output logic [7:0] parallel_out
);
    logic [7:0] next;

    always_ff @ (posedge Clk) begin
        parallel_out <= next;
    end

    always_comb begin
        next = parallel_out;
        if(clear) begin
            next = 8'b0;
        end else if (right_shift_enable) begin
            next = { serial_input, parallel_out[7:1] };
        end else if (load) begin
            next = parallel_in;
        end
    end

endmodule

module ff_X (
    input logic in,
    input logic Clk,
    input logic load,
    input logic clear,
    output logic out
);

    always_ff @ (posedge Clk) begin
        out <= out;
        if(clear) begin
            out <= 1'b0;
        end else if (load) begin
            out <= in;
        end
    end

endmodule