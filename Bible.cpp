// Bible class function definitions
// Computer Science, MVNU

#include "Ref.h"
#include "Verse.h"
#include "Bible.h" 
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

Bible::Bible(const string s) {
	infile = s;
	if (buildTextIndex(infile) != 1) {
		cout << "Error building index";
	}

	/*instream.open(infile.c_str(), ios_base::in);
	if (!instream) {
		cerr << "Error - can't open input file: " << infile << endl;
		exit(2);
	}
	instream.unsetf(ios::skipws); 
	isOpen = true;
	*/
}

void Bible::close() {
	instream.close();
}

Verse Bible::lookup(Ref ref, LookupResult& status) {
	map<Ref, int>::iterator it;
	it = index.find(ref);
	int x=0;


	//If ref was found in index
	if (it != index.end()) {
		x = index[ref];
	}
	//If ref was not found
	else {
		//This is a reference for the input book number, chapter 1, and verse 1
		//Since there is a chapter 1 verse 1 for every book, this will reveal if the book number is the issue
		Ref testBook(ref.getBook(), 1, 1);


		//This only gets used if the book number is determined to not be the issue
		//It used the input book number and chapter number, but verse 1
		//Since there is a verse 1 to every book in the bible, it will reveal if the chapter number is the issue
		Ref testChap(ref.getBook(), ref.getChap(), 1);


		//test the book
		if (!refExists(testBook)) status = NO_BOOK;


		//test the chapter
		else if (!refExists(testChap)) status = NO_CHAPTER;


		//if book and chapter aren't the issue, then the issue must be the verse number
		else status = NO_VERSE;


		return Verse();
	}


	//move to the right position
	instream.seekg(x);


	//read the line
	string line;
	getline(instream, line);


	Verse v(line);
	return v;
}


//This function return true if ref exists within the index
//return false otherwise
bool Bible::refExists(Ref ref) {
	map<Ref, int>::iterator it;
	it = index.find(ref);
	if (it != index.end())
		return true;
	return false;
}

Verse Bible::nextVerse(LookupResult& status) {

	//placeholders
	Verse v;
	string s;
	Ref r;
	status = SUCCESS;

	//tests if file is closed
	if (!isOpen) {
		instream.open(infile.c_str(), ios::in);
		if (!instream) {
			cerr << "Error - can't open input file: " << infile << endl;
			exit(2);
		}
		instream.unsetf(ios::skipws); // include white space in read operations
		isOpen = true;

		//returns the first verse
		getline(instream, s);
		v = Verse(s);
		return v;
	}

	//tests if end of file has been reached
	if (instream.peek() == EOF) {
		status = END_OF_FILE;
		return Verse();
	}


	getline(instream, s);
	r = Ref(s);
	//tests if a new book has been entered. Throws an error is so
	if (r.getChap() == 1 && r.getVerse() == 1) {
		status = DIFFERENT_BOOK;
		return Verse();
	}
	v = Verse (s);
	return v;
}

// REQUIRED: Return an error message string to describe status
string Bible::error(LookupResult status, Ref ref) {
	switch (status) {
		case (NO_BOOK): {
			return "Error: no such book " + to_string(ref.getBook())+"\n";
			break;
		}
		case (NO_CHAPTER) : {
			return "Error: no such chapter " + to_string(ref.getChap())+"\n";
			break;
		}
		case (NO_VERSE): {
			return "Error: no such verse " + to_string(ref.getVerse())+"\n";
			break;
		}
		case (END_OF_FILE): {
			return "Error: end of file reached\n";
			break;
		}
		case (DIFFERENT_BOOK): {
			return "Error: verses span more than 1 book\n";
			break;
		}
		case (BAD_FORMAT): {
			return "Error: Incorrect format\n";
			break;
		}
		case (UNKNOWN_ERROR): {
			return "Error: unknown error occurred\n";
			break;
		}
	default: {
		return "";
		break;
	}
	}
}

void Bible::display() {
	cout << "Bible file: " << infile << endl;
}

int Bible::buildTextIndex(string filename) {
	int position;         /* location of line in the file */
	string text, line, word;
	Ref ref;

	/* open text file */
	instream.open(filename.c_str(), ios::in);
	if (!instream) {
		cerr << "Error - can't open input file: " << filename << endl;
		return 0;  /* false, indicates error */
	}

	/* process text file */
	
	while (instream.peek()!=EOF) {
		/* Get the file position at beginning of line */
		position = instream.tellg();
		/* get the next line */
		getline(instream, line);
		ref = Ref(line);
		index.insert({ ref, position });
	}
	//} // end while loop for lines of file

	if (debugging) {

		cout << "References: " << index.size() << endl;
		cout << "Last byte offset: " << position << endl;
		ref = Ref(1, 1, 1);
		int x = index[ref];
		cout << "Byte offset for Ref(1, 1, 1): " << x << endl;
		ref = Ref(1, 1, 2);
		x = index[ref];
		cout << "Byte offset for Ref(1, 1, 2): " << x << endl;
		ref = Ref(1, 1, 3);
		x = index[ref];
		cout << "Byte offset for Ref(1, 1, 3): " << x << endl;
	}
	isOpen = true;
	return 1;  /* true, indicates success */
}


string Bible::getNextWord(string& line) {
	string word = "";
	int pos;

	if (line.length() > 1)
	{
		pos = line.find_first_of(".,; :?!""''()");
		/* get next word */
		if (pos != 0) {
			word = line.substr(0, pos);
		}
		/* trim the line */
		if (pos == string::npos) {
			pos = line.length() - 1;
		}
		line = line.substr(pos + 1, 2000);
	}
	return word;
}

Ref Bible::next(const Ref ref, LookupResult& status) {
	map<Ref, int>::iterator it = index.find(ref);
	int x = it->second;
	instream.seekg(x);
	string s;
	getline(instream, s);
	getline(instream, s);
	Ref r(s);
	return r;
}


// OPTIONAL access functions
// OPTIONAL: Return the reference after the given ref


// OPTIONAL: Return the reference before the given ref
//Ref Bible::prev(const Ref ref, LookupResult& status) {}
