#include <iostream>
#include <errno.h>
#include "SocketConnection.h"
#include "reading_failure.h"

using namespace std;

int main(int argc, char** argv) {

	SocketConnection* listener = new SocketConnection(50000);
	SocketConnection** clients = new SocketConnection*[2];

	clients[0] = listener->GetClientConnection();
	cout << "First Client Connected" << endl;
	clients[1] = listener->GetClientConnection();
	cout << "Second Client Connected" << endl;

	char* message = (char*)clients[1]->ReadFromStream(50);
	cout << "Message received: " << (string)message << endl;

	clients[0]->WriteToStream((void*)message, 50);
	cout << "Message sent" << endl;

	try {
		clients[0]->ReadFromStream(50);
	} catch (reading_failure e) {
		if (!clients[0]->IsAlive()) {
			cout << "He's dead, Jim" << endl;
		}		
	}

	return 0;

}