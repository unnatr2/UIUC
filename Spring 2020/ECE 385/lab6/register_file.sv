module register_file (
    input logic Clk,
    input logic Reset,
    input logic [15:0] DATA_IN,
    input logic [2:0] DR,
    input logic [2:0] SR1,
    input logic [2:0] SR2,
    input logic LD_REG,
    output logic [15:0] SR2_OUT,
    output logic [15:0] SR1_OUT
);

    // Select logic for registers
    logic LD_R0, LD_R1, LD_R2, LD_R3, LD_R4, LD_R5, LD_R6, LD_R7;
    always_comb begin
        LD_R0 = 0;
        LD_R1 = 0;
        LD_R2 = 0;
        LD_R3 = 0;
        LD_R4 = 0;
        LD_R5 = 0;
        LD_R6 = 0;
        LD_R7 = 0;
        unique case (DR) 
            3'd0: LD_R0 = LD_REG;
            3'd1: LD_R1 = LD_REG;
            3'd2: LD_R2 = LD_REG;
            3'd3: LD_R3 = LD_REG;
            3'd4: LD_R4 = LD_REG;
            3'd5: LD_R5 = LD_REG;
            3'd6: LD_R6 = LD_REG;
            3'd7: LD_R7 = LD_REG;
        endcase
    end

    // Registers R0-7
    logic [15:0] regs [8];
    register #(.N(16)) R0_reg(.Clk(Clk), .data_in(DATA_IN), .load_enable(LD_R0), .clear(Reset), .data_out(regs[0]));
    register #(.N(16)) R1_reg(.Clk(Clk), .data_in(DATA_IN), .load_enable(LD_R1), .clear(Reset), .data_out(regs[1]));
    register #(.N(16)) R2_reg(.Clk(Clk), .data_in(DATA_IN), .load_enable(LD_R2), .clear(Reset), .data_out(regs[2]));
    register #(.N(16)) R3_reg(.Clk(Clk), .data_in(DATA_IN), .load_enable(LD_R3), .clear(Reset), .data_out(regs[3]));
    register #(.N(16)) R4_reg(.Clk(Clk), .data_in(DATA_IN), .load_enable(LD_R4), .clear(Reset), .data_out(regs[4]));
    register #(.N(16)) R5_reg(.Clk(Clk), .data_in(DATA_IN), .load_enable(LD_R5), .clear(Reset), .data_out(regs[5]));
    register #(.N(16)) R6_reg(.Clk(Clk), .data_in(DATA_IN), .load_enable(LD_R6), .clear(Reset), .data_out(regs[6]));
    register #(.N(16)) R7_reg(.Clk(Clk), .data_in(DATA_IN), .load_enable(LD_R7), .clear(Reset), .data_out(regs[7]));

    // Drive output registers
    assign SR1_OUT = regs[SR1];
    assign SR2_OUT = regs[SR2];

endmodule