all: sw_project

open:
	xsdk -workspace ./project_sw &

hw_import:
	if [ -a ../hw_project/project_hw/project_hw.sdk/accelerator_project_wrapper.hdf ]; then rm -f hwdef.hdf; cp ../hw_project/project_hw/project_hw.sdk/accelerator_project_wrapper.hdf hwdef.hdf; fi;
	if [ -a ../hw_project/project_hw/project_hw.sdk/accelerator_project_wrapper.hdf ]; then rm -rf ../hw_project/project_hw/project_hw.sdk; fi;
	
sw_project:
	xsdk -batch -source src/tcl/sdk.tcl
	cp -f project_sw/sw_design/src/lscript.ld src/sw/.
	cp -f project_sw/sw_design/src/Xilinx.spec src/sw/.
	rm -rf project_sw/sw_design/src
	ln -s ../../src/sw/ project_sw/sw_design/src
		
clean:
	rm -rf vivado.* vivado_* .Xil/ webtalk* project_sw