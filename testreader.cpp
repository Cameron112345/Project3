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
using namespace std;

int main(int argc, char* argv[]) {
	// Create Bible object to process the raw text file
	Bible webBible("./web-complete");
	LookupResult result = SUCCESS;

	if (argc < 4 || argc > 5) { 
		cout << "Error: Invalid number of arguments";
		return 0;
	}

	int book = stoi(argv[1]);
	int chap = stoi(argv[2]);
	int verse = stoi(argv[3]);
	int numVerses=1;
	if (argc == 5)
		numVerses = stoi(argv[4]);

	Ref ref(book, chap, verse);


	cout << "Result before lookup: " << result << endl;
	Verse v = webBible.lookup(ref, result);
	cout << "Result after lookup: " << result << endl;
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
		cout << next.getRef().getVerse() << " " << next.getVerse() << "\n";
	}

	webBible.close();

	//Verse verse;
	////span holds the number of verses that will be displayed. Default is 1
	//int b, c, v, span = 1;

	////tests if enough inputs are provided
	//if (argc < 4) {
	//	if (argc == 3)
	//		cout << "Error: verse number is missing\n";
	//	else cout << "Error: not enough inputs\n";
	//	return 0;
	//}

	////tests that not too many inputs are provided
	//else if (argc <= 5) {

	//	b = stoi(argv[1]);
	//	c = stoi(argv[2]);
	//	v = stoi(argv[3]);

	//	//if a span is specified, set it to the number provided
	//	if (argc == 5) {
	//		span = stoi(argv[4]);
	//	}

	//}
	//else {
	//	cout << "Error: too many inputs";
	//	return 0;
	//}

	//LookupResult result;
	//cout << "Using Bible from: ";
	//webBible.display();
	//// TODO: your final program should get input from command line arguments instead.

	//// Create a reference from the numbers
	//Ref ref(b, c, v);

	//verse = webBible.lookup(ref, result);
	//if (result != SUCCESS) {
	//	cout << webBible.error(result, ref);
	//	return 0;
	//}



	////loops if a span is provided, calling the next function
	//verse.display();
	//Verse next;
	//for (int x = 1; x < span; x++) {
	//	next = webBible.nextVerse(result);
	//	if (result != SUCCESS) {
	//		cout << webBible.error(result, ref);
	//		return 0;
	//	}
	//	//this avoids printing the full reference redundantly
	//	cout << next.getRef().getVerse() << " " << next.getVerse() << "\n";
	//}
	//cout << endl;
}
