COMPILER  =gcc
CFLAGS    =-O02 -fPIC -Wall -Werror -Wextra
LDFLAGS   =-shared

LIB_NAME  =prng

LIB_SRCS  =mtwister.c
LIB_OBJ   =$(LIB_SRCS:.c=.o)

all: $(LIB_NAME)

$(LIB_NAME): $(LIB_OBJ)
	mkdir -p lib
	$(COMPILER) $(LDFLAGS) -o lib/lib$@.so obj/$^ -Iinclude

%.o: src/%.c
	mkdir -p obj
	$(COMPILER) $(CFLAGS) -c $< -o obj/$@ -Iinclude

examples: $(LIB_NAME)
	mkdir -p bin
	$(COMPILER) $(CFLAGS) -o bin/example-twister examples/twister.c -Iinclude -Llib -l$(LIB_NAME) -Wl,-rpath=`pwd`/lib

clean:
	rm -rf bin
	rm -rf lib
	rm -rf obj

.PHONY: all clean examples $(LIB_NAME)
