ifeq ($(CASE),DATA)
	LIB_SRCS = data.c
	TEST_SRCS = test-data.c
	TEST_EXE = test-data.out

else ifeq ($(CASE),MEMORY)
	LIB_SRCS = memory.c
	TEST_SRCS = test-memory.c
	TEST_EXE = test-memory.out

else ifeq ($(CASE),CIRCBUF)
	LIB_SRCS = circbuf.c
	TEST_SRCS = test-circbuf.c
	TEST_EXE = test-circbuf.out
else
	FAIL=TRUE

endif

CC = cc
CFLAGS = -std=c99 -Wall -Werror -g -O0 -I $(CMOCKA_INCLUDE_DIR)

LIB_OBJS = $(LIB_SRCS:%.c=%.o)

LIB_HEADERS = $(LIB_SRCS:%.c=%.h)

LIB = libutils.a

EXE_SRCS = main.c
EXE_OBJS = $(EXE_SRCS:%.c=%.o)

EXE = main.out

TEST_OBJS = $(TEST_SRCS:%.c=%.o)

SUBDIRS = Libraries

ifdef SUBDIRS
.PHONY : $(SUBDIRS)
$(SUBDIRS) : 
	@if [ -d $@ ]; then \
		$(MAKE) --no-print-directory --directory=$@ \
			CC=$(CC) CFLAGS="$(CFLAGS)" $(MAKECMDGOALS); \
	fi	
endif


LIB_DIR = Libraries
CMOCKA_INCLUDE_DIR = $(LIB_DIR)/build-Debug/include
CMOCKA_LIBRARY = $(LIB_DIR)/build-Debug/lib/libcmocka.a

cmocka : $(SUBDIRS)

test : $(TEST_EXE)|test_case test_def
	./$(TEST_EXE)

test_case:
ifndef CASE
	$(error A valid value for CASE is not set)
endif

test_def:
ifdef FAIL
	$(error A valid value for CASE is not set)
endif

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB) : $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $^


$(EXE) : $(EXE_OBJS) $(LIB)
	$(CC) $(CFLAGS) -o $@ $^ -lm

$(TEST_EXE) : $(TEST_SRCS) $(LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(CMOCKA_LIBRARY) -lm


.PHONY : clean
clean : FORCE
	-rm -rf *.o $(EXE) $(TEST_EXE) *.out

.PHONY : clobber
clobber : $(SUBDIRS) clean


FORCE :