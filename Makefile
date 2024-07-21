COMPILER  =gcc
CFLAGS    =-O02 -fPIC -Wall -Werror -Wextra
LDFLAGS   =-shared

LIB_NAME  =prng

LIB_SRCS  =src/mtwister.c \
           src/prng.c

LIB_OBJ   =$(LIB_SRCS:.c=.o)

all: $(LIB_NAME)

$(LIB_NAME): $(LIB_OBJ)
	mkdir -p lib
	$(COMPILER) $(LDFLAGS) -o lib/lib$@.so $^ -Iinclude -lm

%.o: %.c
	$(COMPILER) $(CFLAGS) -c $< -o $@ -Iinclude

define build_example
	$(COMPILER) $(CFLAGS) -o bin/example-$(1) examples/$(1).c -Iinclude -Llib -l$(LIB_NAME) -Wl,-rpath=`pwd`/lib
endef

examples: $(LIB_NAME)
	mkdir -p bin
	$(call build_example,twister)
	$(call build_example,uniform)

clean:
	rm -rf bin
	rm -rf lib
	rm -f src/*.o

.PHONY: all clean examples $(LIB_NAME)
