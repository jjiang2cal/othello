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
    // this -> trans.clear();
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

    if (msLeft <= 1)
        return NULL;
    /*
    clock_t start = clock();
    double time_alloc;         // time allocated for search for this move
    */
    Move * move = NULL;
    int maxDepth;
    /*
    int diskCount = this -> board -> countBlack() + this -> board -> countWhite();
    // cerr << "diskCount = " << diskCount << endl;
    
    if (diskCount < 21)
    {
        time_alloc = (msLeft / 1000 - 860.) / 8.;    // 10s for beginning stage approx.
        maxDepth = 6;
    }
    else if (diskCount < 55)
    {
        time_alloc = (msLeft / 1000 - 350.) / 17.;    // 30s for middle stage approx.
        maxDepth = 8;
    }
    else
    {
        time_alloc = msLeft /1000 / 5.;    // 60s for endding stage approx.
        maxDepth = 8;
    }
    */
    
    /*
    cerr << "time_alloc = " << time_alloc << endl;
    cerr << "maxDepth = " << maxDepth << endl;
    */
    /*
    if (diskCount >= 56)
    {
        move = (testingMinimax) ? (this -> minimax(2)) : 
                (this -> minimax(64 - diskCount));
    }
    else if (diskCount > 20)
    {
        while ( (double)(clock() - start) / (CLOCKS_PER_SEC) * 2 < time_alloc)
        {
           
            cerr << "Passed: " << (double)(clock() - start) / (CLOCKS_PER_SEC) 
                << " seconds" << endl;
            cerr << "Now maxDepth = " << maxDepth << endl;
	    
            move = (testingMinimax) ? (this -> minimax(2)) : 
                (this -> minimax(maxDepth));
            maxDepth ++;
        }
    }
    else
    {
        move = (testingMinimax) ? (this -> minimax(2)) : 
            (this -> minimax(maxDepth));
    }
    */
    
    // fixed maxDepth
    maxDepth = 6;
    move = (testingMinimax) ? (this -> minimax(2)) : 
                (this -> minimax(maxDepth));
    
    if (move != NULL)
    {
        (this -> board) -> doMove(move, this -> side);
        return move;
    }
    return NULL;
}

/**
 * @brief  Minimax algorithm.
 * @param  maxDepth Depth to explore.
 * @return The best move calculated by minimax algorithmß.
 */
Move * Player::minimax(int maxDepth)
{
    /* minimax */
    // get available moves
    list<Move *> moves;
    this -> getMoves(this -> board, this -> side, &moves);
    if (moves.size() == 0)
        return NULL;

    // heuristic function score; start with a very negative number
    int score = -INFINITY;
    int alpha = -INFINITY;
    int beta = INFINITY;
    Move * move2 = NULL;
    list<Move *>::iterator it;
    // check each possible move
    for (it = moves.begin(); it != moves.end(); it++)
    {
        Move * move3 = *it;
        // simulate move on a copied board
        Board * copyBoard = (this -> board) -> copy();
        copyBoard -> doMove(move3, this -> side);
        // get heuristic function score; opponent wants to minimize our score
        int tempScore;
        tempScore = this -> min(copyBoard, maxDepth, alpha, beta);
            
        // maximize score on the minimal scores
        if (tempScore > score)
        {
            move2 = move3;
            score = tempScore;
        }
        
        alpha = (alpha > score) ? alpha : score;
        if (beta <= alpha)
        {
            continue;
        }
        
        delete copyBoard;
        
    }
    
    for (it = moves.begin(); it != moves.end(); it++)
    {
        Move * move3 = *it;
	if (move3 != move2)
	{
	    delete move3;
	}
    }

    return move2;
}

/**
 * @brief Min part of the minimax algorithm. Opponent's turn to play.
 *        Opponent wants to minimize our score.
 * @param board Board state before opponent chooses a move.
 *        depth Remaining depth to explore.
 * @return The minimal score of ours, because opponent will choose his best
 *         move to minimize our score.
 */
int Player::min(Board * board, int depth, int a, int b)
{
    int score;
    int alpha = a;
    int beta = b;
    Side side = this -> opponentSide;  // opponent's turn
    list<Move *> moves;
    this -> getMoves(board, side, &moves);
    if (depth == 0 || moves.size() == 0)
    {
        score = this -> evaluateBoard(board, this -> side);
        return score;
    }
    // Opponent passes. Now our turn. Maximize our score.
    if (moves.size() == 0)
    {
        return max(board, depth - 1, alpha, beta);
    }
    score = INFINITY;      // start with a very large number.
    list<Move *>::iterator it;
    // check each possible move
    for (it = moves.begin(); it != moves.end(); it++)
    {
        Move * move3 = *it;
        // simulate move on a copied board
        Board * copyBoard = board -> copy();
        copyBoard -> doMove(move3, side);
	/*
	// if the board with the depth has been calculated
	long hashValue = zobristHash(copyBoard, depth);
	if (this -> trans.find(hashValue) != this -> trans.end())
	{
	    return this -> trans[hashValue];
	}
	*/
        // Our turn. Maximize our score.
        int tempScore = max(copyBoard, depth - 1, alpha, beta);
	/*
	// if the transposition table exceeds size, erase the beginning one
	if (this -> trans.size() >= 100000)
	{
	    this -> trans.erase(trans.begin());
	}
	// store the score of copyBoard with the depth to the transposition table
	this -> trans[hashValue] = tempScore;
	*/
        // Opponent chooses the best step for him which minimizes our score.
        // store the min score
        if (tempScore < score)
        {
            score = tempScore;
        }
        beta = (beta < score) ? (beta) : score;
	delete copyBoard;
        if (beta <= alpha)
        {
            break;
        }
    }
    for (it = moves.begin(); it != moves.end(); it++)
    {
        Move * move3 = *it;
	delete move3;
    }
    return score;
}

