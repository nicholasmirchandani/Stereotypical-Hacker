#include <iostream>
#include <errno.h>
#include "SocketConnection.h"

using namespace std;

int main(int argc, char** argv) {

	SocketConnection* listener = new SocketConnection();
	SocketConnection** clients = new SocketConnection*[2];

	clients[0] = listener->GetClientConnection();
	cout << "First Client Connected" << endl;
	clients[1] = listener->GetClientConnection();
	cout << "Second Client Connected" << endl;

	char* message = (char*)clients[1]->ReadFromStream(50);
	cout << "Message received: " << (string)message << endl;

	clients[0]->WriteToStream((void*)message, 50);
	cout << "Message sent" << endl;

	return 0;

}