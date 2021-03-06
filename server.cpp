#include <iostream>
#include <string>
#include <cstring> //for memset
#include <thread> //for std::thread
#include <exception> //for std::terminate
#include <vector> //for std::vector
#include <fstream> //For ifstream
//Networking Includes!
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
//TODO: Move custom includes to the top and throw necessary includes in there to prevent this clusterfuck
#include "VirtualServer.h"  //No need to include Player, as it's included in VirtualServer

void initializeServers();
bool playGame(int p1Socket, int p2Socket, std::string targetSentence);
void listenPlayerGame(int playerSocket, int otherSocket, int* index, std::string targetSentence, bool* gameOver);
void listenPlayer(Player* player);

std::vector<VirtualServer> serverList;
std::vector<Player*> players;

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
    std::thread listenP1(listenPlayer, &p1);
    players.insert(players.end(), &p1); //Pointer so it's everywhere
    p2.socket = accept(listeningSocket, (struct sockaddr*)&p2Address, &client_len);
    std::thread listenP2(listenPlayer, &p2);
    players.insert(players.end(), &p2); //Pointer so it's everywhere

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
void listenPlayer(Player* player) {
    sem_init(&(player->ready), 0, 1);
    char buffer[100];
    memset(buffer, 0, sizeof(buffer)); //Clearing the buffer before each read
    int len = read(player->socket, buffer, 100); //TODO: Have this read for a char received/cancel everything message, and terminate the thread on char received
    printf("Received %d bytes from socket %d: %s\n", len, player->socket, buffer); //Prints out receivedMessage
    fflush(stdout);
    std::string username(buffer);
    player->username = username;
    while(true) {
        sem_post(&(player->ready));
        //Leaving space in here for game code to take place; will add spinlock on playingGame if needed
        while(player->playingGame);
        sem_wait(&(player->ready));
        memset(buffer, 0, sizeof(buffer)); //Clearing the buffer before each read
        int len = read(player->socket, buffer, 100); //TODO: Have this read for a char received/cancel everything message, and terminate the thread on char received
        printf("Command Read: Received %d bytes from socket %d: %s\n", len, player->socket, buffer); //Prints out receivedMessage
        fflush(stdout);
        if(player->playingGame) {
            //If the player is supposed to be playing the game, let it give up teh ready semaphore and the command output will be the game instead
            continue;
        }
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
                        arguments.insert(arguments.end(), temp.substr(j + 1, i-j-1)); //-1 to remove the space
                        j = i;
                    }
                }
            }

            if(j != temp.length() - 1 && command != temp) {
                //Adding the last element, not delineated by space
                arguments.insert(arguments.end(), temp.substr(j + 1));
            }
        }

        std::cout << "COMMAND: " << command << " ";
        for(std::string argument : arguments) {
            std::cout << argument << " ";
        }
        std::cout << std::endl;
        temp = ""; //Reusing temp for optimization;
        if(command == "help") {
            //Help simply sends a long string with all available user commands
            temp = "PRINT: \n  --- USER COMMANDS ---\n\n";

            temp += "leaderboard - Lists all users and their scores\n";
            temp += "   Usage: leaderboard\n\n";
	        
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
        } else if(command == "leaderboard") {
            temp = "PRINT: ";
            temp += "Username: \tScore:\n";
            for(Player* p : players) {
                temp += p->username + "\t" + std::to_string(p->score) + "\n";
            }
        } else if(command == "ls") {
            temp = "PRINT: Unimplemented command: ls";    
        } else if(command == "read" || command == "cat") { //Allowing cat to be an alias for read
            temp = "PRINT: Unimplemented command: read";    
        } else if(command == "exec") {
            if(arguments.size() != 1) {
                temp = "PRINT: Invalid Argument(s)\nUsage: exec <executable>";
            } else {
                if(arguments[0] == "johntheripper") {
                    if(player->currentServer == nullptr) {
                        temp = "PRINT: You don't need to crack your own password";
                    } else {
                        temp = "PRINT: ";
                        for(std::pair<std::string, std::string> user : player->currentServer->users) {
                            temp += user.first + "\t->\t" + user.second + "\n";
                        }
                    }
                } else if(arguments[0] == "pingsweep") {
                    temp = "PRINT: ";
                    for(VirtualServer vs : serverList) {
                        temp += vs.ip + (vs.captured ? " CAPTURED\n" : " Host is up\n");
                    }
                } else {
                    temp = "PRINT: Invalid executable\nCurrent executables: pingsweep johntheripper";
                }
            }
        } else if (command == "cap") {
            if(arguments.size() != 2) {
                temp = "PRINT: Invalid Arguemnt(s)\nUsage: cap <username> <password>";
            } else {
                if(player->currentServer == nullptr) {
                    temp = "PRINT: ERROR: Cannot capture localhost"; 
                } else {
                    if(player->currentServer->captureServer(arguments[0], arguments[1])) {
                        temp = "PRINT: Server Captured";
                        //Upon capturing the server, player's currentServer gets reset as they're booted to localHost, but server keeps currentPlayer to remember who captured it
                        player->currentServer = nullptr;
                        ++(player->score);
                        //TODO: Make player a pointer not a Player so everywhere gets updated and not just the copy
                    } else {
                        temp = "PRINT: Incorrect Username or Password";
                    }
                }
            }
        } else if (command == "pwd") {
            temp = "PRINT: Unimplemented command: pwd";  
        } else if (command == "cd") {
            temp = "PRINT: Unimplemented command: cd"; 
        } else if(command == "ssh") {
            if(arguments.size() != 1) {
                temp = "PRINT: Invalid Argument(s)\nUsage: ssh <ip-address>";
            } else {
                //Check if the argument exists as an ip to a virtual server (targetIndex of -1 meaning no), and then go to it if it does
                int targetIndex = -1;
                for(int i = 0; i < serverList.size(); ++i) {
                    if(serverList[i].ip == arguments[0]) {
                        targetIndex = i;
                    }
                }
                if(targetIndex == -1) {
                    //If it's not found in the serverList, it's either localhost or an invalid IP
                    if(arguments[0] == "localhost" || arguments[0] == "127.0.0.1") {
                        //TODO: Have Player->ChangeServer as a refactor
                        if(player->currentServer != nullptr) {
                            //Disconnect players from whatever server they're on when they connect to a new one.
                            player->currentServer->currentPlayer = nullptr;
                        }
                        player->currentServer = nullptr; //nullptr as a server refers to localHost
                        temp = "PRINT: Connected to localhost";
                    } else {
                        temp = "PRINT: Invalid IP.  Use 'exec pingsweep' to see the ips you can connect to";
                    }
                } else if(serverList[targetIndex].captured) {
                    temp = "PRINT: Cannot ssh to a captured server";    
                } else {
                    if(player->currentServer != nullptr) {
                        //Disconnect players from whatever server they're on when they connect to a new one.
                        player->currentServer->currentPlayer = nullptr;
                    }
                    if(serverList[targetIndex].currentPlayer != nullptr) {
                        Player* otherPlayer = serverList[targetIndex].currentPlayer;
                        //Play game: only need to set appropriate vars of other player
                        serverList[targetIndex].currentPlayer->playingGame = true;
                        sem_wait(&(otherPlayer->ready));

                        //GAME PLAY CODE

                        //Send PLAYGAME: to both clients
                        temp = "PLAYGAME: This is a new test sentence.";
                        char* toClient = new char[temp.size()+1];
                        std::copy(temp.begin(), temp.end(), toClient);
                        toClient[temp.size()] = '\0';
                        write(player->socket, toClient, strlen(toClient));
                        write(otherPlayer->socket, toClient, strlen(toClient));

                        //Waiting for both players to respond with something
                        memset(buffer, 0, sizeof(buffer)); //Clearing the buffer before each read
                        int len = read(player->socket, buffer, 100); //TODO: Have this read for a char received/cancel everything message, and terminate the thread on char received
                        printf("Game Read: Received %d bytes from socket %d: %s\n", len, player->socket, buffer); //Prints out receivedMessage
                        fflush(stdout);

                        memset(buffer, 0, sizeof(buffer)); //Clearing the buffer before each read
                        len = read(otherPlayer->socket, buffer, 100); //TODO: Have this read for a char received/cancel everything message, and terminate the thread on char received
                        printf("Game Read: Received %d bytes from socket %d: %s\n", len, otherPlayer->socket, buffer); //Prints out receivedMessage
                        fflush(stdout);

                        //Once synchronization is out of the way, start the game
                        if(playGame(otherPlayer->socket, player->socket, "This is a new test sentence.")) {
                            //P2(Attacking Player) Won!
                            //Disconnect original player from the server; Connecting winning user to the server is handled below
                            otherPlayer->currentServer = nullptr;
                            serverList[targetIndex].currentPlayer = nullptr;
                            temp = "PRINT: Your server was taken over by another player";
                            char* toClient = new char[temp.size()+1];
                            std::copy(temp.begin(), temp.end(), toClient);
                            toClient[temp.size()] = '\0';
                            write(otherPlayer->socket, toClient, strlen(toClient));
                        } else {
                            //P1(Defending Player) Won
                            //Don't let player connect to the server
                            temp = "PRINT: You lost and thus were kicked from the server " + arguments[0];
                        }

                        otherPlayer->playingGame = false;
                        sem_post(&(otherPlayer->ready));
                    }

                    if(serverList[targetIndex].currentPlayer == nullptr) {
                        //Only connect to the server if nobody's on it (if you won the game or nobody was there in the first place)
                        player->currentServer = &(serverList[targetIndex]);
                        serverList[targetIndex].currentPlayer = player;
                        temp = "PRINT: Connected to server at ip address " + arguments[0];
                    }
                }
            }
        } else if(command == "quit") {
            //Quit signals the client and server to both end
            temp = "QUITGAME: ";
            //Sending message to client here so can break without the client not receiving the message
            char* toClient = new char[temp.size()+1];
            std::copy(temp.begin(), temp.end(), toClient);
            toClient[temp.size()] = '\0';
            write(player->socket, toClient, strlen(toClient));
            break;
        } else {
            //Send all commands back to client if it isn't quit
            temp = "PRINT: Unknown command: " + command;
        }
        //Always send a message back to the player, to prevent it from waiting forever
        char* toClient = new char[temp.size()+1];
        std::copy(temp.begin(), temp.end(), toClient);
        toClient[temp.size()] = '\0';
        write(player->socket, toClient, strlen(toClient));
    }
}

