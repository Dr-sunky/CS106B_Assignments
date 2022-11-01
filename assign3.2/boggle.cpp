/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"
using namespace std;

/**
 * @brief points:This function returns the point of each word
 * @param str
 * @return
 */
int points(string str) {
    return str.size() <= 3? 0:(str.size()-3);
}


/**
 * @brief generateValidMoves:This function finds and save all avaliable next movements around current location
 * right, west, top, bottom, top-left, top-right, bottom-left, bottom-right
 * @param board
 * @param cur
 * @return
 */
Set<GridLocation> generateValidMoves(const Grid<char>& board, GridLocation cur) {
    Set<GridLocation> neighbors;

    // get current location
    int curRow = cur.row;
    int curCol = cur.col;

    for ( int i = cur.row - 1; i<= curRow +1; i ++){
        for ( int j = cur.col - 1; j <= curCol+1; j ++){
            if (board.inBounds(i,j)){
                neighbors.add({i,j});
            }
        }
    }
    return neighbors;
}

/**
 * @brief scoreBoardRef: This function find and save all words which meet the requirements
 * @param board
 * @param soFar
 * @param cur
 * @param result
 * @param history
 * @param lex
 */
void scoreBoardRef(const Grid<char> &board, string &soFar, GridLocation cur, Set<string> &result, Set<GridLocation> &history, Lexicon &lex){
    //1.Base case
    if(!lex.containsPrefix(soFar)){
          return;
    } else if (soFar.size() >= 4 && lex.contains(soFar)){
        result.add(soFar);
    }

    auto neighbors =  generateValidMoves(board, cur).difference(history);

    if (neighbors.isEmpty()){
            return;

    } else {

        //2.Recursive
        for (const auto &loc : neighbors){
            //choose
            soFar += (board.get(loc));
            history.add(loc);
            //explore
            scoreBoardRef(board, soFar, loc, result, history, lex);
            //unchoose
            soFar.erase(soFar.size()-1);
            history.remove(loc);
        }
    }
}

/**
 * @brief scoreBoard: This function returns points in tatall
 * @param board
 * @param lex
 * @return
 */
int scoreBoard(Grid<char>& board, Lexicon& lex) {
    string soFar;
    int count = 0;
    Set<string> result;
    Set<GridLocation> history;
    for (int row = 0; row < board.numRows(); row++){
        for (int col = 0; col < board.numCols(); col ++){
            string soFar = charToString(board.get(row,col));
            Set<GridLocation> history = {{row,col}};
            scoreBoardRef(board, soFar, {row,col}, result, history,lex);
        }
    }

    for (const string &element : result){
        count += points(element);
    }
    return count;
}

/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}
