#ifndef READING_FAILURE
#define READING_FAILURE

#include <iostream>

using namespace std;

/* An exception class for failure to read data from socket.
*/
class reading_failure {

  public:
    reading_failure() {
    	errMessage = "Data failed to be read from socket.";
    }
    reading_failure(const string &errMessage) {
    	this->errMessage = errMessage;
    }
    string get_message() {
    	return errMessage;
    }

  private:
    string errMessage;

};

#endif