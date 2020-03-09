module mux #(WIREWIDTH=16, NUMINPUTS=2) (
    input logic [WIREWIDTH-1:0] ins [NUMINPUTS],
    input logic [$clog2(NUMINPUTS)-1:0] select,
    output logic [WIREWIDTH-1:0] out
);

    always_comb begin
        if (select >= NUMINPUTS) begin
            out = {WIREWIDTH{1'bZ}};
        end else begin
            out = ins[select];
        end
    end

endmodule