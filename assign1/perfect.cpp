/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "console.h"
#include <iostream>
#include "testing/SimpleTest.h"
using namespace std;

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */

long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* This function takes one long type argument "n" and calculates and sum of
 * all suitable divisors of "n"
 * a for loop iterats form 1 to sqrt(n), testing for zero remainder from division.
 * Conditional operator "? :" is employed at the end of function
 */
long smarterSum(long n) {
    /* TODO: Fill in this function. */
    long total = 1;
    for (long divisor = 2; divisor <= sqrt(n); divisor++) {
        if (n % divisor == 0) {
            if (divisor !=n/divisor )
            total += divisor + n/divisor;
            else
                total += divisor;
           }
    }
    return n == 1 ? 0 : total;
}

/* This function takes one long type argument "n" and return the boolean (true / false) value
 * to indicate whether "n" is perfect number or not.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfectSmarter(long n) {
    /* TODO: Fill in this function. */
    return (n != 0) && (n == smarterSum(n));
    //return false;
}

/* This function improves the original method to search perfect numbers.
 * Instead of searching from 1 to "N" to check each number,
 * we can actually start from 1, but stop at square root of N (sqrt(n)).
 */
void findPerfectsSmarter(long stop) {
     /* TODO: Fill in this function. */
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* This function takes one long type argument "n" and return the bollean value
 * to indicate whether "n" is primenumber or not .
 */

bool isPrimeNumber(long n) {
    for (long divisor = 2; divisor <= sqrt(n); divisor++) {
                if (n % divisor == 0)
                    return false;
            }
            return n ==1 ? 0 :true;
}

/*This function uses Mersenne number to search perfect number.
 * Mersenne number is "2^k - 1", if a Mersenne number is primenumber,
 * then "2^(k-1) * (2^k - 1)" is a perfect number, and if this perfect number is the n_th perfect number
 * then return the value.
*/
long findNthPerfectEuclid(long n) {
    //initialize counter
    long count = 0;

        for (long k = 1; k > 0; k++){
            // calculate m
            long t = pow(2,k)-1;

            //is primeNumber?
            if (isPrimeNumber(t)){
               // if so, found perfect number
               long m = pow(2,k-1) * t;
               count = count +1;
               if (count == n){
                   return m;
                   break;
               }

            }
    }
    return 0;
}

/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
    EXPECT_EQUAL(divisorSum(25), 6);
}

PROVIDED_TEST("Confirm 6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("Confirm 12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Test oddballs: 0 and 1 are not perfect") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
}

PROVIDED_TEST("Confirm 33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(10000, findPerfects(10000));
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
}

// TODO: add your student test cases here

STUDENT_TEST("Test negative findPerfect") {
    EXPECT(isPerfect(6));
}
/*
STUDENT_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(80000, findPerfects(80000));
    TIME_OPERATION(160000, findPerfects(160000));
    TIME_OPERATION(320000, findPerfects(320000));
    TIME_OPERATION(640000, findPerfects(640000));
}
*/

STUDENT_TEST("Test findPerfectsSmarter"){
    EXPECT_EQUAL(smarterSum(1), 0);
    EXPECT_EQUAL(smarterSum(6), 6);
    EXPECT_EQUAL(smarterSum(12), 16);
    EXPECT_EQUAL(smarterSum(25), 6);
    for (int i = 1; i < 100; i++ ) {
        EXPECT_EQUAL(smarterSum(i), divisorSum(i));
        EXPECT_EQUAL(smarterSum(i*i), divisorSum(i*i));
    }
}
/*
 *
STUDENT_TEST("Time trials of findPerfects on doubling input sizes") {
    TIME_OPERATION(80000, findPerfectsSmarter(80000));
    TIME_OPERATION(160000, findPerfectsSmarter(160000));
    TIME_OPERATION(320000, findPerfectsSmarter(320000));
    TIME_OPERATION(640000, findPerfectsSmarter(640000));
}

STUDENT_TEST("Time trials of findNthPerfectEuclid on doubling input sizes") {
    TIME_OPERATION(1, findNthPerfectEuclid(1));
    TIME_OPERATION(2, findNthPerfectEuclid(2));
    TIME_OPERATION(3, findNthPerfectEuclid(3));
    TIME_OPERATION(4, findNthPerfectEuclid(4));
}
STUDENT_TEST("Test the 5th findNthPerfectEuclid") {
    EXPECT(isPerfect(findNthPerfectEuclid(5)));
}

*/
STUDENT_TEST("Test whether the function can output the corrcet value ") {
    EXPECT_EQUAL(6, findNthPerfectEuclid(1));
    EXPECT_EQUAL(28, findNthPerfectEuclid(2));
    EXPECT_EQUAL(496, findNthPerfectEuclid(3));
    EXPECT_EQUAL(8128, findNthPerfectEuclid(4));
    EXPECT_EQUAL(33550336, findNthPerfectEuclid(5));
}

