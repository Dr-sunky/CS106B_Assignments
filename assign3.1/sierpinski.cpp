/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

/*
 * This function draws Sierpinski triangle
 */
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    GPoint midOne;
    GPoint midTwo;
    GPoint midThree;

    //1.Base Case, according to order
    if (order == 0){
        fillBlackTriangle(window, one, two, three);
        return 1;
    }

    //2.recursion case
    //2.1 pick up the middle point between one and two
    midOne.x = (one.x + two.x)/2;
    midOne.y = (one.y + two.y)/2;

    //2.2 pick up the middle point between two and three
    midTwo.x= (two.x + three.x)/2;
    midTwo.y = (two.y + three.y)/2;

    //2.3 pick up the middle point between one and three
    midThree.x = (one.x + three.x)/2;
    midThree.y= (one.y + three.y)/2;

//    fillBlackTriangle(window, one, midOne, midThree);
//    fillBlackTriangle(window, midOne, two, midTwo);
//    fillBlackTriangle(window, midThree, midTwo, three);

    // 2.4 decrease order
    int num1  = drawSierpinskiTriangle(window, one, midOne,midThree, order - 1);
    int num2  = drawSierpinskiTriangle(window, midOne, two, midTwo, order - 1);
    int num3  = drawSierpinskiTriangle(window, midThree, midTwo, three, order -1);

    return num1 + num2 + num3;
}


/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

