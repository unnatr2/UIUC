module datapath(
    input logic Clk,    //
    input logic LD_MAR, //
	input logic LD_MDR, //
    input logic LD_IR,  //
    input logic LD_BEN, //
    input logic LD_CC,  //
    input logic LD_REG,
    input logic LD_PC,  //
    input logic LD_LED,
    input logic GatePC, //
    input logic GateMDR,    //
	input logic GateALU,    //
	input logic GateMARMUX, //
    input logic [1:0] ALUK,								
	input logic [1:0] PCMUX,   //
	input logic DRMUX,
	input logic SR1MUX,
	input logic SR2MUX,
	input logic ADDR1MUX,
    input logic [1:0] ADDR2MUX,
    input logic Mem_OE,
    input logic Mem_WE,
    input logic MIO_EN,         //
    input logic [15:0] MDR_In,  //
    input logic Reset,          //
    output logic BEN,           //
    output logic [15:0] MAR,   //
    output logic [15:0] MDR,   //
    output logic [15:0] IR,    //
    output logic [15:0] PC    //
);

    // CPU Bus Wire/Logic
    // logic [15:0] MDR is passed in as an input
    // logic [15:0] PC is passed in as an input
    wire [15:0] BUS;

    // Register File Package
    logic [2:0] DRMUX_out, SR1MUX_out;
    logic [15:0] SR2_out, SR1_out;
    logic [2:0] DR_inputs [2];
    logic [2:0] SR1_inputs [2];
    assign DR_inputs = '{ IR[11:9], 3'b111 };
    assign SR1_inputs = '{ IR[11:9], IR[8:6] };
    register_file reg_file(.Clk(Clk), .Reset(Reset), .DATA_IN(BUS), .DR(DRMUX_out), .SR1(SR1MUX_out), .SR2(IR[2:0]), .LD_REG(LD_REG), .SR2_OUT(SR2_out), .SR1_OUT(SR1_out));
    mux #(.WIREWIDTH(3), .NUMINPUTS(2)) DR_multuplexer(.select(DRMUX), .ins(DR_inputs), .out(DRMUX_out));
    mux #(.WIREWIDTH(3), .NUMINPUTS(2)) SR1_multiplexer(.select(SR1MUX), .ins(SR1_inputs), .out(SR1MUX_out));

    // ALU Package
    logic [15:0] SR2MUX_out;
    logic [15:0] SR2_inputs [2];
    logic [15:0] ALU_out;
    assign SR2_inputs = '{ SR2_out, {{11{IR[4]}}, IR[4:0]} };
    alu ALU_unit(.A(SR1_out), .B(SR2MUX_out), .ALUK(ALUK), .OUT(ALU_out));
    mux #(.WIREWIDTH(16), .NUMINPUTS(2)) SR2_multiplexer(.select(SR2MUX), .ins(SR2_inputs), .out(SR2MUX_out));
    
    // Branch Enable (BEN) Package
    logic next_ben;
    logic [2:0] nzp, next_nzp;
    assign next_nzp = { BUS[15], BUS == 16'b0, ~BUS[15] }; // Check MSB for N/P, and compare against 0 for Z 
    assign next_ben = (IR[11] & nzp[0]) | (IR[10] & nzp[1]) | (IR[9] & nzp[2]); // See state 32 of control flow doc
    register #(.N(3)) nzp_reg(.Clk(Clk), .data_in(next_nzp), .data_out(nzp), .load_enable(LD_CC), .clear(Reset));
    register #(.N(1)) ben_reg(.Clk(Clk), .data_in(next_ben), .data_out(BEN), .load_enable(LD_BEN), .clear(Reset));
    
    // Address Adder Package
    logic [15:0] ADDR_out, ADDR1_out, ADDR2_out;
    logic [15:0] ADDR2_inputs [4];
	 logic [15:0] ADDR1_inputs [2];
    assign ADDR_out = ADDR1_out + ADDR2_out;
    assign ADDR1_inputs = '{ PC, SR1_out };
    assign ADDR2_inputs = '{ 16'b0, {{10{IR[5]}}, IR[5:0]}, {{7{IR[8]}}, IR[8:0]}, {{5{IR[10]}}, IR[10:0]} };
    mux #(.WIREWIDTH(16), .NUMINPUTS(2)) ADDR1_multiplexer(.select(ADDR1MUX), .ins(ADDR1_inputs), .out(ADDR1_out));
    mux #(.WIREWIDTH(16), .NUMINPUTS(4)) ADDR2_multiplexer(.select(ADDR2MUX), .ins(ADDR2_inputs), .out(ADDR2_out));

    // PC Package
    logic [15:0] PCMUX_out;
    logic [15:0] PCMUX_inputs [3];
    assign PCMUX_inputs = '{ PC + 1, BUS, ADDR_out };
    register #(.N(16)) PC_reg(.Clk(Clk), .data_in(PCMUX_out), .data_out(PC), .load_enable(LD_PC), .clear(Reset));
    mux #(.WIREWIDTH(16), .NUMINPUTS(3)) PC_multiplexer(.select(PCMUX), .ins(PCMUX_inputs), .out(PCMUX_out));
    
    // IR Package
    register #(.N(16)) IR_reg(.Clk(Clk), .data_in(BUS), .data_out(IR), .load_enable(LD_IR), .clear(Reset));
    
    // MDR Package;
    logic [15:0] MDRMUX_out, MDR_out;
    logic [15:0] MDRMUX_inputs [2];
    assign MDRMUX_inputs = '{ BUS, MDR_In };
    register #(.N(16)) MDR_reg(.Clk(Clk), .data_in(MDRMUX_out), .data_out(MDR_out), .load_enable(LD_MDR), .clear(Reset));
    mux #(.WIREWIDTH(16), .NUMINPUTS(2)) MDR_multiplexer(.select(MIO_EN), .ins(MDRMUX_inputs), .out(MDRMUX_out));

    // MAR Package
    logic [15:0] MAR_out;
    register #(.N(16)) MAR_reg(.Clk(Clk), .data_in(BUS), .data_out(MAR_out), .load_enable(LD_MAR), .clear(Reset));

    // Bus & Tristate Buffer Package
    logic [15:0] busGateInputs [4];
    logic [2:0] busSelect;
    assign busGateInputs = '{ PC, MDR, ALU_out, ADDR_out };
    assign busSelect = { ~GatePC & ~GateMDR, ~GateALU & ~GatePC };
    mux #(.WIREWIDTH(16), .NUMINPUTS(4)) busGate_multiplexer(.select(busSelect), .ins(busGateInputs), .out(BUS));

    // Pause Package
    logic [15:0] FMARMUX_inputs [2];
    logic [15:0] FMDRMUX_inputs [2];
    assign FMARMUX_inputs = '{ MAR_out, 16'hFFFF };
    assign FMDRMUX_inputs = '{ MDR_out, IR };
    mux #(.WIREWIDTH(16), .NUMINPUTS(2)) FMAR_multiplexer(.select(LD_LED), .ins(FMARMUX_inputs), .out(MAR));
    mux #(.WIREWIDTH(16), .NUMINPUTS(2)) FMDR_multiplexer(.select(LD_LED), .ins(FMDRMUX_inputs), .out(MDR));
    
endmodule