void initializeServers() {
    //TODO: Decide on how many servers and what their ips are
    srand(time(0));
    for(int i = 2; i <= 10; ++i) { //Starting at 2 because .0 is the network, and .1 is typically a router or something
        VirtualServer vs;
        std::vector<std::string> usernames_vec;
        std::vector<std::string> passwords_vec;
        std::ifstream users_is("usernames.txt");
        if(users_is.fail()) {
            std::cout << "usernames.txt could not be opened" << std::endl;
            exit(1);
        }
        std::ifstream passwords_is("passwords.txt");
        if(passwords_is.fail()) {
            std::cout << "passwords.txt could not be opened" << std::endl;
            exit(1);
        }

        std::string temp;
        while(!users_is.eof()) {
            getline(users_is, temp);
            if(temp != "") {
                usernames_vec.insert(usernames_vec.end(), temp);
            }
        }
        while(!passwords_is.eof()) {
            getline(passwords_is, temp);
            if(temp != "") {
                passwords_vec.insert(passwords_vec.end(), temp);
            }
        }

        int numUsers = rand() % 6 + 5; //numUsers is 5-10
        int rootNum = rand() % numUsers;
        for(int j = 0; j < numUsers; ++j) {
        ///If the username isn't unique within the collection, regenerate it
        regenerateUsername:
            std::string tempUser = usernames_vec[rand() % usernames_vec.size()];
            for(std::pair<std::string, std::string> userInfo : vs.users) {
                if(tempUser == userInfo.first) {
                    goto regenerateUsername;
                }
            }
            std::string tempPass = passwords_vec[rand() % usernames_vec.size()];
            vs.users.insert(std::pair<std::string, std::string>(tempUser, tempPass));
            if(j == rootNum) {
                vs.rootUser = tempUser;
                vs.rootPass = tempPass;
            }
        }

        vs.ip = "192.168.1." + std::to_string(i);
        serverList.insert(serverList.end(), vs);
    }
}

