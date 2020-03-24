#include <iostream>
#include <string>
#include <stdio.h>

//https://www.tutorialspoint.com/Read-a-character-from-standard-input-without-waiting-for-a-newline-in-Cplusplus

int main() {
    std::string targetSentence = "This is a test sentence.";
    int p1Index = 0;
    int p2Index = 0;
    bool isPlayer1 = true;
    char userInput = 0;
    for(int i = 0; i < targetSentence.length(); ++i) {
        while(targetSentence[i] != userInput) {
            //Alternating the terminal between "cooked" (To output as usual) and "raw" (To take individual characters as inputs)
            system("stty cooked");
            std::cout << "\rPlayer " << (isPlayer1 ? "1" : "2" ) << ": Please enter the character: " << targetSentence[i] << '\n'; //DEBUG: Will be replaced by Logan tilemap code once that's up and running.
            system("stty raw");
            userInput = getchar();
            if(userInput == '\t') {
                std::cout << "\rDEBUG: Player Switch!" << std::endl;
                isPlayer1 = !isPlayer1;
                if(isPlayer1) {
                    i = p1Index;
                } else {
                    i = p2Index;
                }
            }
        }
        if(isPlayer1) {
            ++p1Index;
        } else {
            ++p2Index;
        }
    }
    system("stty cooked"); //Swapping back the terminal to "cooked" to ensure terminal behaves normally upon exiting the program
    if (p1Index > p2Index) {
        std::cout << "\rPlayer 1 wins!" << std::endl;
    } else if (p2Index > p1Index) {
        std::cout << "\rPlayer 2 wins!" << std::endl;
    } else {
        std::cout << "\rPlayers tied!" << std::endl; //NOTE: With the current implementation, this is not technically possible, but if we have some sort of time restriction it's possible both players get to the same character
    }
    return 0;
}