myshell: myshell.o

myshell.o: myshell.c

clean:
	rm -f *.o

test: myshell