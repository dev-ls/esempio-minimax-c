//
// Created by lsidari on 29/11/2018.
//

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "MinMaxTree.h"



//children is a pointer of pointes, so you've to get the value (*) of children[i]
Node getNodeValueFromChildren(Node node, int index) {
    return *((Node*)node.children[index]);//(Node*) is a cast to ensure node.children[index] is a pointer to a Node -> get value of a node pointer from children[i]
}

/*
in c doesn't exists inf, so we'll get maxint;
playerNumber is used to obtain -inf or +inf
*/
int getRealVal(int remainingStiksAfterAtLeastOneIsPickedUp,int playerNumber) {
    if (remainingStiksAfterAtLeastOneIsPickedUp < 0)//if remaning stiks are less than 0 set heuristic value to opposite infinite
        return INT_MAX * -playerNumber;
    if (remainingStiksAfterAtLeastOneIsPickedUp == 0) {//if this will make you win then return your inf.
        return INT_MAX * playerNumber;
    }
    return 0;//if this value state doesn't allow you to win, it doesn't metter
}

/*
 creates all possible moves until a certain depths;
 each node contains an Heustic value which rapresents the game states.
 In this way the pc can chose the best option in order to win.

 Let's suppose that we're the player number one, so pc is the num. -1.
 PC's goal is to chose a node which values -inf or 0.
*/
void createChildren(Node *currentNode, MinMaxTreeConf minMaxTreeConf) {
    /*
    until depth>0 this func will create a node which will be passed as actual parameters for x times in this func recursively
    */
    if (currentNode->depth >= 0) {
        currentNode->children = malloc(minMaxTreeConf.numberOfSticksRemovable * sizeof(Node));

        /*create minMaxTreeConf.numberOfSticksRemovable node child for each node child recursively, then add it on the provided Node*/
        for (int i = 0;i < minMaxTreeConf.numberOfSticksRemovable;i++) {
            int remainingStiksAfterAtLeastOneIsPickedUp = currentNode->remaningStriks - (i+1);
            int playerNumber = -currentNode->playerNumber;//flip user
            Node *child=malloc(sizeof(Node));
            child->depth = currentNode->depth - 1;//decrease depth
            child->gameState = getRealVal(remainingStiksAfterAtLeastOneIsPickedUp, playerNumber);//calc game state for this case
            child->remaningStriks = remainingStiksAfterAtLeastOneIsPickedUp;//remaning sticks if this action will be performed
            child->playerNumber = playerNumber;

            createChildren(child, minMaxTreeConf);//create child's children
            currentNode->children[i] = child;
        }
    }
}


void freeAllNodes(Node *currentNode, MinMaxTreeConf minMaxTreeConf) {
    if (currentNode->depth >= 0) {
        int x = minMaxTreeConf.numberOfSticksRemovable;
        for (int i = 0;i < x;i++) {
            freeAllNodes((Node*)currentNode->children[i], minMaxTreeConf);
            free(currentNode->children[i]);
        }
        free(currentNode->children);
    }
}

int calcMinMax(Node node,int depth,int playerNumber, MinMaxTreeConf minMaxTreeConf) {
    int nodeValue=node.gameState,bestValue;
    //checks if this is the last node or nodeValue values an inf state (he will win the game with that action)
    if ((node.depth == 0) || (abs(nodeValue) == INT_MAX)) {
        return nodeValue;
    }
    /*
        pc have to look for best value; at start best value is set to the bestValue for the other player.
        It's like set best to the wost possible value;
    */
    bestValue = INT_MAX * -playerNumber;
    //serach in each node and in every node's child in order to obtain bestValue
    for (int i = 0, x = minMaxTreeConf.numberOfSticksRemovable;i<x;i++) {
        Node child = getNodeValueFromChildren(node, i);//gets child value from a vector of pointers
        nodeValue = calcMinMax(child,depth-1,-playerNumber,minMaxTreeConf);//for each depth filps user in order to forecast which moves allows who to win
        //is pc (-inf - nodeValue) value greater than (-inf - (+inf or bestValue))?
        if (abs(INT_MAX*playerNumber - nodeValue) < abs(INT_MAX*playerNumber - bestValue)) {
            bestValue = nodeValue;
        }
    }
    if (minMaxTreeConf.debugEnabled == 1) {
        printf("\n[realVal nearest to inf %i (abs(MAXINT*playerNumber - bestValue)=%i) has been retrived from depth %i for player %i]", bestValue, abs(INT_MAX*playerNumber - bestValue), depth, playerNumber);
    }
    return bestValue;
}


int calcChoice(Node rootWithPreviusGameState,int currentPlayerNumber,MinMaxTreeConf minMaxTreeConf) {
    int playerNumber= currentPlayerNumber, depth=rootWithPreviusGameState.depth, nodeValue = rootWithPreviusGameState.gameState,
    //set as best the worst option possible, as the other player will win
            bestValue = INT_MAX * -playerNumber,
            bestChoice = INT_MAX * -playerNumber;
    printf("\n\nI'm going to search for the nearest inf value from each children at any depth, then I'll return root's child in which is contained");
    for (int i = 0, x = minMaxTreeConf.numberOfSticksRemovable;i < x;i++) {
        Node child = getNodeValueFromChildren(rootWithPreviusGameState, i);
        //calc opposite player (pc) minMax value
        nodeValue = calcMinMax(child, depth, -playerNumber, minMaxTreeConf);//checks in each children of each node which realValue is nearest to pc inf (-inf for pc and +inf for player)
        int absNodeValue = abs(INT_MAX*playerNumber - nodeValue),absBestValue= abs(INT_MAX*playerNumber - bestValue);
        if (absNodeValue <=absBestValue) {
            bestValue = nodeValue;
            bestChoice = i+1;//adds 1 in order to get the real stick number
            if (minMaxTreeConf.debugEnabled == 1) {
                printf("\n\n[[actual cals was: abs(MAXINT*(%i) - %i) [=%i]<= abs(MAXINT*(%i) - %i)[=%i]]]\n\n", playerNumber, nodeValue, absNodeValue, playerNumber, bestValue,absBestValue);
                printf("\n\n[[possible bestChoice found: %i]]\n\n", bestChoice);
            }
        }
    }
    printf("\n\n[bestChoice %i with value realVal %i (abs(MAXINT*playerNumber - bestValue)=%i) has been retrived from depth %i for player %i]\n\n", bestChoice, bestValue, abs(INT_MAX*playerNumber - bestValue), depth, currentPlayerNumber);
    return bestChoice;
}
/*
int calcNodes(depth, numberOfSticksRemovable) {
	int ris = 0;
	for (;depth > 0;depth--)ris += pow(numberOfSticksRemovable, depth);
	return ris;
}

int calcDepth(numberOfSticks, numberOfSticksRemovable) {
	int safeSupported = calcNodes(DEPTH, NUM_OF_MIN_STICKS);
	int requested = calcNodes(numberOfSticks, numberOfSticksRemovable);

	if (safeSupported > requested) {
		return numberOfSticks;
	}
	else {

	}
}
*/
