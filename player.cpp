#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
    // set player's side and opponent's side
    this -> side = side;
    this -> opponentSide = (side == BLACK) ? WHITE : BLACK;

    // set up a board
    this -> board = new Board;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete board;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */ 
    // process the opponent's move
    if (opponentsMove != NULL)
    {
        (this -> board) -> doMove(opponentsMove, this -> opponentSide);
    }

    // do a random move
    list<Move *> moves;        // to store available moves
    moves = this -> getMoves(this -> board, this -> side);
    if (moves.size() != 0)
    {
        int score = -10000;      // a very negative number to start with
        Move * move2;
        list<Move *>::iterator it;
        for (it = moves.begin(); it != moves.end(); it++)
        {
            Move * move3 = *it;
            Board * copyBoard = (this -> board) -> copy();
            copyBoard -> doMove(move3, this -> side);
            int tempScore = this -> evaluateBoard(copyBoard, this -> side);
            if (tempScore > score)
            {
                move2 = move3;
                score = tempScore;
            }
        }
        (this -> board) -> doMove(move2, this -> side);
        return move2;
    }
    // if no move available, pass
    cout << "PASS" << endl;
    return NULL;
}

/**
 * @brief A private helper function which gets available moves.
 *
 * @param  board  this player's board
 *         side   this player's side
 * @return A list containing available moves.
 */
list<Move *> Player::getMoves(Board * board, Side side)
{
    list<Move *> moves;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Move * move = new Move(i, j);
            if (!board -> checkMove(move, side))
                continue;
            moves.push_back(move);
        }
    }
    return moves;
}

int Player::evaluateBoard(Board * board, Side side)
{
    int score = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++) {
            if (board -> get(side, i, j))
            {
                score += this -> evaluateDisk(i, j);
                continue;
            }
            if (board -> get(this -> opponentSide, i, j))
            {
                score -= this -> evaluateDisk(i, j);
            }
        }
    }
    return score;
}

int Player::evaluateDisk(int i, int j)
{
    // reference: http://play-othello.appspot.com/files/Othello.pdf
    int value[8][8] = {
        {100, -20, 10, 5, 5, 10, -20, 100},
        {-20, -50, -2, -2, -2, -2, -50, -20},
        {10, -2, -1, -1, -1, -1, -2, 10},
        {5, -2, -1, -1, -1, -1, -2, 5},
        {5, -2, -1, -1, -1, -1, -2, 5},
        {10, -2, -1, -1, -1, -1, -2, 10},
        {-20, -50, -2, -2, -2, -2, -50, -20},
        {100, -20, 10, 5, 5, 10, -20, 100}
    };
    
    return value[i][j];
}
