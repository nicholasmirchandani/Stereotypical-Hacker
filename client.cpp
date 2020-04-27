#include <iostream>
#include <string>
#include <thread> //for std::thread
//Networking Includes!
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <cstring> //for strlen
#include <stdio.h>

//https://www.tutorialspoint.com/Read-a-character-from-standard-input-without-waiting-for-a-newline-in-Cplusplus

void waitForFIN(int serverSocket, bool* gameOver);

int main() {
    /*Boilerplate Network Code*/

    //Network code taken from Springer and then modified
    // Step 1: Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Step 2: Set the destination information
    struct sockaddr_in dest;
    memset(&dest, 0, sizeof(struct sockaddr_in));
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr("127.0.0.1");  //<-- Host we're connecting to
    dest.sin_port = htons(9090); //<-- Port we're connecting to
    // Step 3: Connect to the server
    int errorCheck = connect(sockfd, (struct sockaddr *)&dest, sizeof(struct sockaddr_in));

    if(errorCheck < 0) {
        std::cout << "ERROR: Couldn't connect to host" << std::endl;
        exit(1);
    }

    /*End Boilerplate network code*/
    bool gameOver = false;
    //Read data from the connection to set targetSentence
    char buffer[100];
    memset(buffer, 0, sizeof(buffer)); //Clearing the buffer before each read
    int len = read(sockfd, buffer, 100); //Reading sentence from buffer
    std::string targetSentence = buffer;
    std::thread listenForFIN(waitForFIN, sockfd, &gameOver);
    char userInput = 0;
    //TODO: Have this receive "terminate" packets to allow the server to asynchronously terminate
    for(int i = 0; i < targetSentence.length() && !gameOver; ++i) {
        while(targetSentence[i] != userInput) {
            //TODO: Check for server terminate here
            //Alternating the terminal between "cooked" (To output as usual) and "raw" (To take individual characters as inputs)
            system("stty cooked");
            if(gameOver) {
                break;
            }
            std::cout << "\rPlease enter the character: " << targetSentence[i] << '\n'; //DEBUG: Will be replaced by Logan tilemap code once that's up and running.
            system("stty raw");
            userInput = getchar();
        }
        if(gameOver) {
            break;
        }
        //Sending server message about character user entered
        //Step 4: Send data to the server, using c strings because c sockets
        std::string message = "Character entered: " + userInput;
        const char* messageToSend = message.c_str();
        write(sockfd, messageToSend, strlen(messageToSend));
    }
    system("stty cooked"); //Swapping back the terminal to "cooked" to ensure terminal behaves normally upon exiting the program
    listenForFIN.join();
    // Step 5: Close the connection
    close(sockfd);
    return 0;
}

void waitForFIN(int serverSocket, bool* gameOver) {
    char buffer[100];
    //Check if either player has 'won' every step of the way by checking if the value of the index is > the length of targetSentence
    while(!(*gameOver)) {
        //Receive a packet from either p1 or p2, by having two separate threads handling each
        //Read data from the connection
        memset(buffer, 0, sizeof(buffer)); //Clearing the buffer before each read
        int len = read(serverSocket, buffer, 100); //TODO: Have this read for a char received/cancel everything message, and terminate the thread on char received
        if(len == 3) {
            *gameOver = true;
            buffer[0] = 'A';
            buffer[1] = 'C';
            buffer[2] = 'K';
            //Sending FIN to both players to ensure they both exit their threads
            write(serverSocket, buffer, 3);
            break; //If len is 3, it's 'FIN', and not some update on P2's index.
        }
    }
}
