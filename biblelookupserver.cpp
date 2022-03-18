// biblereader.cpp
// main function for Project 1

#include "Ref.h"
#include "Verse.h"
#include "Bible.h"
#include <iostream>
#include <fstream>
#include <string> 
#include <stdio.h>
#include <stdlib.h>
#include "fifo.h"
#include <regex>
using namespace std;

string receive_pipe = "SSrequest";
string send_pipe = "SSreply";

bool debugging = false;

int main() {
	// Create Bible object to process the raw text file
	Bible webBible("./web-complete");
	LookupResult result = SUCCESS;

	Fifo recfifo(receive_pipe);
	Fifo sendfifo(send_pipe);

	recfifo.openread();

	while (1) {
		string input = recfifo.recv();
		sendfifo.openwrite();


		int book, chap, verse;

		regex format(R"(^\d\d?:\d\d?:\d\d?$)");
		if (!(regex_match(input, format))) {
			sendfifo.send("1");
		}
		
		int first = input.find_first_of(':');
		int last = input.find_last_of(':');
		book = stoi(input.substr(0, first));
		chap = stoi(input.substr(first + 1, (last - first) - 1));
		verse = stoi(input.substr(last + 1, string::npos));

		if (debugging) {
			cout << "Book: " << book << endl;
			cout << "Chap: " << chap << endl;
			cout << "Verse: " << verse << endl;
		}

		Ref ref(book, chap, verse);
		Verse verseObject = webBible.lookup(ref, result);
		string verseText = verseObject.getVerse();
		ostringstream ss;
		ss << result;

		if (result == SUCCESS) {
			ss << verseText;
		}
		else {
			ss << webBible.error(result, ref);
		}

		sendfifo.send(ss.str());
		sendfifo.fifoclose();
	}

	/*
	//TODO: input check

	int numVerses = 1;

	int book, chap, verse;


	Ref ref(book, chap, verse);


	Verse v = webBible.lookup(ref, result);


	if (result != SUCCESS) {
		cout << "Lookup no success";
		cout << webBible.error(result, ref);
		return 0;
	}

	v.display();

	Verse next;
	for (int x = 1; x < numVerses; x++) {
		next = webBible.nextVerse(result);
		if (result != SUCCESS) {
			cout << webBible.error(result, ref);
			return 0;
		}
		//this avoids printing the full reference redundantly
		if (next.getRef().getVerse() == 1) {
			cout << next.getRef().getBookName() << " " << next.getRef().getChap() << "\n";
		}
		cout << next.getRef().getVerse() << " " << next.getVerse() << "\n";
	}
	*/

	webBible.close();
}
