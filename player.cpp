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
    /* 
     * choose a move by heuristic function based on positional weights
     * and mobility
     */
    // get available moves
    list<Move *> moves
        = this -> getMoves(this -> board, this -> side);
    // get opponent's available moves
    list<Move *> oppoMoves
        = this -> getMoves(this -> board, this -> opponentSide);
    // heuristic function score; start with a very negative number
    int score = -1000000;
    // Ending state. Give it more weight.
    if (moves.size() == 0 && oppoMoves.size() == 0)
    {
        score = (this -> board -> count(this -> side) - 
            this -> board -> count(this -> opponentSide)) * 10000;
    }
    if (moves.size() != 0)
    {
        Move * move2;
        list<Move *>::iterator it;
        // check each possible move
        for (it = moves.begin(); it != moves.end(); it++)
        {
            Move * move3 = *it;
            // simulate move on a copied board
            Board * copyBoard = (this -> board) -> copy();
            copyBoard -> doMove(move3, this -> side);
            // get opponent's current possible moves
            oppoMoves = this -> getMoves(copyBoard, this -> opponentSide);
            // get a score based on mobility difference
            int mobility = 10 * (moves.size() - oppoMoves.size()) /
                (moves.size() + oppoMoves.size());
            // get a score based on positional weights
            int position = this -> evaluateBoard(copyBoard, this -> side);
            // get a score based on difference of numbers of disks
            int myDisks = copyBoard -> count(this -> side);
            int oppoDisks = copyBoard -> count(this -> opponentSide);
            int parity = (myDisks - oppoDisks) * 10;
            // temp score after move3
            int tempScore;
            if (myDisks + oppoDisks < 54)
            {
                tempScore = position + mobility;
            }
            else
            {
                tempScore = position + mobility + parity;
            }
            // store the higher score and corresponding move
            if (tempScore > score)
            {
                move2 = move3;
                score = tempScore;
            }
        }
        // do move which will lead to the highest score at this level
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
            // check whether move is legal
            if (!board -> checkMove(move, side))
                continue;
            // add the legal move to the list
            moves.push_back(move);
        }
    }
    return moves;
}

/**
 * @brief A private helper function which evaluates the 
 *        positional weights of the heuristic function.
 *
 * @param  board  this player's board
 *         side   this player's side
 * @return A value of the heuristic function.
 */
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

/**
 * @brief A private helper function which evaluates the positional weight.
 *
 * reference: http://play-othello.appspot.com/files/Othello.pdf
 *
 * @param  i      the row number of the board
 *         j      the colomn number of the board
 * @return A value of the positional weight.
 */
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
