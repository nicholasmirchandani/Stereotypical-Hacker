#include <iostream>
#include <string>
#include "SocketConnection.h"

using namespace std;

int main(int argc, char** argv) {

	SocketConnection* server = new SocketConnection((char*)"127.0.0.1");
	server->WriteToStream((void*)argv[1], 50);

	return 0;

}