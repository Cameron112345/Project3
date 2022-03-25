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
	


	//Create pipes
	Fifo recfifo(receive_pipe);
	Fifo sendfifo(send_pipe);

	//open the receiving pipe
	recfifo.openread();


	//define the regex to check the format of search strings
	regex format(R"(^\d\d?:\d\d?:\d\d?$)");


	//Declarations to prevent scope issues
	LookupResult result;
	int book, chap, verse;
	Verse verseObject;
	Ref ref;


	//infinite loop for listening for requests
	while (1) {
		string input = recfifo.recv();


		//open the send pipe
		sendfifo.openwrite();


		//String stream to build the response with
		ostringstream ss;


		//Default value for result
		result = SUCCESS;


		//Checks if request string matches regex
		if (!(regex_match(input, format))) {
			result = BAD_FORMAT;
			ss << result;
			ss << webBible.error(result, Ref());
		}
		//regex matches
		else {
			//These are markers for the positions of the colons
			int first = input.find_first_of(':');
			int last = input.find_last_of(':');


			//using substr method to parse book, chapter, and verse numbers
			string bookString = input.substr(0, first);
			string chapString = input.substr(first + 1, (last - first) - 1);
			string verseString = input.substr(last + 1, string::npos);


			//Checks if the request if for the nextVerse method
			if (bookString == "00" && chapString == "00" && verseString == "00") {
				verseObject = webBible.nextVerse(result);
				ss << result;
				if (result == SUCCESS) {
					Ref r = verseObject.getRef();

					//Checks if a new chapter has been entered
					//If so, the book name and new chapter number need displayed on a new line
					if (r.getVerse() == 1) {
						ss << "<p><strong>";
						ss << verseObject.getRef().getBookName();
						ss << " ";
						ss << verseObject.getRef().getChap();
						ss << "</strong></p>";
					}
				}
			}
			//Not a request for nextVerse()
			else {
				//convert book, chapter, and verse into their integer equivalents
				book = stoi(bookString);
				chap = stoi(chapString);
				verse = stoi(verseString);


				//debugging
				if (debugging) {
					cout << "Book: " << book << endl;
					cout << "Chap: " << chap << endl;
					cout << "Verse: " << verse << endl;
				}


				//create ref with the integer values
				ref = Ref(book, chap, verse);


				//Call lookup function 
				verseObject = webBible.lookup(ref, result);
				ss << result;


				//Since this verse is being retrieved by the lookup function instead of the nextVerse function,
				//It must be the first verse being displayed on the page,
				//therefore, it needs the book name + chapter number heading
				if (result == SUCCESS) {
					ss << "<p><strong>";
					ss << verseObject.getRef().getBookName();
					ss << " ";
					ss << verseObject.getRef().getChap();
					ss << "</strong></p>";
				}
			}


			//add the verse number as well as verse text
			if (result == SUCCESS) {
				ss << verseObject.getRef().getVerse();
				ss << " ";
				ss << verseObject.getVerse();
			}
			//add error message instead  if relevant
			else
				ss << webBible.error(result, ref);

		}

		//send the message
		sendfifo.send(ss.str());


		//close the pipes
		sendfifo.fifoclose();
	}

	webBible.close();
}
