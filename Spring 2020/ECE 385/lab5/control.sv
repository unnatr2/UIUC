module control_unit (
    input logic Clk,
    input logic Reset,
    input logic ClearAX_LoadB,
    input logic Run,
    input logic M,
    output logic Shift,
    output logic Clear_XA,
    output logic Clear_B,
    output logic Sub,
    output logic Load_XA,
    output logic Load_B
);

    enum logic [4:0]  { 
        Done, Initial, Start,
        Add_0, Shift_0,
        Add_1, Shift_1,
        Add_2, Shift_2,
        Add_3, Shift_3,
        Add_4, Shift_4,
        Add_5, Shift_5,
        Add_6, Shift_6,
        Sub_7, Shift_7 
    } state, next_state = Initial;

    always_ff @ (posedge Clk)
    begin
        state <= next_state;
    end

    always_comb begin
        next_state = state;
        
        if (~Reset) begin
            next_state = Initial;
            Shift = 1'b0;
            Clear_XA = 1'b1;
            Clear_B = 1'b1;
            Sub = 1'b0;
            Load_XA = 1'b0;
            Load_B = 1'b0;
        end else begin
            Shift = 1'b0;
            Clear_XA = 1'b0;
            Clear_B = 1'b0;
            Sub = 1'b0;
            Load_XA = 1'b0;
            Load_B = 1'b0;
            unique case (state)
                Initial:
                begin
                    if (~Run) begin // Run is pressed
                        next_state = Start;
                    end else if (~ClearAX_LoadB) begin
                        Load_B = 1'b1;
                        next_state = Initial;
                    end else begin // Run is not pressed
                        next_state = Initial;
                    end
                end
                Start:
                begin
                    Clear_XA = 1'b1;
                    next_state = Add_0;
                end
                Add_0:
                begin
                    if (M) begin
                        Load_XA = 1'b1;
                    end 
                    next_state = Shift_0;
                end
                Shift_0:
                begin
                    Shift = 1'b1;
                    next_state = Add_1;   
                end
                Add_1:
                begin
                    if (M) begin
                        Load_XA = 1'b1;
                    end 
                    next_state = Shift_1;
                end
                Shift_1:
                begin
                    Shift = 1'b1;
                    next_state = Add_2; 
                end
                Add_2:
                begin
                    if (M) begin
                        Load_XA = 1'b1;
                    end 
                    next_state = Shift_2;
                end
                Shift_2:
                begin
                    Shift = 1'b1;
                    next_state = Add_3; 
                end
                Add_3:
                begin
                    if (M) begin
                        Load_XA = 1'b1;
                    end 
                    next_state = Shift_3;
                end
                Shift_3:
                begin
                    Shift = 1'b1;
                    next_state = Add_4;
                end
                Add_4:
                begin
                    if (M) begin
                        Load_XA = 1'b1;
                    end 
                    next_state = Shift_4;
                end
                Shift_4:
                begin
                    Shift = 1'b1;
                    next_state = Add_5;
                end
                Add_5:
                begin
                    if (M) begin
                        Load_XA = 1'b1;
                    end 
                    next_state = Shift_5;
                end
                Shift_5:
                begin
                    Shift = 1'b1;
                    next_state = Add_6;
                end
                Add_6:
                begin
                    if (M) begin
                        Load_XA = 1'b1;
                    end 
                    next_state = Shift_6;
                end
                Shift_6:
                begin
                    Shift = 1'b1;
                    next_state = Sub_7;
                end
                Sub_7:
                begin
                    if (M) begin
                        Load_XA = 1'b1;
                        Sub = 1'b1;
                    end 
                    next_state = Shift_7;
                end
                Shift_7:
                begin
                    Shift = 1'b1;
                    next_state = Done;
                end
                Done:
                begin
                    if (~Run) begin // Run is still pressed
                        next_state = Done;
                    end else begin // Run has been released
                        next_state = Initial;
                    end
                end
            endcase
        end
    end
endmodule