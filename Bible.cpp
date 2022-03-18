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
	if (it != index.end()) {
		x = index[ref];
	}
	else {
		Ref testBook(ref.getBook(), 1, 1);
		Ref testChap(1, ref.getChap(), 1);
		Ref testVerse(1, 1, ref.getVerse());

		if (!refExists(testBook)) status = NO_BOOK;
		else if (!refExists(testChap)) status = NO_CHAPTER;
		else if (!refExists(testVerse)) status = NO_VERSE;
		else status = UNKNOWN_ERROR;
		return Verse();
	}
	instream.seekg(x);
	string line;
	getline(instream, line);

	Verse v(line);
	return v;
}

bool Bible::refExists(Ref ref) {
	map<Ref, int>::iterator it;
	it = index.find(ref);
	if (it != index.end())
		return true;
	else return false;
}

// REQUIRED: lookup finds a given verse in this Bible
/*Verse Bible::lookup(Ref ref, LookupResult& status) {
    // TODO: scan the file to retrieve the line that holds ref ...
    // update the status variable
	status = SUCCESS; // placeholder until retrieval is attempted
	// create and return the verse object
	Verse aVerse;   // default verse, to be replaced by a Verse object
	                // that is constructed from a line in the file.

	string buffer;
	short c = 0; //placeholder for current lines book/chap/verse
	short book = ref.getBook(); //shorthands for the requested reference numbers
	short chap = ref.getChap();
	short verse = ref.getVerse();
	Ref r;

	//loops through lines of the file until the correct book number is reached
	while (c != book) {
		getline(instream, buffer);
		r = Ref(buffer);
		c = r.getBook();
		//tests if end of file is reached
		if (instream.peek() == EOF) {
			status = NO_BOOK;
			aVerse = Verse(buffer);
			return aVerse;
		}
	}

	//After correct book is reached, read lines from file until correct chapter is reached
	c = 1;
	while (c != chap) {
		getline(instream, buffer);
		r = Ref(buffer);
		c = r.getChap();
		//tests if end of file is reached
		if (instream.peek() == EOF) {
			status = NO_CHAPTER;
			aVerse = Verse(buffer);
			return aVerse;
		}
		//tests if chapter exists within the book
		if (r.getBook() != book) {
			status = NO_CHAPTER;
			aVerse = Verse(buffer);
			return aVerse;
		}
	}

	//after correct chapter is reached, loop through file until correct verse is found
	c = 1;
	while (c != verse) {
		getline(instream, buffer);
		r = Ref(buffer);
		c = r.getVerse();
		//tests if end of file is reached and ensures that requesting the last verse in the Bible does not throw an error
		if (instream.peek() == EOF && c != verse) {
			status = NO_VERSE;
			aVerse = Verse(buffer);
			return aVerse;
		}
		//tests if verse exists within the chapter
		if (r.getChap() != chap) {
			status = NO_VERSE;
			aVerse = Verse(buffer);
			return aVerse;
		}
	}

	aVerse = Verse(buffer);
	status = SUCCESS;
    return(aVerse);
}*/

// REQUIRED: Return the next verse from the Bible file stream if the file is open.
// If the file is not open, open the file and return the first verse.
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
