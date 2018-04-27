all: ipcore hw_project	
	
hw_project:
	vivado -mode batch -source tcl/build.tcl
	
ipcore:
	vivado -mode batch -source tcl/ipcore_accelerator_interface.tcl
	cp -rf src/ip_repo/axis_to_bram_1.0 project_ipcores/.
	cp -rf src/ip_repo/testbench_axilite_master_1.0 project_ipcores/.

open:
	vivado project_hw/project_hw.xpr -tempDir /tmp &

clean:
	rm -rf vivado.* vivado_* .Xil/ webtalk* -f
	rm -rf project_hw project_ipcores