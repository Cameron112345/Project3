USER= camcole

CC= g++
CFLAGS= -g -std=c++11

all: biblelookupserver PutCGI PutHTML 

Ref.o : Ref.h Ref.cpp
	$(CC) $(CFLAGS) -c Ref.cpp

Verse.o : Ref.h Verse.h Verse.cpp
	$(CC) $(CFLAGS) -c Verse.cpp

Bible.o : Ref.h Verse.h Bible.h Bible.cpp
	$(CC) $(CFLAGS) -c Bible.cpp



biblelookupclient.cgi:	biblelookupclient.o Ref.o Verse.o Bible.o fifo.o
	$(CC) $(CFLAGS) -o biblelookupclient.cgi biblelookupclient.o Ref.o Verse.o Bible.o fifo.o -lcgicc

biblelookupclient.o:	biblelookupclient.cpp fifo.h
	$(CC) $(CFLAGS) -c biblelookupclient.cpp fifo.h



biblelookupserver: biblelookupserver.o fifo.o Bible.o Ref.o Verse.o
	$(CC) $(CFLAGS) -o biblelookupserver biblelookupserver.o fifo.o Bible.o Ref.o Verse.o

biblelookupserver.o: biblelookupserver.cpp fifo.h Bible.h Ref.h Verse.h
	$(CC) $(CFLAGS) -c biblelookupserver.cpp


			
PutCGI:	biblelookupclient.cgi
		chmod 755 biblelookupclient.cgi
		cp biblelookupclient.cgi /var/www/html/class/csc3004/$(USER)/cgi-bin

		echo "Current contents of your cgi-bin directory: "
		ls -l /var/www/html/class/csc3004/$(USER)/cgi-bin/

PutHTML:
		cp bibleindex.html /var/www/html/class/csc3004/$(USER)

		echo "Current contents of your HTML directory: "
		ls -l /var/www/html/class/csc3004/$(USER)

fifo.o: fifo.cpp fifo.h
	$(CC) $(CFLAGS) -c fifo.cpp




clean:		
		rm *.o core biblelookupclient.cgi
