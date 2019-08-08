//
// Created by lsidari on 29/11/2018.
//
#ifndef MINMAXTREE_H
#define MINMAXTREE_H

typedef struct TNode {
    int depth;//how much the node is depth in tree
    int playerNumber;
    int remaningStriks;
    int gameState;//heuristic value (game state)
    struct Node **children;

} Node;

typedef struct TMinMaxTreeConf {
    int numberOfSticksRemovable;
    int depth;
    int debugEnabled;
}MinMaxTreeConf;

void createChildren(Node *currentNode, MinMaxTreeConf minMaxTreeConf);
void freeAllNodes(Node *currentNode, MinMaxTreeConf minMaxTreeConf);
int calcChoice(Node rootWithPreviusGameState,int currentPlayerNumber,MinMaxTreeConf minMaxTreeConf);

#endif
