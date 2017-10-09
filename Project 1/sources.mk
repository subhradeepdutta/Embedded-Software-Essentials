#Tells make where the header files are located
vpath=./headers: 

# DIR=-I ./headers -I ./sources
#A variable to point to the location of the header files
DIR=-I ./headers

#SRCS defines the list of source .c files
SRCS=main.c memory.c data.c project_1.c feature.c

#HDR defines the list of header files
HDR=memory.h data.h project_1.h feature.h

#OBJS defines the list of object files
OBJS=$(SRCS:.c=.o)

CODE_TO_PPR=$(OBJS:.o=.i)
PPR_TO_ASM=$(OBJS:.o=.s)
ASM_TO_OBJS=$(OBJS)

#TARGET defines the final exectuable
TARGET=project1

#LINKER defines the list of files needed for the library generation
LINKER_C= memory.c data.c

#LINKER_OBJS defines the list of object files needed for the library generation
LINKER_OBJS=$(LINKER_C:.c=.o)

#LIBRARY defines the generated library
LIBRARY=libproject1.a

#DEP defines the list of dependency files
DEP:=$(SRCS:.c=.dep)