//GAME CODE BELOW

//TODO: Pass player here instead of an int for better debug
//NOTE: Sockets are ints because they're expected to be c style socket file descriptors.

//Returns false if p1 wins, true if p2 wins
bool playGame(int p1Socket, int p2Socket, std::string targetSentence) {
    //TODO: Write to both player sockets game start!
    std::string temp = "STARTGAME: ";
    char* toClient = new char[temp.size()+1];
    std::copy(temp.begin(), temp.end(), toClient);
    toClient[temp.size()] = '\0';
    write(p1Socket, toClient, strlen(toClient));
    write(p2Socket, toClient, strlen(toClient));
    std::cout << "DEBUG: Playing game" << std::endl;

    int p1Index = 0;
    int p2Index = 0;
    //NOTE: Sockets don't need to be passed as pointers, since they're just file descriptors for the kernel
    bool gameOver = false;
    std::thread listenP1(listenPlayerGame, p1Socket, p2Socket, &p1Index, targetSentence, &gameOver);
    std::thread listenP2(listenPlayerGame, p2Socket, p1Socket, &p2Index, targetSentence, &gameOver);
    //Killing the threads instead of joining them, since when one of them finishes the spinlock will free up.
    listenP1.join();
    listenP2.join();
    if (p1Index > p2Index) {
        std::cout << "Player 1 wins!" << std::endl;
        return false;
    } else if (p2Index > p1Index) {
        std::cout << "Player 2 wins!" << std::endl;
        return true;
    } else {
        std::cout << "Players tied!  Player 1 holds." << std::endl; //NOTE: With the current implementation, this is not technically possible, but if we have some sort of time restriction it's possible both players get to the same character
        return false;
    }
}


