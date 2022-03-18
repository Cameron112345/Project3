/*****************************************************************************/
/*  Filename:     textindex.cpp                                              */
/*  Author(s):    Jim Skon                                                   */
/*  Last Update:  2/16/2018 by Bob Kasper (comments and formatting)          */
/*                                                                           */
/*  This code creates an inverted index of all the words in some text        */
/*  as a STL map structure that maps from words to positions in the file.    */
/*  The resulting structure can than be used to retrieve a complete list of  */
/*  all places in the file that contain a certain word.                      */
/*                                                                           */
/*  The function buildTextIndex(filename) creates the inverted index.        */
/*                                                                           */
/*  The function indexSearch(string) returns a STL list structure of         */
/*  all positions where that word is found in the file.                      */
/*****************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <map>
#include <utility>
#include <list>
#include <set>

using namespace std;
#include "textindex.h"