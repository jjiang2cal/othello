Contributions:
solo

Improvements:
- Heuristic function
    I used different heuristic functions for different game stages, i.e., positional weights and mobility for early and middle stages, and positional weights and parity (difference of numbers of two players) for the end game stage. This made my AI capture favourable positions in the early and middle stages, and focus on the total number of places taken in the end stage.
    I also considered frontier disks, stability, etc., but decided not to include them in my heuristic function, because this the heuristic function was manually tuned instead of automatically tuned by machine learning, the more complex the heuristic function is, the more difficult it is to tune the function manually. So I kept the function simple and focused on the most important things.

- Minimax
    With minimax algorithm, the AI could look several steps forward and determine the best move at current search depth.

- Alpha beta pruning
    With alpha beta pruning, some subtrees with bad scores were cut off, which decreased the game states that minimax needed to search, and thus decreased the search time.

Tried but abandoned:
- Iterative deepening
    With iterative deepening, the AI could search as deep as there was time left. But my code consumed too much memory.

- Transposition table
    A transposition table could save the calculated nodes for quick lookup. I tried to hash a board with certain search depth, but there might be some collisions and it did not work well.
