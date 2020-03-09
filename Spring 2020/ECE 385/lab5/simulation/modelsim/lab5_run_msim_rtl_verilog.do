transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -sv -work work +incdir+C:/Users/unnat/Desktop/ECE\ 385/lab5 {C:/Users/unnat/Desktop/ECE 385/lab5/toplevel.sv}
vlog -sv -work work +incdir+C:/Users/unnat/Desktop/ECE\ 385/lab5 {C:/Users/unnat/Desktop/ECE 385/lab5/shift_register.sv}
vlog -sv -work work +incdir+C:/Users/unnat/Desktop/ECE\ 385/lab5 {C:/Users/unnat/Desktop/ECE 385/lab5/HexDriver.sv}
vlog -sv -work work +incdir+C:/Users/unnat/Desktop/ECE\ 385/lab5 {C:/Users/unnat/Desktop/ECE 385/lab5/control.sv}
vlog -sv -work work +incdir+C:/Users/unnat/Desktop/ECE\ 385/lab5 {C:/Users/unnat/Desktop/ECE 385/lab5/add_sub.sv}

vlog -sv -work work +incdir+C:/Users/unnat/Desktop/ECE\ 385/lab5 {C:/Users/unnat/Desktop/ECE 385/lab5/testbench.sv}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cycloneive_ver -L rtl_work -L work -voptargs="+acc"  testbench

add wave *
view structure
view signals
run 1000 ns
