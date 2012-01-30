LDFLAGS=-L/usr/local/Cellar/readline/6.2.2/lib
CPPFLAGS=-I/usr/local/Cellar/readline/6.2.2/include

myshell: myshell.o

myshell.0: myshell.c
	
clean:
	rm -f *.o

test: myshell 