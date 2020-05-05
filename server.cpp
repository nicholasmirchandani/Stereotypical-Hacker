#include <iostream>
#include <string>
#include <cstring> //for memset
#include <thread> //for std::thread
#include <exception> //for std::terminate
#include <vector> //for std::vector
//Networking Includes!
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
//TODO: Move custom includes to the top and throw necessary includes in there to prevent this clusterfuck
#include "Player.h"
#include "VirtualServer.h"

void initializeServers();
void playGame(int p1Socket, int p2Socket);
void listenPlayerGame(int playerSocket, int otherSocket, int* index, std::string targetSentence, bool* gameOver);
void listenPlayer(int playerSocket);

std::vector<VirtualServer> serverList;

//NOTE: Just about everything in main is boilerplate code that won't be used in the final game.  just playGame; everything else should be handled by Lloyd and Alex
int main() {

    //Before network code, we need to initialize our serverList of Virtual Servers
    initializeServers();

    //Socket code taken from springer and then modified
    int listeningSocket;
    struct sockaddr_in serverAddress, p1Address, p2Address;

    // Step 1: Create a socket
    listeningSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Step 2: Bind to a port number
    memset(&serverAddress, 0, sizeof(struct sockaddr_in));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(9090); //<-- Change this port to change the port
    bind(listeningSocket, (struct sockaddr*)&serverAddress, sizeof(struct sockaddr_in));

    // Step 3: Listen for connections
    listen(listeningSocket, 5);

    // Step 4: Accept 2 connection requests
    unsigned int client_len = sizeof(struct sockaddr_in);
    Player p1;
    Player p2;    

    //TODO: Lobby system to make the players start at the same time
    //Interweaving accept and thread calls so player doesn't have to wait for other to connect to begin
    p1.socket = accept(listeningSocket, (struct sockaddr*)&p1Address, &client_len);
    std::thread listenP1(listenPlayer, p1.socket);
    p2.socket = accept(listeningSocket, (struct sockaddr*)&p2Address, &client_len);
    std::thread listenP2(listenPlayer, p2.socket);

    //Close the listening socket after we've connected our 2 players
    close(listeningSocket);


    listenP1.join();
    listenP2.join();

    //Don't just start the game, unless both sockets say to play the game.  Commenting out game now to handle connection stuff later
    //playGame(p1Socket, p2Socket);

    // Step 6: Close the connections
    close(p1.socket);
    close(p2.socket);
    return 0;
}

//TODO: Pass player here instead of an int for better debug
void listenPlayer(int playerSocket) {
    while(true) {
        char buffer[100];
        memset(buffer, 0, sizeof(buffer)); //Clearing the buffer before each read
        int len = read(playerSocket, buffer, 100); //TODO: Have this read for a char received/cancel everything message, and terminate the thread on char received
        printf("Received %d bytes from socket %d: %s\n", len, playerSocket, buffer); //Prints out receivedMessage
        fflush(stdout);
        std::string command(buffer);
        std::vector<std::string> arguments;
        std::string temp(buffer); //Copying from buffer instead of command because it's probably faster

        //Split command by space
        { //Using a block here so j is only as persistent as necessary
            int j = 0;
            for(int i = 0; i < temp.length(); ++i) {
                if(temp[i] == ' ') {
                    if(command == temp) {
                        command = temp.substr(0, i);
                        j = i;
                    } else {
                        arguments.insert(arguments.end(), temp.substr(j + 1, i-j));
                        j = i;
                    }
                }
            }

            if(j != temp.length() - 1 && command != temp) {
                //Adding the last element, not delineated by space
                arguments.insert(arguments.end(), temp.substr(j + 1));
            }
        }

        std::cout << "COMMAND: " << command << std::endl;
        for(std::string argument : arguments) {
            std::cout << "ARGUMENT: " << argument << std::endl;
        }
        temp = ""; //Reusing temp for optimization;
        if(command == "help") {
            //Help simply sends a long string with all available user commands
            temp = "PRINT: \n  --- USER COMMANDS ---\n\n";
	        
            temp += "ls - Lists all files and subdirectories in current directory\n";
            temp += "   Usage: ls\n\n";

            temp += "read - Prints contents of a text file to console\n";
            temp += "   Usage: read <text-file>\n\n";

            temp += "exec - Executes any executable file in /bin directory\n";
            temp += "   Usage: exec <executable-file> <executable-arguments>\n\n";

            temp += "cap - Captures server, given root credentials\n";
            temp += "   Usage: cap <username> <password>\n\n";

            temp += "pwd - Prints user's current directory to console\n";
            temp += "   Usage: pwd\n\n";

            temp += "cd - Changes user's current directory\n";
            temp += "   Usage: cd <destination-directory>  OR  cd ..  to move up a directory\n\n";

            temp += "ssh - Connects to new host on network\n";
            temp += "   Usage: ssh <host-IP>\n\n";

            temp += "quit - Quits the game\n";
            temp += "   Usage: quit\n";
        } else if(command == "ls") {
            temp = "PRINT: Unimplemented command: ls";    
        } else if(command == "read" || command == "cat") { //Allowing cat to be an alias for read
            temp = "PRINT: Unimplemented command: read";    
        } else if(command == "exec") {
            temp = "PRINT: Unimplemented command: exec";    
        } else if (command == "cap") {
            temp = "PRINT: Unimplemented command: cap";  
        } else if (command == "pwd") {
            temp = "PRINT: Unimplemented command: pwd";  
        } else if (command == "cd") {
            temp = "PRINT: Unimplemented command: cd"; 
        } else if(command == "ssh") {
            if(arguments.size() != 1) {
                temp = "PRINT: Invalid Argument(s)\nUsage: ssh <ip-address>";
            } else {
                temp = "PRINT: DEBUG: Take me to the server at ip address " + arguments[0];
                //TODO: Create/Initialize virtualServers, check if the one here exists, and then go to it if it does
            }
        } else if(command == "quit") {
            //Quit signals the client and server to both end
            temp = "QUITGAME: ";
            //Sending message to client here so can break without the client not receiving the message
            char* toClient = new char[temp.size()+1];
            std::copy(temp.begin(), temp.end(), toClient);
            toClient[temp.size()] = '\0';
            write(playerSocket, toClient, strlen(toClient));
            break;
        } else {
            //Send all commands back to client if it isn't quit
            temp = "PRINT: Unknown command: " + command;
        }
        //Always send a message back to the player, to prevent it from waiting forever
        char* toClient = new char[temp.size()+1];
        std::copy(temp.begin(), temp.end(), toClient);
        toClient[temp.size()] = '\0';
        write(playerSocket, toClient, strlen(toClient));
    }
}

