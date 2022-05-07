#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include "headers.h"

// The number of coins in each game; MUST BE EVEN --> both players get the same number
const int NUM_COINS = 10;

// The highest value that a coin's monetary value is allowed to be
const int MAX_COIN_VALUE = 1000;

// The minimum value that a coin's monetary value is allowed to be
const int MIN_COIN_VALUE = 100;

// Allows the user to exit the game
bool quit = false;

// Used to make it easier to distinguish between who's turn it is
std::string border = "-";

/**
    The initial text that the user sees upon viewing the program
*/
void initialPrompt() {
    std::cout << "COINS-IN-A-LINE\n" << std::endl;
    std::cout << "The Basic Coins-in-a-Line Game!  You will be provided with various coins." << std::endl;
    std::cout << "The objective is to take turns with another player and pick coins such that" << std::endl;
    std::cout << "your coin value at the end of the game is higher than your opponent's total." << std::endl;
    std::cout << "Your opponent will be the program itself, in which there are two modes: EASY and HARD.\n" << std::endl;
    std::cout << "Regardless of the mode," << std::endl;
    std::cout << "\tPRESS 1 and ENTER to select the coin at the left" << std::endl;
    std::cout << "\tPRESS 0 and ENTER to select the coin at the right\n" << std::endl;
    std::cout << "Enjoy the game!\n" << std::endl;

    displayOptions();
}

/**
    Prompts the user to select an option
*/
void displayOptions() {
    std::cout << "PRESS 1 and ENTER to start." << std::endl;
    std::cout << "PRESS 2 and ENTER to view details about the EASY and HARD modes." << std::endl;
    std::cout << "PRESS 3 and ENTER to quit." << std::endl;

    std::string value;
    std::cin >> value;

    options(value);
}

/**
    Handles the various choices the user can make

    @param s    the string option that the user has chosen
*/
void options(std::string s) {
    if(s.length() <= 0) {
        displayOptions();
        return;
    }

    switch(s.at(0)) {
        case '1': // go to the game (user should select game mode first)
            modes();
            break;
        case '2': // displays additional information about EASY and HARD game modes
            gameModeInformation();
            break;
        case '3': // allows the user to exit the program
            quit = true;
            break;
        default: // input is not valid; bring up prompt again
            displayOptions();
            break;
    }
}

/**
    Prompts the user to select a game mode (EASY or HARD)
*/
void modes() {
    std::cout << "\nPRESS 1 and ENTER for EASY Mode." << std::endl;
    std::cout << "PRESS 2 and ENTER for HARD Mode." << std::endl;
    std::cout << "PRESS 3 and ENTER for CUSTOM Mode." << std::endl;

    std::string mode;
    std::cin >> mode;

    if(mode.length() <= 0) {
        modes();
        return;
    }

    switch(mode.at(0)) {
    case '1':
        playGame(0);
        break;
    case '2':
        playGame(9);
        break;
    case '3':
    {
        std::cout << "You may select any number (one digit) from 0-9, with 0 = EASY Mode and 9 = HARD Mode." << std::endl;
        std::cout << "Select a number from 0 - 9 (inclusive of 0 and 9):" << std::endl;
        std::string difficulty;
        std::cin >> difficulty;
        if(difficulty.at(0) >= '0' && difficulty.at(0) <= '9')
            playGame(difficulty.at(0) - '0'); // get integer representation (subtracting '0')
        else {
            std::cout << "Game mode not selected.  Default of value 5 selected." << std::endl;
            playGame(5);
        }
    }
        break;
    default:
        modes();
        break;
    }
}

