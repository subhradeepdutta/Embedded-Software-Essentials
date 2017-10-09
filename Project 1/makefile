#	Date : 02/12/2017
#	ECEN 5013 Spring 2017
#	Team: Sameer Vaze & Subhradeep Dutta

#Description:
#	This makefile has a host of recipes listed below
#	which supports three different architectures 
#	namely the BeagleBone, the host Linux machine
#	and the FRDM KL-25z. The makefile supports a host
#	of utlities for each of these architectures

#Include the sources.mk file
include sources.mk

#shell command to detect the current architecture of the system on which the program is running	
DEVICE:=$(shell arch)

#CC defines the appropriate compiler depedning upon the platform and architecture
#CFLAGS defines the compiler flags for various architectures
#SIZE defines the flags for using the size option
#NM defines the command for listing symbols
#DMP defines the command for object dump

#If the platform is FRDM KL25z then set flags and compiler accordingly
ifeq ($(PLATFORM),FRDM)
	CC=arm-none-eabi-gcc
	CFLAGS = -Wall -Werror -g -O0 -mthumb -mcpu=cortex-m0plus -std=c99 --specs=nosys.specs -DKL25Z_PLATFORM -DPROJECT1 #-DTYPE_1
	SIZE = arm-none-eabi-size -B $(TARGET)
	NM = arm-none-eabi-nm $(TARGET)
	DMP = arm-none-eabi-objdump

#If the platform is BBB then need to decide if the program is running natively on the BeagleBone or the host machine
#To make that decision we use the DEVICE variable which reads the architecture on which the program is running
#if the device is x86_64 then we determine that this is the host machine
#else we assume that it is the beaglebone and set flags and compilers accordingly
else
	ifeq ($(PLATFORM),BBB)
		ifeq ($(DEVICE),x86_64)
			CC=arm-linux-gnueabihf-gcc
			CFLAGS = -Wall -Werror -g -O0 -mthumb -march=armv7-a -mfloat-abi=hard -mfpu=vfpv3-d16 -mcpu=cortex-a8 -std=c99 -DPROJECT1 #-DTYPE_1
			SIZE = arm-linux-gnueabihf-size -B $(TARGET)
			NM = arm-linux-gnueabihf-nm $(TARGET)
			DMP = arm-linux-gnueabihf-objdump

		else
			CC=gcc
			CFLAGS = -Wall -Werror -g -O0 -mthumb -march=armv7-a -mfloat-abi=hard -mfpu=vfpv3-d16 -mcpu=cortex-a8 -std=c99  -DPROJECT1 #-DTYPE_1
			SIZE = size -B $(TARGET)
			NM = nm $(TARGET)
			DMP = objdump

		endif

	else
		CC=gcc
		CFLAGS = -Wall -Werror -g -O0 -std=c99 #-DPROJECT1 #-DTYPE_1 #-DKL25Z_PLATFORM
		SIZE = size -B $(TARGET)
		NM = nm $(TARGET)
		DMP = objdump

	endif

endif
	

ifeq ($(PROJ),OFF)
	CFLAGS += #-DPROJECT1
else
	CFLAGS += -DPROJECT1

endif
		

#Define flag to generate the map file for the executable
LDFLAGS= -Xlinker -Map=project1.map

#This is the default target which has the object files as its prerequisites
$(TARGET):$(OBJS) #| $(CODE_TO_PPR) $(PPR_TO_ASM) $(ASM_TO_OBJS)
	$(CC) $(CFLAGS) $(DIR) $(LDFLAGS) -o $(TARGET) $(OBJS)
	$(SIZE)

#This is the recipe to generate preprocessed files from the respective .c files
%.i:%.c 
	$(CC) $(CFLAGS) $(DIR) $^ -E -o $@

#This is the recipe to generate assembly files from the respective .c files
%.asm:%.c
	$(CC) $(CFLAGS) $(DIR) $^ -S -o $@

%.s:%.c
	$(CC) $(CFLAGS) $(DIR) $^ -S -o $@

%.asm:%.i
	$(CC) $(CFLAGS) $(DIR) $^ -S -o $@

%.s:%.i
	$(CC) $(CFLAGS) $(DIR) $^ -S -o $@

%.o:%.c
	$(CC) $(CFLAGS) $(DIR) -c $^ -o $@

%.dep:%.c
	$(CC) $(CFLAGS) $(DIR) -M $^ > $@

#phony target protection telling make that these are recipes and not file names
.PHONY: build compile-all disp_size disp_nm build_lib build_transfer build_dep objectdump

#the compile all recipe builds all files into their corresponding object files but does not link them together
compile-all: $(OBJS)

#The build recipe builds all the object files and links them together to create the executable
build: $(TARGET)

#The disp_size recipe displays the size for each section of the executable and the total size
disp_size: $(TARGET)
	$(SIZE)

#The disp_nm recipe lists all the symbols from the object files
disp_nm: $(TARGET)
	$(NM)

#The build_lib recipe creates a library from the object files
build_lib: $(LINKER_OBJS)
	ar -cvq $(LIBRARY) $^

link_lib: $(LIBRARY) | build_lib
	ld -o out_lib 

#The build_dep recipe generates the dependencies for all the files
build_dep:$(DEP)

#The build_transfer recipe generates an executables and moves it to the BeagleBone
#This recipe is only for the BeagleBone cross compiler
build_transfer:$(TARGET)
	sudo scp -o StrictHostKeyChecking=no project1 root@192.168.7.2:/home/debian/bin

#The objectdump recipe disassembles the executable back to assembly code and stores it in the project1.asm file
objectdump:$(TARGET)
	$(DMP) -D -S $(TARGET) > project1.asm


#The unit_test recipe builds the executable for unit test cases
unit_test:
	cd ../Project2 && $(MAKE) test CASE=DATA
	cd ../Project2 && $(MAKE) test CASE=MEMORY
	cd ../Project2 && $(MAKE) test CASE=CIRCBUF

#The clean recipe removes all the generated files
.PHONY: clean
clean:
	rm -rf *.o *.i *.asm *.s *.map *.dep *.a $(TARGET)