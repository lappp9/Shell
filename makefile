PAWPRINT=lappp9
# set up linker options
LDLIBS= -lreadline -lhistory

myshell: 

clean:
	rm -f *.o

test: myshell
	cat test.txt | ./myshell

nuke:
	rm -rf *.o myshell *.gz dist

submit.sh:
	curl -L --output submit.sh http://www.ryannedolan.info/teaching/cs4520/projects/submit.sh
	chmod +x submit.sh

myshell3.tar.gz: myshell.c test.txt makefile submit.sh output.txt
	mkdir -p dist
	cp $^ dist
	tar -cvzf $@ dist

submit: submit.sh myshell3.tar.gz
	PAWPRINT=$(PAWPRINT) sh submit.sh myshell3.tar.gz

