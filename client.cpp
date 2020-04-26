#include <iostream>
#include <string>
#include <stdio.h>
//Networking Includes!
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <cstring> //for strlen

//https://www.tutorialspoint.com/Read-a-character-from-standard-input-without-waiting-for-a-newline-in-Cplusplus

int main() {
    /*Boilerplate Network Code*/

    //Network code taken from Springer and then modified
    // Step 1: Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Step 2: Set the destination information
    struct sockaddr_in dest;
    memset(&dest, 0, sizeof(struct sockaddr_in));
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr("10.0.2.69");  //<-- Host we're connecting to
    dest.sin_port = htons(9090); //<-- Port we're connecting to
    // Step 3: Connect to the server
    connect(sockfd, (struct sockaddr *)&dest, sizeof(struct sockaddr_in));

    /*End Boilerplate network code*/
    std::string targetSentence = "This is a test sentence.";
    bool isPlayer1 = true;
    char userInput = 0;
    for(int i = 0; i < targetSentence.length(); ++i) {
        while(targetSentence[i] != userInput) {
            //Alternating the terminal between "cooked" (To output as usual) and "raw" (To take individual characters as inputs)
            system("stty cooked");
            std::cout << "\rPlayer " << (isPlayer1 ? "1" : "2" ) << ": Please enter the character: " << targetSentence[i] << '\n'; //DEBUG: Will be replaced by Logan tilemap code once that's up and running.
            system("stty raw");
            userInput = getchar();
        }
        //Sending server message about character user entered
        //Step 4: Send data to the server, using c strings because c sockets
        std::string message = "Character entered: " + userInput;
        const char* messageToSend = message.c_str();
        write(sockfd, message, strlen(message));
    }
    system("stty cooked"); //Swapping back the terminal to "cooked" to ensure terminal behaves normally upon exiting the program
    // Step 5: Close the connection
    close(sockfd);
    return 0;
}