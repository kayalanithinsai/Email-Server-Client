CC=gcc
CFLAGS=-I.

myfiles: emailserver.c emailclient.c
	$(CC) -o emailserver emailserver.c
	$(CC) -o emailclient emailclient.c
	
clean:	
	rm -f *.txt emailserver emailclient