//TODO: Pass player here instead of an int for better debug
//NOTE: This is insecure, poorly coded, and easily exploitable, as you can just flood the server with whatever you want atm.  Goal is to actually check packet contents eventually.
void listenPlayerGame(int playerSocket, int otherSocket, int* index, std::string targetSentence, bool* gameOver) {
    char buffer[28]; //Only using a buffer size of 28 to prevent multiple entries from buffering up
    //Check if either player has 'won' every step of the way by checking if the value of the index is > the length of targetSentence
    while(!(*gameOver)) {
        //Receive a packet from either p1 or p2, by having two separate threads handling each
        //Read data from the connection
        memset(buffer, 0, sizeof(buffer)); //Clearing the buffer before each read
        int len = read(playerSocket, buffer, 28); //TODO: Have this read for a char received/cancel everything message, and terminate the thread on char received
        printf("Received %d bytes from socket %d: %s\n", len, playerSocket, buffer); //Prints out receivedMessage
        fflush(stdout);
        if(len == 3) {
            std::cout << "ACK RECEIVED!" << std::endl;
            break; //If len is 3, it's just 'ACK' to a 'FIN'
        }
        //Ensure character is correct before incrementing index
        if(len == 28 && buffer[27] == targetSentence[*index]) {
            *index = *index + 1; //Incrementing index whenever a packet is received.
            std::cout << "Player index: " << *index << std::endl;
        }
        if(*index >= targetSentence.length() - 1) {
            *gameOver = true;
            buffer[0] = 'F';
            buffer[1] = 'I';
            buffer[2] = 'N';
            buffer[3] = 0;
            //Sending FIN to both players to ensure they both exit their threads
            std::cout << "SENDING FIN NOW!" << std::endl;
            write(playerSocket, buffer, strlen(buffer));
            write(otherSocket, buffer, strlen(buffer));
        }
    }
}
