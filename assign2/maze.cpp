// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* Generate all of the valid neighbouring loactions base on current location,
 * and save these information into the a type container
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;

    // get current location
    int curRow = cur.row;
    int curCol = cur.col;

    // move one step to east
    // check whether the location is inside of the boundaries
    if (maze.inBounds(curRow, curCol + 1)){

        // check the next location is path: 1 or wall: 0
        if (maze[curRow][curCol + 1]){
            GridLocation nextEast (curRow, curCol + 1);
            neighbors.add(nextEast);
        }
    }

    // move one step to west
    // check whether the location is inside of the boundaries
    if (maze.inBounds(curRow, curCol - 1)){

        // check the next location is path: 1 or wall: 0
        if (maze[curRow][curCol - 1]){
            GridLocation nextWest (curRow, curCol - 1);
            neighbors.add(nextWest);
        }
    }

    // move one step to north
    // check whether the location is inside of the boundaries
    if (maze.inBounds(curRow + 1, curCol)){

        // check the next location is path: 1 or wall: 0
        if (maze[curRow + 1][curCol]){
            GridLocation nextNorth (curRow + 1, curCol);
            neighbors.add(nextNorth);
        }
    }

    // move one step to south
    // check whether the location is inside of the boundaries
    if (maze.inBounds(curRow - 1, curCol)){

        // check the next location is path: 1 or wall: 0
        if (maze[curRow - 1][curCol]){
            GridLocation nextSouth (curRow - 1, curCol);
            neighbors.add(nextSouth);
        }
    }

    return neighbors;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    GridLocation mazeEntrance = {0, 0};
    GridLocation cur;
    GridLocation next;
    Set<GridLocation> locations;

    // check maze exit is at right bottom
    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }
    int n = path.size();
    //check the each movement obey the rule
    while (!path.isEmpty()) {
        cur = path.pop();
        locations.add(cur);
        if (path.isEmpty()){
            break;
        }
        next = path.peek();
        if (generateValidMoves(maze,cur).contains(next)) {
            continue;
        }
        else {
            error("movement is illegal");
        }
    }

    // check maze entrance is at left top
    if ( cur != mazeEntrance){
        error("Path does not start at maze entrance");
    }

    //make sure each location will be employed only one time
    if (locations.size() !=n ){
        error("there is one location appear more than one time");
    }
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    Stack<GridLocation> path;
    Set<GridLocation> validMoves;
    Set<GridLocation> historyMoves;
    Stack<GridLocation> curPath;

    //1.creat an empty queue to add path
    Queue<Stack<GridLocation>> addPath;

    //2.creat a path including entrance only
    path.push({0, 0});
    addPath.enqueue(path);

    //3.check whether the queue is empty
    while (!addPath.isEmpty()) {
        path = addPath.dequeue();
        // 3.1 if current location is exit, then output
        if(path.peek() == mazeExit){
            break;
        }
        else{
            // 3.2 find next validMoves according to current location
            validMoves = generateValidMoves(maze, path.peek());
            historyMoves.add(path.peek());

            // 3.3 make sure these validMoves don't include all historyMoves
            for (const GridLocation &item : historyMoves){
                if (validMoves.contains(item))
                    validMoves.remove(item);
            }
        }

        // 3.4 include the current path into queue
        while (!validMoves.isEmpty()) {
            GridLocation copy = validMoves.first();
            validMoves.remove(copy);
            path.push(copy);
            addPath.enqueue(path);
            path.pop();
        }

        // 4. Animation
        curPath = addPath.peek();
        MazeGraphics::highlightPath(curPath, "blue", 20);
    }

    return path;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 25x33") {
    Grid<bool> maze;
    readMazeFile("res/25x33.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

 //TODO: add your test cases here
STUDENT_TEST("generateValidMoves on location on the side of 3x3 grid with no path") {
    Grid<bool> maze = {{false, false, false},
                       {false, false, false},
                       {false, false, false}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

STUDENT_TEST("generateValidMoves on location on the center of 3x3 grid with no path") {
    Grid<bool> maze = {{false, false, false},
                       {false, false, false},
                       {false, false, false}};
    GridLocation side = {1, 1};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

STUDENT_TEST("generateValidMoves on location on the corner of 3x3 grid with no path") {
    Grid<bool> maze = {{false, false, false},
                       {false, false, false},
                       {false, false, false}};
    GridLocation side = {2, 2};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

STUDENT_TEST("validatePath on wrong solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {0, 1}, {0, 0}, {0, 1}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("validatePath on wrong solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/19x35.soln", soln);

    EXPECT_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/19x35.maze", maze);
    readSolutionFile("res/19x35.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 17x37") {
    Grid<bool> maze;
    readMazeFile("res/17x37.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("solveMaze on file 33x41") {
    Grid<bool> maze;
    readMazeFile("res/33x41.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}
