#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
#include <list>
// #include <stdlib.h>  /* rand, srand */
#include <time.h>    /* clock_t, clock, CLOCKS_PER_SEC */
// #include <unordered_map>

#define INFINITY     1000000;

using namespace std;

class Player {

private:
    Side side;
    Side opponentSide;
    Board * board;
    // unordered_map<long, int> trans;

    void getMoves(Board * board, Side side, list<Move *> * moves);
    int evaluateBoard(Board * board, Side side);
    int evaluatePosition(Board * board, Side side);
    int evaluateDisk(int i, int j);
    Move * minimax(int maxDepth);
    int min(Board * board, int depth, int alpha, int beta);
    int max(Board * board, int depth, int alpha, int beta);
    // long zobristHash(Board * board, int depth);

public:
    Player(Side side);
    ~Player();
    
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    // Set player's board. Used in testminimax.
    void setPlayerBoard(char data[]) { this -> board -> setBoard(data); };
};

#endif
