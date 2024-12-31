// Can run g++ CardGame.cpp TestGame.cpp to compile

#include "CardGame.h"
#include <cstdlib>

int main(int argc, char* argv[]) {

    int seed;
    cout << "Enter a seed" << endl;
    cin >> seed;
    srand(seed);

    int numCPUs;
    cout << "How many CPUs?" << endl;
    cin >> numCPUs;
    while(numCPUs < 1 || numCPUs > 4) {
        cout << "Please select a number between 1 and 4: " << endl;
        cin >> numCPUs;
    }

    GameHandler game(numCPUs + 1);

    int choice;

    while(!game.IsGameOver()) {
        game.GetCurrentPlayer()->TakeTurn(game);
        game.AdvanceTurn();
    }

    game.PrintResults();  
}