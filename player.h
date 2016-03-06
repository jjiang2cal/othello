#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
#include <list>
#include <stdlib.h>   /* srand, rand */
#include <time.h>
using namespace std;

class Player {

private:
	Side side;
    Side opponentSide;
    Board * board;

    list<Move *> getMoves(Board * board, Side side);
    int evaluateBoard(Board * board, Side side);
    int evaluateDisk(int i, int j);

public:
    Player(Side side);
    ~Player();
    
    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
