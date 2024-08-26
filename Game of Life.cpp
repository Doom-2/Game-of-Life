#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <windows.h>

using namespace std;

// Prints current state of the universe.
// Returns count of alive neighbors using pass by reference.
void printUniverse(char** arr, int rows, int cols, int gen_num, int& aliveNumOut) {
    int aliveNum = 0;
    cout << endl;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << arr[i][j] << " ";
            if (arr[i][j] == '*') aliveNum++;
        }
        cout << endl;
    }
    aliveNumOut = aliveNum;
    cout << "Generation: " << gen_num << ". " << "Alive cells: " << aliveNumOut << endl;
}

// Counts all cells adjacent to the current one, containing 'match_ch' character.
// Boundary conditions are taken into account.
int countAliveNeighbors(char** arr, int row, int col, int last_row, int last_col, const char match_ch) {
    int count = 0;
    // top left corner
    if (row == 0 && col == 0) {
        if (arr[row][col + 1] == match_ch) count++;
        if (arr[row + 1][col] == match_ch) count++;
        if (arr[row + 1][col + 1] == match_ch) count++;
    }
    // top row except for corners
    else if (row == 0 && col > 0 && col < last_col) {
        if (arr[row][col - 1] == match_ch) count++;
        if (arr[row][col + 1] == match_ch) count++;
        if (arr[row + 1][col - 1] == match_ch) count++;
        if (arr[row + 1][col] == match_ch) count++;
        if (arr[row + 1][col + 1] == match_ch) count++;
    }
    // top right corner
    else if (row == 0 && col == last_col) {
        if (arr[row][col - 1] == match_ch) count++;
        if (arr[row + 1][col - 1] == match_ch) count++;
        if (arr[row + 1][col] == match_ch) count++;
    }
    // left column except for corners
    else if (col == 0 && row > 0 && row < last_row) {
        if (arr[row - 1][col] == match_ch) count++;
        if (arr[row - 1][col + 1] == match_ch) count++;
        if (arr[row][col + 1] == match_ch) count++;
        if (arr[row + 1][col + 1] == match_ch) count++;
        if (arr[row + 1][col] == match_ch) count++;
    }
    // non-boundary conditions
    else if (row > 0 && col > 0 && row < last_row && col < last_col) {
        if (arr[row - 1][col - 1] == match_ch) count++;
        if (arr[row - 1][col] == match_ch) count++;
        if (arr[row - 1][col + 1] == match_ch) count++;
        if (arr[row][col - 1] == match_ch) count++;
        if (arr[row][col + 1] == match_ch) count++;
        if (arr[row + 1][col - 1] == match_ch) count++;
        if (arr[row + 1][col] == match_ch) count++;
        if (arr[row + 1][col + 1] == match_ch) count++;
    }
    // right column except for corners
    else if (col == last_col && row > 0 && row < last_row) {
        if (arr[row - 1][col - 1] == match_ch) count++;
        if (arr[row - 1][col] == match_ch) count++;
        if (arr[row][col - 1] == match_ch) count++;
        if (arr[row + 1][col - 1] == match_ch) count++;
        if (arr[row + 1][col] == match_ch) count++;
    }
    // bottom left corner
    else if (row == last_row && col == 0) {
        if (arr[row - 1][col] == match_ch) count++;
        if (arr[row - 1][col + 1] == match_ch) count++;
        if (arr[row][col + 1] == match_ch) count++;
    }
    // bottom row except for corners
    else if (row == last_row && col > 0 && col < last_col) {
        if (arr[row][col - 1] == match_ch) count++;
        if (arr[row][col + 1] == match_ch) count++;
        if (arr[row - 1][col - 1] == match_ch) count++;
        if (arr[row - 1][col] == match_ch) count++;
        if (arr[row - 1][col + 1] == match_ch) count++;
    }
    // bottom right corner
    else if (row == last_row && col == last_col) {
        if (arr[row - 1][col - 1] == match_ch) count++;
        if (arr[row - 1][col] == match_ch) count++;
        if (arr[row][col - 1] == match_ch) count++;
    }
    return count;
}

// Fills all cells of the temporary 2D-array according to the game rules.
void makeNewUniverseState(char** universe, char** universe_tmp, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            const char match = '*';
            int alive_neighbors_num = countAliveNeighbors(universe, i, j, rows - 1, cols - 1, match);
            switch (universe[i][j])
            {
            case '-':
                if (alive_neighbors_num == 3) universe_tmp[i][j] = '*';
                break;

            case '*':
                universe_tmp[i][j] = (alive_neighbors_num == 2 || alive_neighbors_num == 3) ? '*' : '-';
                break;
            default:
                break;
            }
        }
    }
}

// Copies all elements of the source 2D-array into the target one.
void copyUniverse(char** arr_src, char** arr_dst, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            arr_dst[i][j] = arr_src[i][j];
    }
}

// Compares original and new states of universe for equality.
// Returns true if all cells are equal, false otherwise.
bool checkUniverseStates(char** arr_1, char** arr_2, int rows, int cols) {
    bool is_equal = false;
    int count = 0;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            if (arr_1[i][j] == arr_2[i][j])
                count++;
    if (count == rows * cols) is_equal = true;
    return is_equal;
}


int main()
{
    ifstream file;
    string fname = "initial_data.txt", line;
    int rows, cols, alive_cell_x, alive_cell_y, gen = 1, alive_num = 0;
    bool gameOver = false, is_world_stagnated = false;

    file.open(fname);

    if (!file.is_open()) {
        cerr << "Error opening the file!" << endl;
        system("pause");
        return 1;
    }
    getline(file, line);

    istringstream(line) >> rows >> cols;

    char** universe = new char* [rows];
    char** universe_tmp = new char* [rows];

    for (int i = 0; i < rows; i++) {
        universe[i] = new char[cols];
        universe_tmp[i] = new char[cols];
        for (int j = 0; j < cols; j++) {
            universe[i][j] = '-';
            universe_tmp[i][j] = '-';
        }
    }

    while (getline(file, line)) {
        istringstream(line) >> alive_cell_x >> alive_cell_y;
        universe[alive_cell_x][alive_cell_y] = '*';
    }
    file.close();

    while (!gameOver) {
        printUniverse(universe, rows, cols, gen, alive_num);
        makeNewUniverseState(universe, universe_tmp, rows, cols);
        gen++;

        is_world_stagnated = checkUniverseStates(universe, universe_tmp, rows, cols);

        // if the game over conditions are not met,
        // copy new (temporary) state of the universe to the actual one for its subsequent printing
        if (alive_num != 0 && !is_world_stagnated) {
            copyUniverse(universe_tmp, universe, rows, cols);
            Sleep(1000);
            system("CLS"); //console cleanup
        }
        // if all cells are set to '-', exit the loop
        else if (alive_num == 0) {
            cout << "All cells are dead. Game over." << endl;
            gameOver = true;
        }
        // if all cells in both states of the universe are equal,
        // print the universe again to show that new state are the same to the previous one and exit the loop
        else {
            Sleep(1000);
            printUniverse(universe_tmp, rows, cols, gen, alive_num);
            cout << "The world has stagnated. Game over." << endl;
            gameOver = true;
        }
    }

    for (int i = 0; i < rows; i++) {
        delete[] universe[i];
        delete[] universe_tmp[i];
    }
    delete[] universe;
    delete[] universe_tmp;
    universe = universe_tmp = NULL;
    system("pause");
}
