/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

/*
 *This function pick all the parenthesis
 *from sting
 */
string operatorsFrom(string str) {
    string operators ("()[]{}");
    string result;
    //1.Base case
    //1.1 is empty?
    if (str.empty()){
        return "";
    }
    //1.2 is parenthesis or not
    if (operators.find(str[0]) != string::npos){
        result = str[0];
        str = str.substr(1);
    } else {
        str.erase(0,1);
    }
    //2.recursion case
    return result[0] + operatorsFrom(str);
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
bool operatorsAreMatched(string ops) {
    string operators ("()[]{}");
    //1.Base case
    if (ops.empty()){
        return true;
    }
    //1.1 pick on substring and check whether
    //is matched
    if (ops.find(operators.substr(0,2)) != string::npos){
        ops.erase(ops.find(operators.substr(0,2)),2);
    } else if (ops.find(operators.substr(2,2)) != string::npos){
        ops.erase(ops.find(operators.substr(2,2)),2);
    } else if (ops.find(operators.substr(4,2)) != string::npos){
         ops.erase(ops.find(operators.substr(4,2)),2);
    } else{
        return false;
    }

    //2.recursion case
    return operatorsAreMatched(ops);
}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}

/* * * * * * My Test Cases * * * * * */
STUDENT_TEST("operatorsFrom on multiple examples") {
    EXPECT_EQUAL(operatorsFrom("{{}}[lo]ve"), "{{}}[]");
    EXPECT_EQUAL(operatorsFrom("12(3)[ha]{0}"), "()[]{}");
    EXPECT_EQUAL(operatorsFrom("]]]see[[)"), "]]][[)");
}

STUDENT_TEST("operatorsAreMatched on multiple examples"){
    EXPECT(operatorsAreMatched("{[][]{}()}"));
    EXPECT(operatorsAreMatched("{[([])]}{}[]()"));
}
