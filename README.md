# Game of Life

## C++ implementation of Conway's Game of Life. Also known simply as Life, is a cellular automaton devised by the British mathematician John Horton Conway in 1970.

### Description
The "game" is a zero-player game, meaning that its evolution is determined by its initial state, requiring no further input.\
One interacts with the Game of Life by creating an initial configuration and observing how it evolves.

### Game Rules
The universe of the Game of Life is an infinite two-dimensional orthogonal grid of square cells,\
each of which is in one of two possible states, alive or dead. Every cell interacts with its eight neighbours,\
which are the cells that are horizontally, vertically, or diagonally adjacent.\
At each step in time, the following transitions occur:

- Any live cell with fewer than two live neighbours dies, as if caused by under-population.

- Any live cell with two or three live neighbours lives on to the next generation.

- Any live cell with more than three live neighbours dies, as if by over-population.

- Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

The initial pattern constitutes the seed of the universe and is located in file `initial_data.txt`.\
First line in this file contains the horizontal and vertical dimensions of the grid,\
whereas all subsequent ones are the coordinates of living cells.
The first generation is created by applying the above rules simultaneously to every cell in the seed—births and deaths occur simultaneously.

### End of Game Criteria
- there are no “live” cells left on the field;
- at the next step none of the squares changes its state, i.e. a stable configuration is formed.

### License
Distributed under the GNU GPL v3.0 license.\
See LICENSE.txt for more information.