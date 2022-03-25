/* sslookupclient.cpp
 * Shakespeare full-text search client:
 * receives an Ajax request from web client
 * extracts search string, and sends request to sslookupserver
 */
#include <iostream>
#include <fstream>
#include <string>

// Stuff for Ajax
#include "/home/class/csc3004/cgicc/Cgicc.h"
#include "/home/class/csc3004/cgicc/HTTPHTMLHeader.h"
#include "/home/class/csc3004/cgicc/HTMLClasses.h"
using namespace cgicc;

//Stuff for pipes                                                               
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include "fifo.h"
using namespace std;

//stuff for logging
#define logging
#define LOG_FILENAME "/home/class/csc3004/tmp/camcole-bibleLog.log"
#include "logfile.h"

// Pipes for communication                                                      
string receive_pipe = "SSreply";
string send_pipe = "SSrequest";

int main() {
  cout << "Content-Type: text/plain\n\n" << endl;
  Cgicc cgi;

  //If logging is enabled
  #ifdef logging 
    logFile.open(LOG_FILENAME, ios::out); 
  #endif

  
  //grab all the values from the url sent to cgi
  form_iterator st = cgi.getElement("search_type");
  form_iterator book = cgi.getElement("book");
  form_iterator chapter = cgi.getElement("chapter");
  form_iterator verse = cgi.getElement("verse");
  form_iterator nv = cgi.getElement("num_verse");


  //convert values to strings and combine them
  string bookNum = book->getValue();
  string chapterNum = chapter->getValue();
  string verseNum = verse->getValue();
  string searchString = bookNum + ":" + chapterNum + ":" + verseNum;
  log("SearchString: " + searchString);
  

  //Create pipes
  Fifo recfifo(receive_pipe);
  Fifo sendfifo(send_pipe);
  log("Pipes Created");
  

  //Open the send pipe
  sendfifo.openwrite();
  log("sendfifo opened");

  
  // Call server to get results                                                 
  sendfifo.send(searchString);


  //open the receiving pipe
  recfifo.openread();
  log("recfifo opened");


  //Recieve the response from the server
  string results = recfifo.recv();
  log("Retreived response from server");


  //Peel the status number off the response
  string statusNum = results.substr(0, 1);
  results.erase(0, 1);


  //Check if status is an error code (Not 0)
  if (statusNum != "0") {
      cout << "<p>Error Number: " << statusNum << "</p>" << endl;
      cout << "<p>" << results << "</p>" << endl;
      return 0;
  }
  

  //Output the message inside of a p tag
  cout << "<p>" << results << "</p>" << endl;


  //See if multiple verses need to be displayed
  int verseCount = nv->getIntegerValue();


  for (int x = 1; x < verseCount; x++) {

      //This string, "00:00:00", is recognized by the server as a request to send the results from the "nextVerse" function
      sendfifo.send("00:00:00");

      //This is the same procedure as above
      results = recfifo.recv();
      statusNum = results.substr(0, 1);
      results.erase(0, 1);
      if (statusNum != "0")
          cout << "<p>Error Number: " << statusNum << "</p>" << endl;
      cout << "<p>" << results << "</p>" << endl;
  }

  //Flushing output and closing pipes
  cout << endl;
  recfifo.fifoclose();
  sendfifo.fifoclose();
  log("pipes closed");
  return 0;
}