void initializeServers() {
    //TODO: Decide on how many servers and what their ips are
    for(int i = 2; i <= 10; ++i) { //Starting at 2 because .0 is the network, and .1 is typically a router or something
        VirtualServer vs;
        vs.ip = "192.168.1." + std::to_string(i);
        std::cout << "DEBUG: Server " << i << "IP set to " << vs.ip << std::endl;
        //TODO: Initialize Usernames/Passwords including root username/password
        serverList.insert(virtualServers.end(), vs);
    }
}

//GAME CODE BELOW

//TODO: Pass player here instead of an int for better debug
//NOTE: Sockets are ints because they're expected to be c style socket file descriptors.
void playGame(int p1Socket, int p2Socket) {
    std::string targetSentence = "This is a new test sentence.";
    int p1Index = 0;
    int p2Index = 0;
    const char* messageToSend = targetSentence.c_str();
    write(p1Socket, messageToSend, strlen(messageToSend));
    write(p2Socket, messageToSend, strlen(messageToSend));
    //NOTE: Sockets don't need to be passed as pointers, since they're just file descriptors for the kernel
    bool gameOver = false;
    std::thread listenP1(listenPlayerGame, p1Socket, p2Socket, &p1Index, targetSentence, &gameOver);
    std::thread listenP2(listenPlayerGame, p2Socket, p1Socket, &p2Index, targetSentence, &gameOver);
    //Killing the threads instead of joining them, since when one of them finishes the spinlock will free up.
    listenP1.join();
    listenP2.join();
    if (p1Index > p2Index) {
        std::cout << "\rPlayer 1 wins!" << std::endl;
    } else if (p2Index > p1Index) {
        std::cout << "\rPlayer 2 wins!" << std::endl;
    } else {
        std::cout << "\rPlayers tied!" << std::endl; //NOTE: With the current implementation, this is not technically possible, but if we have some sort of time restriction it's possible both players get to the same character
    }
}


//TODO: Pass player here instead of an int for better debug
//NOTE: This is insecure, poorly coded, and easily exploitable, as you can just flood the server with whatever you want atm.  Goal is to actually check packet contents eventually.
void listenPlayerGame(int playerSocket, int otherSocket, int* index, std::string targetSentence, bool* gameOver) {
    char buffer[100];
    //Check if either player has 'won' every step of the way by checking if the value of the index is > the length of targetSentence
    while(!(*gameOver)) {
        //Receive a packet from either p1 or p2, by having two separate threads handling each
        //Read data from the connection
        memset(buffer, 0, sizeof(buffer)); //Clearing the buffer before each read
        int len = read(playerSocket, buffer, 100); //TODO: Have this read for a char received/cancel everything message, and terminate the thread on char received
        printf("Received %d bytes from socket %d: %s\n", len, playerSocket, buffer); //Prints out receivedMessage
        fflush(stdout);
        if(len == 3) {
            break; //If len is 3, it's just 'ACK' to a 'FIN'
        }
        *index = *index + 1; //Incrementing index whenever a packet is received.
        if(*index >= targetSentence.length()) {
            *gameOver = true;
            buffer[0] = 'F';
            buffer[1] = 'I';
            buffer[2] = 'N';
            buffer[3] = 0;
            //Sending FIN to both players to ensure they both exit their threads
            write(playerSocket, buffer, strlen(buffer));
            write(otherSocket, buffer, strlen(buffer));
        }
    }
}
