OBJ = $(patsubst %.c, %.o, $(wildcard *.c))

invsearch: $(OBJ)
	gcc -o $@ $^

clean:
	rm -f *.o