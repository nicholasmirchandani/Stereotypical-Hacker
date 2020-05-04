
#include <iostream>
#include "PersistentServer.h"

using namespace std;


int main(int argc, char** argv) {

	PersistentServer* perserv = new PersistentServer(3, 2, 49153);

	perserv->ServerLoop();

	cout << "Server has shut down" << endl;

}