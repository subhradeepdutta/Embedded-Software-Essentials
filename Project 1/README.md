#***************************************************************************************************
#	Date : 02/12/2017
#	ECEN 5013 Spring 2017
#	Team: Sameer Vaze & Subhradeep Dutta
#***************************************************************************************************
#Description:
#	This makefile has a host of recipes listed below
#	which supports three different architectures 
#	namely the BeagleBone, the host Linux machine
#	and the FRDM KL-25z. The makefile supports a host
#	of utlities for each of these architectures
#***************************************************************************************************
#
#***************************************************************************************************
#Build Instructions for the makefile:
#***************************************************************************************************
#For the host Linux VM(Default option):
#
#make - This is the default option which builds the executable for the Linux VM
#make build - This is the default option which builds the executable for the Linux VM
#make compile-all - This compiles all the files to create the object files but does not link them
#make disp_size - The recipe displays the size for each section of the executable and the total size
#make disp_nm - recipe lists all the symbols from the object files
#make build_lib - creates a library from the object files
#make build_dep - recipe generates the dependencies for all the files
#make objectdump - This recipe disassembles the executable back to assembly code and stores it in the project1.asm file
#make clean - The clean recipe removes all the generated files
#
#***************************************************************************************************
#For the FRDM KL-25z
#***************************************************************************************************
#make PLATFORM=FRDM
#make build PLATFORM=FRDM
#make compile-all PLATFORM=FRDM  - This compiles all the files to create the object files but does not link them
#make disp_size PLATFORM=FRDM  - The recipe displays the size for each section of the executable and the total size
#make disp_nm PLATFORM=FRDM  - recipe lists all the symbols from the object files
#make build_lib PLATFORM=FRDM- creates a library from the object files
#make build_dep PLATFORM=FRDM - recipe generates the dependencies for all the files
#make objectdump PLATFORM=FRDM - This recipe disassembles the executable back to assembly code and stores it in the project1.asm file
#make clean PLATFORM=FRDM The clean recipe removes all the generated files
#
#***************************************************************************************************
#For the BeagleBone
#***************************************************************************************************
#make PLATFORM=BBB
#make build  PLATFORM=BBB
#make compile-all  PLATFORM=BBB - This compiles all the files to create the object files but does not link them
#make disp_size PLATFORM=BBB  - The recipe displays the size for each section of the executable and the total size
#make disp_nm PLATFORM=BBB  - recipe lists all the symbols from the object files
#make build_lib PLATFORM=BBB- creates a library from the object files
#make build_dep PLATFORM=BBB - recipe generates the dependencies for all the files
#make objectdump PLATFORM=BBB -This recipe disassembles the executable back to assembly code and stores it in the project1.asm file
#make clean PLATFORM=BBB -The clean recipe removes all the generated files
#make build_transfer PLATFORM=BBB - This builds an executable for the BeagleBone and transfers it to the device
#***************************************************************************************************
#
