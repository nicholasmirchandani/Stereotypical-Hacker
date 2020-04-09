#include <iostream>
#include "SocketConnection.h"

using namespace std;

int main(int argc, char** argv) {

	SocketConnection* server = new SocketConnection((char*)"127.0.0.1", 50000);
	char* test = (char*)server->ReadFromStream(50);

	cout << test << endl;

	return 0;

}