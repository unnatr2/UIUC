module testbench();
    // Define time constraints
	timeunit 10ns;
    timeprecision 1ns;

    logic [15:0] S;
    logic Clk, Reset, Run, Continue;
    logic [11:0] LED;
    logic [6:0] HEX0, HEX1, HEX2, HEX3, HEX4, HEX5, HEX6, HEX7;
    logic CE, UB, LB, OE, WE;
    logic [19:0] ADDR;
    wire [15:0] Data;

    // integer ERR = 0;

    // Run the clock
    always begin: CLOCK_GENERATION
        #1 Clk = ~Clk;
    end

    // Initialize the clock
    initial begin: CLOCK_INITIALIZATION
        Clk = 0;
    end
    
    // logic [1:0] mux_ins [3];
    // logic [1:0] mux_out;
    // logic [1:0] mux_select;
    // mux #(.WIREWIDTH(2), .NUMINPUTS(3)) mdrmux(.ins(mux_ins), .out(mux_out), .select(mux_select));

    lab6_toplevel tp1 (.*);
    
    initial begin: TESTS
        // Initial Button states
        Reset = 1;
        Run = 1;
        Continue = 1;
        S = 16'h0003;

        // Start the testing sequence
        #2 Reset = 0;
        #2 Reset = 1;

        #2 Run = 0;
        #2 Run = 1;
        
        #10 Continue = 0;
        #1 Continue = 1;

        #10 Continue = 0;
        #1 Continue = 1;

        #10 Continue = 0;
        #1 Continue = 1;

        #50;

        // #10 begin: TEST_MUX
        //     mux_ins[0] = 2'b00;
        //     mux_ins[1] = 2'b01;
        //     mux_ins[2] = 2'b10;

        //     #2 mux_select = 2'b00;
        //     #2 if(mux_out != 2'b00) begin
        //         ERR++;
        //         $display("First mux input not correctly selected.");
        //     end
            
        //     #2 mux_select = 2'b01;
        //     #2 if(mux_out != 2'b01) begin
        //         ERR++;
        //         $display("Second mux input not correctly selected.");
        //     end

        //     #2 mux_select = 2'b10;
        //     #2 if(mux_out != 2'b10) begin
        //         ERR++;
        //         $display("Third mux input not correctly selected.");
        //     end

        //     #2 mux_select = 2'b11;
        //     #2 if(mux_out != 2'bxx) begin
        //         ERR++;
        //         $display("Fourth mux input not correctly selected.");
        //     end
        // end
        
        // #50 if (ERR == 0) $display("All tests passed.");
        // else $display("%d error(s) detected.", ERR);
        
    end
endmodule