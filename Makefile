MAIN_FILE = main.c
HEADERS = async_wrappers.h

MAIN_OBJECT = $(patsubst %.c, %.o, $(MAIN_FILE))
EXEC = $(patsubst %.c, %, $(MAIN_FILE))

$(EXEC): $(HEADERS)

$(EXEC): $(MAIN_OBJECT)
	gcc -o $@ $^ -lpthread

%.o: %.c
	gcc -fPIC -c $^

clean:
	rm -rf *.o
	rm -rf $(EXEC)
