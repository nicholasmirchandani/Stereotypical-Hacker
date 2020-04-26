#include <iostream>
#include <string>
#include <thread> //for std::thread
#include <exception> //for std::terminate
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

//NOTE: Just about everything in main is boilerplate code that won't be used in the final game.  just playGame; everything else should be handled by Lloyd and Alex
int main() {
    //Socket code taken from springer and then modified
    int listeningSocket, p1Socket, p2Socket;
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
    int client_len = sizeof(struct sockaddr_in);
    p1Socket = accept(listeningSocket, (struct sockaddr*)&p1Address, &client_len);
    p2Socket = accept(listeningSocket, (struct sockaddr*)&p2Address, &client_len);

    //NOTE: Reading from the connections within the game itself.
    playGame(p1Socket, p2Socket);

    // Step 6: Close the connections
    close(p2Socket);
    close(p1Socket) 
    close(listeningSocket);
    return 0;
}

//NOTE: Sockets are ints because they're expected to be c style socket file descriptors.
void playGame(int p1Socket, int p2Socket) {
    std::string targetSentence = "This is a test sentence.";
    int p1Index = 0;
    int p2Index = 0;
    //NOTE: Sockets don't need to be passed as pointers, since they're just file descriptors for the kernel
    bool gameOver = false;
    std::thread listenP1(listenPlayer, p1Index, &p1Index, targetSentence, &gameOver);
    std::thread listenP2(listenPlayer, p2Index, &p2Index, targetSentence, &gameOver);
    
    while(!gameOver); //Spinlocking on gameOver

    //Killing the threads instead of joining them, since when one of them finishes the spinlock will free up.
    listenP1.terminate();
    listenP2.terminate();
    if (p1Index > p2Index) {
        std::cout << "\rPlayer 1 wins!" << std::endl;
    } else if (p2Index > p1Index) {
        std::cout << "\rPlayer 2 wins!" << std::endl;
    } else {
        std::cout << "\rPlayers tied!" << std::endl; //NOTE: With the current implementation, this is not technically possible, but if we have some sort of time restriction it's possible both players get to the same character
    }
}


//NOTE: This is insecure, poorly coded, and easily exploitable, as you can just flood the server with whatever you want atm.  Goal is to actually check packet contents eventually.
void listenPlayer(int playerSocket, int* index, std::string targetSentence, bool* gameOver) {
    char buffer[100];
    //Check if either player has 'won' every step of the way by checking if the value of the index is > the length of targetSentence
    while(!(*gameOver)) {
        //Receive a packet from either p1 or p2, by having two separate threads handling each
        //Read data from the connection
        memset(buffer, 0, sizeof(buffer)); //Sets buffer to 0 at all locations, clearing it before each read
        int len = read(p1Socket, buffer, 100);
        printf("Received %d bytes: %s", len, buffer); //Prints out receivedMessage
        *index = *index + 1; //Incrementing index whenever a packet is received.
        if(*index >= targetSentence.length()) {
            *gameOver = true;
        }
    }
}