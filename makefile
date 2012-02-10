PAWPRINT=lappp9

myshell: 
	gcc -lreadline -lhistory -o myshell myshell.c

clean:
	rm -f *.o

test: myshell
	cat test.txt | ./myshell

nuke:
	rm -rf *.o myshell *.gz dist

submit.sh:
	curl -L --output submit.sh http://www.ryannedolan.info/teaching/cs4520/projects/submit.sh
	chmod +x submit.sh

project3.tar.gz: myshell.c test.txt makefile submit.sh
	mkdir -p dist
	cp $^ dist
	tar -cvzf $@ dist

submit: submit.sh project3.tar.gz
	PAWPRINT=$(PAWPRINT) sh submit.sh project2.tar.gz

