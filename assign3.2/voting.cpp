// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "map.h"
#include "voting.h"
#include "testing/SimpleTest.h"
using namespace std;

/**
 * @brief totalVotes:This function returns total votes of all blocks
 * @param blocks
 * @return
 */
int sumOfSet(const Vector<int> &blocks){
    int counts = 0;
    for (int i = 0; i < blocks.size(); i++){
        counts += blocks[i];
    }
    return counts;
}

/**
 * @brief listSubsets: This function returns all subsets of blocks, using backtracking method
 * @param blocks
 * @param curBlock
 * @param soFar
 * @param index
 * @param criticalPower
 */
void listSubsets(Vector<int> &blocks, int &curBlock, Vector<int> &soFar, int index, int &criticalPower){
    // jump current block
    if (index == curBlock){
        index ++;
    }
    //check current coalition win or not
    if (sumOfSet(soFar) > sumOfSet(blocks)/2){
        return;
    } else if (sumOfSet(soFar) + blocks[curBlock] > sumOfSet(blocks)/2  ){
            // count critical vote for current block
            criticalPower +=1;
    }
    for (int i = index; i < blocks.size(); i++){
        //1.include
        if(i == curBlock){
            continue;
        }
        soFar.add(blocks[i]);
        // Recursion
        listSubsets(blocks, curBlock, soFar, i+1, criticalPower);
        //2.exclude
        soFar.remove(soFar.size() - 1);

    }
}

/**
 * @brief computePowerIndexes: This function returns critical votes of each block
 * @param blocks
 * @return
 */
Vector<int> computePowerIndexes(Vector<int> &blocks)
{
    //Initialize
    Vector<int> result (blocks.size(),0);
    Vector<int> soFar;
    Map<int, int> mergeResult;
    //1.look for each block, find critical vote
    for (int i = 0; i < blocks.size(); i++){
        int criticalPower = 0;
        if (mergeResult.containsKey(blocks[i])){
            result[i] = mergeResult[blocks[i]];
        } else {
        //1.1 look for and check all possible results based on current block
        listSubsets(blocks, i, soFar, 0, criticalPower);
        mergeResult[blocks[i]] = criticalPower;
        result[i] = criticalPower;

        //1.2 empty soFar container
        soFar.clear();
        }
    }

    //2. transfer critical vote to percentage %
    double sum = sumOfSet(result);
    for(int i = 0; i < blocks.size(); i++){
        result[i] = 100 *result[i] / sum;
    }

    return result;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks Hempshead 1-1-3-7-9-9") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-NY 55-38-39") {
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 25; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}

STUDENT_TEST("Test power index, blocks 50-49-1") {
    Vector<int> blocks = {50, 49, 1};
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

STUDENT_TEST("Test power index, blocks 51-48-1") {
    Vector<int> blocks = {51, 48, 1};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

STUDENT_TEST("Test power index, blocks 49-49-2") {
    Vector<int> blocks = {49, 49, 2};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

