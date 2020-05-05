#ifndef SENDING_FAILURE
#define SENDING_FAILURE

#include <iostream>

using namespace std;

/* An exception class for failure sending data across socket.
*/
class sending_failure {

  public:
    sending_failure() {
    	errMessage = "Data failed to be send through socket.";
    }
    sending_failure(const string &errMessage) {
    	this->errMessage = errMessage;
    }
    string get_message() {
    	return errMessage;
    }

  private:
    string errMessage;

};

#endif