/**
    Displays additional information about the game modes
*/
void gameModeInformation() {
    std::cout << "\nEASY Mode is based on a Greedy Algorithm which basically looks at the coin on" << std::endl;
    std::cout << "the most left and the most right sides of the board (screen).  It will pick the" << std::endl;
    std::cout << "coin with the highest value and repeatedly commit to this process until there are" << std::endl;
    std::cout << "no coins left.  The reason this approach is marked as 'EASY' is because picking" << std::endl;
    std::cout << "the coin with the largest value every turn is not always the best option.  One example" << std::endl;
    std::cout << "of this reasoning is as follows:" << std::endl;
    std::cout << "\n\t\t\t10\t5\tINFINITY\t100\n" << std::endl;
    std::cout << "Let's say the person using the greedy strategy goes first.  They will choose" << std::endl;
    std::cout << "value 100.  This move leaves the next player with the option to choose INFINITY." << std::endl;
    std::cout << "In most strategies, choosing the INFINITY coin is optimal.  Assuming the second player" << std::endl;
    std::cout << "does in fact choose this INFINITY coin, the first player (who was using the greedy strategy)" << std::endl;
    std::cout << "has lost the game.\n" << std::endl;

    std::cout << "HARD Mode is based on Dynamic Programming and is harder to beat because it uses an algorithm" << std::endl;
    std::cout << "to put the user of the algorithm in the more favorable position while disadvantaging the" << std::endl;
    std::cout << "other player.  In this mode, it is pretty much impossible to beat this DP solution, especially" << std::endl;
    std::cout << "considering that the opponent (who uses the DP algorithm) goes first, which is why this mode is only" << std::endl;
    std::cout << "recommended for those who are interested in seeing how the algorithm works.\n" << std::endl;


    std::cout << "CUSTOM Mode is a mode in which the user picks the difficulty.  It is a combination of both" << std::endl;
    std::cout << "the EASY and HARD modes, in which the user picks a number from 0-9, with lower numbers" << std::endl;
    std::cout << "being closer to the EASY Mode and those higher up being closer to the HARD Mode of the game.\n" << std::endl;

    displayOptions();
}

/**
    Generates an array to store pseudo-random numbers representing coin values for the game
*/
int * generateCoinValues() {
    srand(time(0));

    // The array of coins, with a -1 value representing a coin that has been removed
    static int coinValues[NUM_COINS];

    for(int i = 0; i < NUM_COINS; i++) {
        coinValues[i] = rand();

        // Readjust max coin value
        if(coinValues[i] > MAX_COIN_VALUE) {
            while(coinValues[i] > MAX_COIN_VALUE)
                coinValues[i] /= 10;
        } else if(coinValues[i] < MIN_COIN_VALUE) {
            while(coinValues[i] < MIN_COIN_VALUE)
                coinValues[i] *= 10;
        }
    }

    /* I've noticed that coins are taken substantially from one side more than the other
    in any given game, so here let's switch the coin values on each end of the line
    */
    int r = rand();
    int i1 = -1; // used to find first index to switch
    int i2 = -1; // used to find second index to switch
    for(/* 'r' value */ ; r > 0; r /= 10) {
        if(i1 == -1 && i2 == -1) {
            i1 = r % 10;
            if(i1 > NUM_COINS - 1) // greater than last index
                i1 = NUM_COINS - 1;
        } else if(i2 == -1) {
            i1 = r % 10;
            if(i2 > NUM_COINS - 1) // greater than last index
                i2 = NUM_COINS - 1;
        }

        // Now switch the value positions
        if(i1 != -1 && i2 != -1) {
            int temp = coinValues[i1];
            coinValues[i1] = coinValues[NUM_COINS - i2 - 1];
            coinValues[i2] = temp;
        }
    }

    return coinValues;
}

/**
    Displays the coins on the screen for the player to see

    @param coinArray    the list of coins which should be displayed on the user's screen
*/
void displayCoinValues(int * coinArray) {
    std::cout << "\t\t";

    // Iterate through all of the coins
    for(int i = 0; i < NUM_COINS; i++) {

        std::cout << "\t"; // This line here to keep coins in same positions on screen

        // Only add coins that are still in play (-1 = already picked)
        if(coinArray[i] != -1) {
            std::cout << *(coinArray + i);
        } else {
            // This line here to keep coins in same positions on screen
            std::cout << "   "; // 3 spaces (99.9% of coin values will be 3 digits)
        }
    }

    std::cout << std::endl;
}

