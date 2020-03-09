#**************************************************************
# Create Clock (where ‘Clk’ is the user-defined system clock name)
#**************************************************************
create_clock -name {Clk} -period 20ns -waveform {0.000 5.000}
[get_ports {Clk}]
#creates a clock, applies it to all ports named “Clk” in toplevel
#note: -waveform specifies duty cycle, in this case 50%