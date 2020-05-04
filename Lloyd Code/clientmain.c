#include "SocketConnection.h"
#include <iostream>
#include <string.h>
#include <thread>

using namespace std;

void ServerListener();
SocketConnection* toserver;

int main(int argc, char** argv) {

	toserver = new SocketConnection((char*)"127.0.0.1", 49153);
	string temp;

	thread listen(ServerListener);

	while (true) {
		cout << "test@shell:> ";
		getline(cin, temp);

		char* toServer = new char[temp.size()+1];
		copy(temp.begin(), temp.end(), toServer);
		toServer[temp.size()] = '\0';

		if (temp == "quit") {

			toserver->WriteToStream((void*)toServer, strlen(toServer));
			break;

		} else {
			toserver->WriteToStream((void*)toServer, strlen(toServer));
		}
	}

}

void ServerListener() {
	while (true) {
		cout << '\n' << (char*)toserver->ReadFromStream(500) << endl << "test@shell:> " << flush;
	}
}