/**
    Displays a visual indicating who's turn it is

    @param playerTurn   determines who's name (user or program) to display on screen
*/
void displayTurn(bool playerTurn) {
    if(playerTurn)
        std::cout << "Current Turn: " << "Player (You)" << std::endl;
    else
        std::cout << "Current Turn: " << "Opponent" << std::endl;
}

/**
    Allows the user to select a coin and makes calculations to add that coin to the player's total value

    @param coinArray    the coins that are in the game
    @param playerValue  the player's current monetary value based on coins chosen
    @param left         the left index of the coinArray that is still in play (i.e. can still be selected)
    @param right        the right index of the coinArray that is still in play (i.e. can still be selected)
*/
void getUserCoin(int * coinArray, int * playerValue, int * left, int * right) {
    // Prompt the user to select a coin
    std::cout << "Choose a coin:" << std::endl;
    std::string option;
    std::cin >> option;

    // Find which side of the coin array the user picked from
    switch(option.at(0)) {
    case '1': // remove from left
        *playerValue += *(coinArray + (*left));
        *(coinArray + (*left)) = -1;
        (*left)++;
        break;
    case '0':
        *playerValue += *(coinArray + (*right));
        *(coinArray + (*right)) = -1;
        (*right)--;
        break;
    default:
        std::cout << "No valid selection --> choice randomly chosen." << std::endl;
        int r = rand();
        if(r % 2) { // = 1, then left
            *playerValue += *(coinArray + (*left));
            *(coinArray + (*left)) = -1;
            (*left++);
        } else {
            *playerValue += *(coinArray + (*right));
            *(coinArray + (*right)) = -1;
            (*right)--;
        }
    }
}

/**
    Finds out which side of the line of coins (left or right) to select from and updates the opponent's score

    @param selectLeft   TRUE if opponent should select the left coin, FALSE if right coin
    @param CPUValue     the current value of the opponent's score
    @param coinArray    contains all of the coins in the game
    @param left         the left side (index) of the coinArray
    @param right        the right side (index) of the coinArray
*/
void makeCPUMove(bool selectLeft, int * CPUValue, int * coinArray, int * left, int * right) {
    if(selectLeft) {
        *CPUValue += *(coinArray + (*left));
        *(coinArray + (*left)) = -1;
        (*left)++;
    } else {
        *CPUValue += *(coinArray + *(right));
        *(coinArray + (*right)) = -1;
        (*right)--;
    }
}

/**
    Shows (on screen) the winner of the coins-in-a-line game

    @param  playerValue     the value that the player achieved at the end of the game
    @param CPUValue         the value that the program achieved at the end of the game
*/
void displayWinner(int playerValue, int CPUValue) {
    std::cout << "Player Total:   " << playerValue << std::endl;
    std::cout << "Opponent Total: " << CPUValue << std::endl;
    std::cout << "Winner: ";
    if(playerValue > CPUValue)
        std::cout << "Player" << std::endl;
    else if(playerValue < CPUValue)
        std::cout << "Opponent" << std::endl;
    else
        std::cout << "Tie" << std::endl;
}

/**
    Calculates an array (pointers) indicating which moves to make

    @param myLeftSum    used to get the left sum (selecting the left coin) of the initial call to this method
    @param myRightSum   used to get the right sum (selecting the right coin) of the initial call to this method
    @param storedVals   stores a two-dimensional array of different coin variations
    @param coins        all of the coins in the game
    @param l            the left side (index) of the array in this particular stack call
    @param r            the right side (index) of the array in this particular stack call
*/
int optimalMove(int * myLeftSum, int * myRightSum, int storedVals[NUM_COINS][NUM_COINS], int * coins, int l, int r) {
    // Indexes cross
    if(l >= r)
        return 0;

    // Right next to each other, so just choose maximum coin
    if(l + 1 == r) {
        *myLeftSum = *(coins + l);
        *myRightSum = *(coins + r);
        storedVals[l][r] = std::max(*myLeftSum, *myRightSum);
        return storedVals[l][r];
    }

    // Already computed
    if(storedVals[l][r] != 0)
        return storedVals[l][r];

    // Assumes player playing optimally, so take minimum amount
    int leftSum = std::min( optimalMove(myLeftSum, myRightSum, storedVals, coins, l + 1, r - 1), optimalMove(myLeftSum, myRightSum, storedVals, coins, l + 2, r) ) + *(coins + l);

    // Assumes player playing optimally, so take minimum amount
    int rightSum = std::min( optimalMove(myLeftSum, myRightSum, storedVals, coins, l, r - 2), optimalMove(myLeftSum, myRightSum, storedVals, coins, l + 1, r - 1) ) + *(coins + r);

    storedVals[l][r] = std::max(leftSum, rightSum);

    *myLeftSum = leftSum;
    *myRightSum = rightSum;
    return std::max(leftSum, rightSum);
}

