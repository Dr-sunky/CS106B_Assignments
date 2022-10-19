/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "error.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function check whether the queue is in the order
 * from small to big
 */
bool isSequence (Queue<int> q){
    //1.check whether queue is empty, if so, return true
    while (!q.isEmpty()) {
        //2.pick and check whether there is only one element
        // in the queue, if so, return true
        int first = q.dequeue();
        if (q.isEmpty()){
            return true;
            break;
        }
        int second = q.peek();
        //3,compare these two elements
        if (first > second){
            return false;
            break;
        }
    }
    return true;
}

/*
 * This function combine two sequenced queues in a new queue in order
 */

Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    Queue<int> result;
    int aSmall = 0;
    int bSmall = 0;
    //1.check whether these two queues are in sequence
    if (!isSequence(a) || !isSequence(b)){
        error("Current queue is not in order from small to big");
    }
    // 1.1 check queue is not empty
    if (a.isEmpty()){
        return b;
    }else if (b.isEmpty()){
            return a;
        }

    // 2.1 pick the first element from queue a,
    while (!a.isEmpty() && !b.isEmpty()) {
         aSmall = a.peek();

        // 2.2 pick the first element from queue b
         bSmall = b.peek();

            // 2.3 compare these two elements
            if (aSmall <= bSmall){

                // 3.1 put the smaller element in queue
                result.enqueue(aSmall);
                // 3.1.1 remove this element
                aSmall = a.dequeue();
            }else{
                // 3.2 put the smaller element in queue
                result.enqueue(bSmall);
                // 3.2.1 remove this element
                bSmall = b.dequeue();
            }
    }
    // 4.merge the rest elements into the new queue
    while (!a.isEmpty()){
        result.enqueue(a.dequeue());
    }
    while (!b.isEmpty()){
        result.enqueue(b.dequeue());
    }
    return result;
}

/*
 * This function assumes correct functionality of the previously
 * defined binaryMerge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;
    //1.Base case
    if (all.size() == 1){
       return all[0];
    }

   //2.seperate vector into two parts
    Vector<Queue<int>> left = all.subList(0,all.size()/2);
    Vector<Queue<int>> right = all.subList(all.size()/2);

    Queue<int> leftMerge = recMultiMerge(left);
    Queue<int> rightMerge = recMultiMerge(right);

    result = binaryMerge(leftMerge, rightMerge);
    return result;
}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}

PROVIDED_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

PROVIDED_TEST("recMultiMerge, compare to naiveMultiMerge") {
    int n = 20;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);
    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
}

PROVIDED_TEST("Time binaryMerge operation") {
    int n = 1000000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
}

PROVIDED_TEST("Time naiveMultiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), naiveMultiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}

/* * * * * * My Test Cases * * * * * */
STUDENT_TEST("Test isSequence"){
    EXPECT(!isSequence({1,2,3,5,4}));
}

STUDENT_TEST("Test isSequence"){
    EXPECT(isSequence({3,4,4,5}));
}

STUDENT_TEST("binaryMerge, two short sequences") {
    Queue<int> a = {1,3,5,7,9,11,14};
    Queue<int> b = {1, 2, 3, 4, 15};
    Queue<int> expected = {1,1, 2, 3, 3, 4,5,7,9, 11, 14,15};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}
STUDENT_TEST("naiveMultiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402},
                             {},
                             {},
                             {},
                             {0,2,4,5},
                            };
    Queue<int> expected = {-5, -5, 0, 1, 2, 3, 4, 5, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}
