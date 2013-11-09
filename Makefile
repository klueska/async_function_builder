LIB_NAME = async
EXEC = main
MAIN_SRC = main.c
LIB_SRC = libc_wrappers.c async_wrappers.c
HEADERS = async_wrappers.h
CFLAGS = -fPIC -O3 -std=gnu99

LIB = lib$(LIB_NAME).so
MAIN_OBJECT = $(patsubst %.c, %.o, $(MAIN_SRC))
LIB_OBJECT = $(patsubst %.c, %.o, $(LIB_SRC))

$(EXEC): $(LIB)

$(EXEC): $(MAIN_OBJECT)
	gcc -o $@ $^ -L. -Wl,-rpath,. -l$(LIB_NAME)

$(LIB): $(LIB_OBJECT)
	gcc -shared -o $@ $^ -lpthread

%.o: %.c $(HEADERS)
	gcc $(CFLAGS) -c $<

clean:
	rm -rf *.o
	rm -rf $(EXEC) $(LIB)
