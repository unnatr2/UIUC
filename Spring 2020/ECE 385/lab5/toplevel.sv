module toplevel (
    input   logic           Clk,
    input   logic           Reset,
    input   logic           ClearAX_LoadB,
    input   logic           Run,
    input   logic[7:0]      S,
    output  logic           X,         
    output  logic[6:0]      AhexU,      
    output  logic[6:0]      AhexL,
    output  logic[6:0]      BhexU,
    output  logic[6:0]      BhexL,
    output  logic[7:0]      Aval,
    output  logic[7:0]      Bval
);

    logic [8:0] adder_output;

    logic Shift;
    logic Load_XA;
    logic Load_B;
    logic Sub;
    logic Clear_XA;
    logic Clear_B;

    control_unit control (.Clk(Clk), 
                        .Reset(Reset), 
                        .ClearAX_LoadB(ClearAX_LoadB), 
                        .Run(Run), 
                        .M(Bval[0]), 
                        .Shift(Shift), 
                        .Clear_XA(Clear_XA), 
                        .Clear_B(Clear_B), 
                        .Sub(Sub), 
                        .Load_XA(Load_XA), 
                        .Load_B(Load_B));

    add_sub adder (.SUB(Sub), .A({Aval[7], Aval}), .B({S[7], S}), .Sum(adder_output));    

    shift_reg_8 regA (.parallel_in(adder_output[7:0]), 
                         .serial_input(X), 
                         .right_shift_enable(Shift), 
                         .clear(Clear_XA),
                         .load(Load_XA), 
                         .Clk(Clk), 
                         .parallel_out(Aval));

    shift_reg_8 regB (.parallel_in(S), 
                         .serial_input(Aval[0]), 
                         .right_shift_enable(Shift), 
                         .clear(Clear_B), 
                         .load(Load_B), 
                         .Clk(Clk), 
                         .parallel_out(Bval));
                         
    ff_X regX (.in(adder_output[8]), .Clk(Clk), .load(Load_XA), .clear(Clear_XA), .out(X));

    HexDriver AhexU_inst (.In0(Aval[7:4]), .Out0(AhexU));
    HexDriver AhexL_inst (.In0(Aval[3:0]), .Out0(AhexL));
    HexDriver BhexU_inst (.In0(Bval[7:4]), .Out0(BhexU));
    HexDriver BhexL_inst (.In0(Bval[3:0]), .Out0(BhexL));

endmodule