/**
    The actual game that the user will play.  EASY Mode is represented by a greedy algorithm
    which always chooses the coin with highest value.  HARD Mode is a strategy consisting of
    dynamic programming in which the optimal coin is picked such that the chosen value
    maximizes the user's score while minimizing the opponent's score

    @param difficulty   on a scale of 0 - 9, represents how hard the game is, with:
                            0   = EASY Mode
                            9   = HARD Mode
                            1-8 = Increasing Difficulty
*/
void playGame(int difficulty) {
    if(difficulty < 0)
        difficulty = 0;
    else if(difficulty > 9)
        difficulty = 9;

    int * coinArray = generateCoinValues();
    int playerValue = 0; // total of player's coins
    int CPUValue = 0; // total of the opponent's coins
    bool playerTurn = false; // false if CPU's turn, true if player's turn
    int left = 0; // index of left side of array that still has a coin
    int right = NUM_COINS - 1; // index of right side of array that still has a coin

    int storedVals[NUM_COINS][NUM_COINS];

    while(left < right) {
        std::cout << "\n" << border << "\n" << std::endl;

        // Display whose turn it is
        displayTurn(playerTurn);

        // Show all of the coins
        displayCoinValues(coinArray);

        // Allow the user or CPU to pick a coin
        if(playerTurn) {
            // Get the chosen coin
            getUserCoin(coinArray, &playerValue, &left, &right);
        } else { // DYNAMIC PROGRAMMING
            srand(time(0));
            int random = rand();
            bool b = true;

            if(difficulty != 0 && (random % 10) >= 9 - difficulty) {
                //  Reset all storedVals elements to 0
                for(int i = 0; i < NUM_COINS; i++) {
                    for(int j = 0; j < NUM_COINS; j++) {
                        storedVals[i][j] = 0;
                    }
                }

                // Find which side to take a coin from
                int myLeftSum = 0;
                int myRightSum = 0;
                int value = optimalMove(&myLeftSum, &myRightSum, storedVals, coinArray, left, right);
                int choice = myLeftSum > myRightSum ? left : right;

                b = choice == left;
            } else {
                b = *(coinArray + left) > *(coinArray + right);
            }

            makeCPUMove(b, &CPUValue, coinArray, &left, &right);
        }

        // Switch turns
        playerTurn = !playerTurn;

        std::cout << "Player Total:   " << playerValue << std::endl;
        std::cout << "Opponent Total: " << CPUValue << std::endl;
    }

    int index = *(coinArray + left) != -1 ? left : right;
    if(playerTurn)
        playerValue += *(coinArray + index);
    else
        CPUValue += *(coinArray + index);

    std::cout << "\n" << border << "\n" << std::endl;

    displayWinner(playerValue, CPUValue);

    std::cout << "\n\n\n\n\n" << std::endl;
}

/**
    Point of Entry ~
        Simply runs the program with a loop until the user/ player is done playing
*/
int main()
{
    // Makes the border longer
    for(int i = 0; i < 50; i++)
        border += (" -"); // probably not the best practice, but it's only done once

    /* Keep iterating until the player is done
    Due to function calls, this loop is probably
    unnecessary, but it is still nice to have. */
    while(!quit)
        initialPrompt();

    return 0;
}
