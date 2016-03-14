Contributions:
solo

Improvements:
- Heuristic function
    I used different heuristic functions for different game stages, i.e., positional weights for early and middle stages, and added parity (difference of numbers of two players) for the end game stage. This made my AI capture favourable positions in the early and middle stages, and focus on the total number of places taken in the end stage.
    I also considered mobility, frontier disks, stability, etc., but decided not to include them in my heuristic function, because this the heuristic function was manually tuned instead of automatically tuned by machine learning, the more complex the heuristic function is, the more difficult it is to tune the function manually. So I kept the function simple and focused on the most important things.

- Minimax
    With minimax algorithm, the AI could look several steps forward and determine the best move at current search depth.

- Alpha beta pruning
    With alpha beta pruning, some subtrees with bad scores were cut off, which decreased the game states that minimax needed to search, and thus decreased the search time.

- Iterative deepening
    With iterative deepening, the AI was able to search as deep as there was time left.
