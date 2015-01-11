VPATH = src:include

LIB_DIR = lib

INCLUDE_DIR = include

LIB = $(LIB_DIR)/libcii.a

objects = arith.o except.o

all: $(objects)
	ar rs $(LIB) $(objects)

arith.o: arith.c arith.h
	gcc -g -c $< -I$(INCLUDE_DIR)

except.o: except.c except.h
	gcc -g -c $< -I$(INCLUDE_DIR)

clean:
	rm *.o
