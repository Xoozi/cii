VPATH = src:include

LIB_DIR = lib

INCLUDE_DIR = include

LIB = $(LIB_DIR)/libcii.a

objects = arith.o assert.o except.o memchk.o

all: $(objects)
	ar crs $(LIB) $(objects)

arith.o: arith.c arith.h
	gcc -g -c $< -I$(INCLUDE_DIR)

except.o: except.c except.h assert.h
	gcc -g -c $< -I$(INCLUDE_DIR)

assert.o: assert.c assert.h except.h
	gcc -g -c $< -I$(INCLUDE_DIR)

memchk.o: memchk.c mem.h assert.h except.h
	gcc -g -c $< -I$(INCLUDE_DIR)

clean:
	rm *.o
	rm $(LIB)
