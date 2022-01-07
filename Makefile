HEADERS = bitcoin.h

default: bitcoin

bitcoin.o: bitcoin.c $(HEADERS)
	gcc -c bitcoin.c -o bitcoin.o

bitcoin: bitcoin.o
	gcc bitcoin.o -o bitcoin

clean:
	-rm -f bitcoin.o
	-rm -f bitcoin
