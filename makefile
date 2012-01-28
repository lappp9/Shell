myshell: myshell.o

myshell.0: myshell.c
	
clean:
	rm -f *.o

test: myshell 