/**
 * @brief Max part of the minimax algorithm. Our turn to play.
 *        We want to maximize our score.
 * @param board Board state before we choose a move.
 *        depth Remaining depth to explore.
 * @return The maximal score of ours, because we will choose our best
 *         move to maximize our score based on the minimal scores from
 *         opponent's previous move.
 */
int Player::max(Board * board, int depth, int a, int b)
{
    int score;
    int alpha = a;
    int beta = b;
    Side side = this -> side;   // our turn
    list<Move *> moves;
    this -> getMoves(board, side, &moves);
    if (depth == 0 || moves.size() == 0)
    {
        score = this -> evaluateBoard(board, this -> side);
        return score;
    }
    // No available move for us. We pass. Now opponent's turn.
    if (moves.size() == 0)
    {
        // opponent always wants to minimize our score;
        return min(board, depth - 1, alpha, beta);
    }
    score = -INFINITY;     // start with a very negative number
    list<Move *>::iterator it;
    // check each possible move
    for (it = moves.begin(); it != moves.end(); it++)
    {
        Move * move3 = *it;
        // simulate move on a copied board
        Board * copyBoard = board -> copy();
        copyBoard -> doMove(move3, side);
	/*
	// if the board with the depth has been calculated
	long hashValue = zobristHash(copyBoard, depth);
	if (this -> trans.find(hashValue) != this -> trans.end())
	{
	    return this -> trans[hashValue];
	}
	*/
        // get minimal score from opponent's move
        int tempScore = min(copyBoard, depth - 1, alpha, beta);
	/*
        // if the transposition table exceeds size, erase the beginning one
	if (this -> trans.size() >= 100000)
	{
	    this -> trans.erase(trans.begin());
	}
	// store the score of copyBoard with the depth to the transposition table
	this -> trans[hashValue] = tempScore;
	*/
        // Maximize our score based on the minimal scores
        if (tempScore > score)
        {
            score = tempScore;
        }
        alpha = (alpha > score) ? alpha : score;
	delete copyBoard;
        if (alpha >= beta)
        {
            break;
        }
    }
    for (it = moves.begin(); it != moves.end(); it++)
    {
        Move * move3 = *it;
	delete move3;
    }
    return score;
}

/**
 * @brief A private helper function which gets available moves.
 *
 * @param  board  this player's board
 *         side   this player's side
 */
void Player::getMoves(Board * board, Side side, list<Move *> * moves)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Move * move = new Move(i, j);

            // check whether move is legal
            // add the legal move to the list
            if (board -> checkMove(move, side))
            {
                moves -> push_back(move);
            }
            else
	    {
	        delete move;
	    }
        }
    }
    return;
}

/**
 * @brief A private helper function which evaluates the 
 *        heuristic function.
 *
 * @param  board  board to be evaluated
 *         side   side
 * @return A value of the heuristic function.
 */
int Player::evaluateBoard(Board * board, Side side)
{
    int score = 0;
    Side oppoSide = (side == BLACK) ? WHITE : BLACK;
    
    // get a score based on positional weights
    int position = this -> evaluatePosition(board, side);
    
    // get a score based on difference of numbers of disks
    int myDisks = board -> count(side);
    int oppoDisks = board -> count(oppoSide);
    int parity = myDisks - oppoDisks;

    // get a score based on difference of mobilities
    list <Move *> myMoves;
    this -> getMoves(board, side, &myMoves);
    // list <Move *> oppoMoves = this -> getMoves(board, oppoSide);
    // int mobility = 40 * (myMoves.size() - oppoMoves.size()) 
    //     / (myMoves.size() + oppoMoves.size());
    int mobility = 40 * myMoves.size();
    list<Move *>::iterator it;
    for (it = myMoves.begin(); it != myMoves.end(); it++)
    {
        delete *it;
    }
    /*
    for (it = oppoMoves.begin(); it != oppoMoves.end(); it++)
    {
        delete *it;
    }
    */
    // use positional weights for early and middle stages
    // add disk difference in late stage
    if (myDisks + oppoDisks < 54)
    {
        score = position + mobility;
    }
    else
    {
        score = position + parity * 100;
    }
    
    return score;

    // return parity;   // to test minimax, un-comment this line.
}

/**
 * @brief A private helper function which evaluates the 
 *        positional weights of the heuristic function.
 *
 * @param  board  this player's board
 *         side   this player's side
 * @return A value of the heuristic function.
 */
int Player::evaluatePosition(Board * board, Side side)
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
/**
 * @brief Zobrist Hashing for a given board state and depth. It takes the depth for consideration, 
 * so that if it is the same board but calculated at a different depth (i.e., a shallower depth)
 * the transposition table does not contain the score and the score at this depth needs to be
 * re-calculated in the minimax algorithm with alpha-beta pruning.
 * @param board Give board state.
 * @param depth The depth the node has explored.
 * @return The hash value.
 */
/*
long Player::zobristHash(Board * board, int depth)
{
    srand(1);
    long ** table = new long * [64];
    for (int i = 0; i < 64; i++)
    {
        table[i] = new long[2];
    }
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 2; j++)
	{
	    table[i][j] = rand();
	}
    }
    long hashValue = 0;
    for (int x = 0; x < 8; x++)
    {
        for (int y = 0; y < 8; y++)
	{
	    int i = 8 * x + y;
	    if (board -> occupied(x, y))
	    {
	        int j = (board -> get(WHITE, x, y)) ? 0 : 1;
		hashValue = (hashValue + depth) ^ (table[i][j] + depth);
	    }
	}
    }
    
    for (int i = 0; i < 64; i++)
        delete [] table[i];
    delete [] table;
    return hashValue;
}
*/
