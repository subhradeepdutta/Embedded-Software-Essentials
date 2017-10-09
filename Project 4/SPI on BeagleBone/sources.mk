#Tells make where the header files are located
vpath=./headers: 

# DIR=-I ./headers -I ./sources
#A variable to point to the location of the header files
DIR=-I ./headers

#SRCS defines the list of source .c files
SRCS=main.c nrf.c spidriver.c

#HDR defines the list of header files
HDR=nrf.h spidriver.h

#OBJS defines the list of object files
OBJS=$(SRCS:.c=.o)

CODE_TO_PPR=$(OBJS:.o=.i)
PPR_TO_ASM=$(OBJS:.o=.s)
ASM_TO_OBJS=$(OBJS)


LINKER_C= main.c spidriver.c nrf.c

#TARGET defines the final exectuable
TARGET=project4

#LINKER_OBJS defines the list of object files needed for the library generation
LINKER_OBJS=$(LINKER_C:.c=.o)

#DEP defines the list of dependency files
DEP:=$(SRCS:.c=.dep)