DEVICE:=$(shell arch)

include sources.mk

#CC defines the appropriate compiler depedning upon the platform and architecture
#CFLAGS defines the compiler flags for various architectures
#SIZE defines the flags for using the size option
#NM defines the command for listing symbols
#DMP defines the command for object dump

#If the platform is FRDM KL25z then set flags and compiler accordingly

ifeq ($(DEVICE),i686)
	CC=arm-linux-gnueabihf-gcc
	CFLAGS = -Wall -g -O0 -mthumb -march=armv7-a -mfloat-abi=hard -mfpu=vfpv3-d16 -mcpu=cortex-a8 -std=c99 
	SIZE = arm-linux-gnueabihf-size -B $(TARGET)
	NM = arm-linux-gnueabihf-nm $(TARGET)
	DMP = arm-linux-gnueabihf-objdump

else
	CC=gcc
	CFLAGS = -Wall -g -O0 -mthumb -march=armv7-a -mfloat-abi=hard -mfpu=vfpv3-d16 -mcpu=cortex-a8 -std=c99  
	SIZE = size -B $(TARGET)
	NM = nm $(TARGET)
	DMP = objdump

endif

	


build: $(SRCS)
	$(CC) $(CFLAGS) $(DIR) $(SRCS) -o $(TARGET)


build_transfer:build
	sudo scp -o StrictHostKeyChecking=no project4 root@192.168.7.2:/home/debian/bin

#The clean recipe removes all the generated files
.PHONY: clean
clean:
	rm -rf *.o *.i *.asm *.s *.map *.dep *.a $(TARGET)