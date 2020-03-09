onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -label CLOCK -radix binary /testbench/tp1/my_slc/Clk
add wave -noupdate -label STATE /testbench/tp1/my_slc/state_controller/State
add wave -noupdate -label {SRAM Address} -radix hexadecimal /testbench/tp1/ADDR
add wave -noupdate -label {SRAM Data} -radix hexadecimal /testbench/tp1/Data
add wave -noupdate -label {Run (Synchronized)} -radix hexadecimal /testbench/tp1/Run_sync
add wave -noupdate -label {Reset (Synchronized)} -radix hexadecimal /testbench/tp1/Reset_sync
add wave -noupdate -label {Continue (Synchronized)} -radix hexadecimal /testbench/tp1/Continue_sync
add wave -noupdate -label LD_IR -radix hexadecimal /testbench/tp1/my_slc/LD_IR
add wave -noupdate -label IR -radix hexadecimal /testbench/tp1/my_slc/IR
add wave -noupdate -label LD_MAR -radix hexadecimal /testbench/tp1/my_slc/LD_MAR
add wave -noupdate -label MAR -radix hexadecimal /testbench/tp1/my_slc/MAR
add wave -noupdate -label LD_MDR -radix hexadecimal /testbench/tp1/my_slc/LD_MDR
add wave -noupdate -label MDR -radix hexadecimal /testbench/tp1/my_slc/MDR
add wave -noupdate -label LD_PC -radix hexadecimal /testbench/tp1/my_slc/LD_PC
add wave -noupdate -label PC -radix hexadecimal /testbench/tp1/my_slc/PC
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {0 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 167
configure wave -valuecolwidth 67
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ns
update
WaveRestoreZoom {0 ps} {1066584 ps}
