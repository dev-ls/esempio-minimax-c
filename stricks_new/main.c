#include <stdio.h>
#include "consts.h"
#include "MinMaxTree.h"
#include <locale.h>

const int DEPTH = IA_LEVEL;
int const NUM_OF_MIN_STICKS_REMOVABLE = NUM_MIN_OF_STICKS_REMOVABLE;


void cleanBuffer() {
    // flushes the standard input
    // (clears the input buffer)
    while ((getchar()) != '\n');
}

int checkGameEnd(remaningStiks, playerNumber) {
    if (remaningStiks == 0) {
        printf("\n-------");
        if (playerNumber == 1) {
            printf("you win");
        }
        else {
            printf("you lose");
        }
        printf("-------\n");
        return playerNumber;
    }
    else if (remaningStiks<=0) {
        printf("\n-------");
        if (playerNumber != 1) {
            printf("you win");
        }
        else {
            printf("you lose");
        }
        printf("-------\n");
        return playerNumber;
    }
    printf("-----game still ongoing, %i sticks left----", remaningStiks);
    return 0;
}

int checkIfIsAValidNumberParameter(int val) {
    if (val>0) {
        return 1;
    }
    return -1;
}

int integerFromBooleanChar(char c) {
    if (c == 'y' || c == 'Y' || c == 'yes' || c == 'YES') {
        return  1;
    }else if (c == 'n' || c == 'N' || c == 'no' || c == 'NO') {
        return -1;
    }
    return 0;
}

void play(numberOfSticks, numberOfSticksRemovable) {
    int suggestedDepth = 6;//calcDepth(numberOfSticks, numberOfSticksRemovable);
    int selectedDepth=0;
    char debugChoice;
    int debug = 0;
    while (debug == 0) {
        printf("\ndo you want to enable debug? (y/n)");
        scanf_s("%c", &debugChoice);
        debug = integerFromBooleanChar(debugChoice);
        cleanBuffer();
    }

    while (checkIfIsAValidNumberParameter(selectedDepth) == -1) {
        printf("\ninsert how much IA should be smart. I suggest you to go for %i. if you will insert 0 will be used %i as default\n", suggestedDepth, DEPTH);
        scanf_s("%i", &selectedDepth);
        if (selectedDepth == 0) {
            selectedDepth = DEPTH;
        }
        cleanBuffer();
    }

    MinMaxTreeConf conf = { numberOfSticksRemovable,  selectedDepth,debug };//it will cals next IA_LEVEL possible action
    int currentPlayerNumber = 1;
    while (numberOfSticks > 0) {
        int userInput=0;
        while (userInput<=0 || userInput> numberOfSticksRemovable) {
            printf("\nchose a number between 1 and %i\n", numberOfSticksRemovable);
            scanf_s("%i",&userInput);
            cleanBuffer();
        }
        numberOfSticks -= userInput;
        if (checkGameEnd(numberOfSticks, currentPlayerNumber)==0) {

            Node root = { conf.depth,currentPlayerNumber,numberOfSticks };
            createChildren(&root, conf);

            currentPlayerNumber *= -1;
            numberOfSticks -=calcChoice(root, currentPlayerNumber,conf);
            freeAllNodes(&root, conf);

            if (checkGameEnd(numberOfSticks, currentPlayerNumber) != 0) {
                return;
            }
            currentPlayerNumber *= -1;
        }


    }
}

void main() {
    setlocale(LC_ALL, "");//utf-8
    int numberOfSticksRemovable=0;
    int numberOfSticks=0;

    printf("To win you must get exactly 0 sticks; you lose in case the pc reaches 0 or if you arrive at a negative number of sticks.\n\n");

    const int defaultNumberOfSticks = NUM_MIN_OF_STICKS;
    while (checkIfIsAValidNumberParameter(numberOfSticks)==-1 || numberOfSticks < defaultNumberOfSticks) {
        printf("\ninsert how many sticks do you want to use to play\n1) if you will insert 0 will be used %i as default\n2) any number lower than the default one will be rejected.\n\n", defaultNumberOfSticks);
        scanf_s("%i", &numberOfSticks);
        if (numberOfSticks == 0) {
            numberOfSticks = defaultNumberOfSticks;
        }
        cleanBuffer();
    }
    while (checkIfIsAValidNumberParameter(numberOfSticksRemovable) == -1 || (numberOfSticksRemovable< NUM_OF_MIN_STICKS_REMOVABLE|| numberOfSticks<= numberOfSticksRemovable)) {
        printf("\ninsert how many sticks do you want to be able to take on each turn\n1) if you will insert 0 will be used %i as default\n2) any number lower than the default one or greater than sticks number will be rejected.\n\n", NUM_OF_MIN_STICKS_REMOVABLE);
        scanf_s("%i", &numberOfSticksRemovable);
        if (numberOfSticksRemovable == 0) {
            numberOfSticksRemovable = NUM_OF_MIN_STICKS_REMOVABLE;
        }
        cleanBuffer();
    }

    play(numberOfSticks, numberOfSticksRemovable);
    getchar();
}