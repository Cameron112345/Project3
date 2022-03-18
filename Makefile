USER= camcole

CC= g++
CFLAGS= -g -std=c++11

all: PutCGI PutHTML biblelookupserver
#all:	bibleajax.cgi PutCGI PutHTML

Ref.o : Ref.h Ref.cpp
	$(CC) $(CFLAGS) -c Ref.cpp

Verse.o : Ref.h Verse.h Verse.cpp
	$(CC) $(CFLAGS) -c Verse.cpp

Bible.o : Ref.h Verse.h Bible.h Bible.cpp
	$(CC) $(CFLAGS) -c Bible.cpp


bibleajax.cgi:	bibleajax.o Ref.o Verse.o Bible.o
		$(CC) $(CFLAGS) -o bibleajax.cgi bibleajax.o Ref.o Verse.o Bible.o -lcgicc

bibleajax.o:	bibleajax.cpp
		$(CC) $(CFLAGS) -c bibleajax.cpp

			
PutCGI:	bibleajax.cgi
		chmod 755 bibleajax.cgi
		cp bibleajax.cgi /var/www/html/class/csc3004/$(USER)/cgi-bin

		echo "Current contents of your cgi-bin directory: "
		ls -l /var/www/html/class/csc3004/$(USER)/cgi-bin/

PutHTML:
		cp bibleajax.html /var/www/html/class/csc3004/$(USER)

		echo "Current contents of your HTML directory: "
		ls -l /var/www/html/class/csc3004/$(USER)

fifo.o: fifo.cpp fifo.h
	$(CC) $(CFLAGS) -c fifo.cpp

biblelookupserver.o: biblelookupserver.cpp fifo.h textindex.h Bible.h Ref.h Verse.h
	$(CC) $(CFLAGS) -c biblelookupserver.cpp

biblelookupserver: biblelookupserver.o fifo.o Bible.o Ref.o Verse.o
	$(CC) $(CFLAGS) -o biblelookupserver biblelookupserver.o fifo.o Bible.o Ref.o Verse.o


clean:		
		rm *.o core bibleajax.cgi
