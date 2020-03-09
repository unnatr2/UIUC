module testbench();

    // Define time constraints
	timeunit 10ns;
    timeprecision 1ns;
    
    // Reserved input names for toplevel
    logic           Clk = 0;
    logic           Reset;
    logic           ClearAX_LoadB;
    logic           Run;
    logic [7:0]     S;
    logic           X;         
    logic [6:0]     AhexU;      
    logic [6:0]     AhexL;
    logic [6:0]     BhexU;
    logic [6:0]     BhexL;
    logic [7:0]     Aval;
    logic [7:0]     Bval;

    integer ERR = 0;

    logic [15:0] answer;

    toplevel tp(
        .Clk(Clk), 
        .Reset(Reset), 
        .ClearAX_LoadB(ClearAX_LoadB), 
        .Run(Run), 
        .S(S), 
        .X(X),
        .AhexU(AhexU),
        .AhexL(AhexU),
        .BhexL(BhexL),
        .BhexU(BhexU),
        .Aval(Aval),
        .Bval(Bval)
    );

    // Run the clock
    always begin: CLOCK_GENERATION
        #1 Clk = ~Clk;
    end

    // Initialize the clock
    initial begin: CLOCK_INITIALIZATION
        Clk = 0;
    end
    
    
    initial begin: TESTS
        // Initial Button states
        #2 

        Reset = 1;
        ClearAX_LoadB = 1;
        Run = 1;
        S = 0;

        #2 Reset = 0;
        #2 Reset = 1;

        // Loads b and clears XA
        #2 S = 8'b1100_0101;
        #2 ClearAX_LoadB = 0;
        #2 ClearAX_LoadB = 1;

        #2 S = 8'b0000_0111;

        #2 Run = 0;
        #2 Run = 1;

        #50

        #2 answer = {Aval, Bval};

        if (answer != 16'b1111_1110_0110_0011) ERR++;

        if (ERR == 0) $display("All tests passed.");
        else $display("%d error(s) detected.", ERR);
        
    end
endmodule