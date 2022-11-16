#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;
const int NONE = -1; // used as sentinel index

/**
 * @brief PQHeap::PQHeap
 * This constructor initialize all of the member variables needed for th PQHeap class.
 * The allocated capacity is initialized to a starting constant and a dynamic array of that
 * size is allocated. The number of filled slots is initially zero.
 */
PQHeap::PQHeap() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated](); // allocated zero'd memory
    _numFilled = 0;
}

/**
 * @brief PQHeap::~PQHeap
 * This deconstructor is responsible for cleaning up any resources used by this class
 */
PQHeap::~PQHeap() {
     delete[] _elements;
}

/**
 * @brief PQHeap::enqueue
 * @param elem
 *
 * adds a new element into the queue, expand the space if current space is not enough
 */
void PQHeap::enqueue(DataPoint elem) {
    if (this->size() == _numAllocated){
        expand();
    }

    _elements[this->size()] = elem;
    upAdjust(_elements);
    _numFilled ++;
}

/**
 * @brief PQHeap::expand
 * This function expand the space of dynamic array
 * dynamic array management
 */
void PQHeap::expand(){
    //backup
    DataPoint *_oldElems = _elements;

    //renew
    this->_numAllocated *= 2;
    _elements = new DataPoint[this->_numAllocated];

    //move data
    for(int i = 0; i < this->size(); i++){
        _elements[i] = _oldElems[i];
    }

    //delete
    delete[] _oldElems;
}

/**
 * @brief PQHeap::upAdjust Helper function
 * @param _elements
 *
 * This function move the element from the end of array to suitable place
 * min heap
 */
void PQHeap::upAdjust(DataPoint* _elements){
    child = this->size();
    parent = getParentIndex(child);
    DataPoint temp = _elements[child];

    while (child > 0 && temp.priority < _elements[parent].priority) {
        // parent node move down
        _elements[child] = _elements[parent];
        // child node move up
        child = parent;
        // get next parent node
        parent = getParentIndex(parent);
    }
    _elements[child] = temp;
}

/**
 * @brief PQHeap::peek
 * @return
 *
 * This function return the first member of the pqheap
 */
DataPoint PQHeap::peek() const {

    return _elements[0];
}

/**
 * @brief PQHeap::dequeue
 * @return
 *
 * This function return and remove the first member of the pqheap
 */
DataPoint PQHeap::dequeue() {
    DataPoint front = _elements[0];
    _elements[0] = _elements[_numFilled - 1];
    _numFilled --;
    downAdjust();
    return front;
}

/**
 * @brief PQHeap::downAdjust Helper function
 * @param _elements
 * @param parent
 * @param length
 *
 * This fucntion return move the first element to the suitable place
 */
void PQHeap::downAdjust(){
    parent = 0;
    DataPoint temp = _elements[parent];
    child = getLeftChildIndex(parent);

    //check whether child node exist
    if (child == NONE){

    }else{
        while (child < this->size()){
            // choose the smaller one
            // if right child exists and right value is lower than left value, then move to right child
            if(child + 1 < this->size()){
                if (_elements[child + 1].priority < _elements[child].priority){
                    child = getRightChildIndex(parent);
                }
            }

            // if parent node lower than the smallest child, jump out
            if(temp.priority <= _elements[child].priority){
                break;
            }
            _elements[parent] = _elements[child];
            parent = child;
            child = getLeftChildIndex(child);
            if (child == NONE){
                break;
            }
        }
    }
    _elements[parent] = temp;
};

/**
 * @brief PQHeap::isEmpty
 * @return
 *
 * This function check whether the array is empty
 */
bool PQHeap::isEmpty() const {

    return size() == 0;;
}

/**
 * @brief PQHeap::size
 * @return
 *
 * This function return the size of the array
 */
int PQHeap::size() const {

    return _numFilled;
}

/**
 * @brief PQHeap::clear
 *
 * This function clear current array
 */
void PQHeap::clear() {
    _numFilled = 0;
}

/**
 * @brief PQHeap::printDebugInfo
 * @param msg
 */
void PQHeap::printDebugInfo(string msg) const {
    cout << msg << endl;
   for(int i = 0; i < 0; i++){
       cout << "[" << i << "] = " << _elements[i] << endl;
   }
}

/*
 * We strongly recommend implementing this helper function, which
 * should traverse the heap array and ensure that the heap property
 * holds for all elements in the array. If elements are found that
 * violate the heap property, an error should be thrown.
 */
void PQHeap::validateInternalState() const {
    if (_numFilled > _numAllocated)
           error("Too many elements in not enough space!");
   validateHelper(0);
}

/**
 * @brief PQHeap::validateHelper
 * @param parent
 * @return
 */
void PQHeap::validateHelper(int parent) const {
    int left = getLeftChildIndex(parent);
    int right = getRightChildIndex(parent);
    // No child node
    if (left == NONE) {
        return;
    }
    // Only left child node
    if (right == NONE) {
        if (_elements[parent].priority > _elements[left].priority) {
            printDebugInfo("validateInternalState");
            error("PQHeap is not valid at index " + integerToString(parent));
        }
        return;
    } else {
        // Two child nodes
        if (_elements[parent].priority > _elements[left].priority ||
            _elements[parent].priority > _elements[right].priority) {
            printDebugInfo("validateInternalState");
            error("PQHeap is not valid at index " + integerToString(parent));
        }
        validateHelper(left);
        validateHelper(right);
    }
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the parent of the
 * specified child index. If this child has no parent, return
 * the sentinel value NONE.
 */
int PQHeap::getParentIndex(int child) const {
    if (child < 1){
        return NONE;
    }

    return (child - 1)/2;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the left child of the
 * specified parent index. If this parent has no left child, return
 * the sentinel value NONE.
 */
int PQHeap::getLeftChildIndex(int parent) const {
    if (2 * parent + 1 >= _numFilled ){
        return NONE;
    }

    return 2 * parent +1;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the right child of the
 * specified parent index. If this parent has no right child, return
 * the sentinel value NONE.
 */
int PQHeap::getRightChildIndex(int parent) const {
    if (2 * parent + 2 >= _numFilled ){
        return NONE;
    }

    return 2 * parent +2;
}

/* * * * * * Test Cases Below This Point * * * * * */
/* Helper function to fill vector with n random DataPoints. */
void fillVector(PQHeap& pq, int n) {
    pq.clear(); // start with empty heap
    for (int i = 0; i < n; i++) {
        DataPoint pt = {"", (double)randomInteger(0, 100)};
        pq.enqueue(pt);
    }
}

/* TODO: Add your own custom tests here! */

STUDENT_TEST("PQArray example from writeup") {
    PQHeap pq;

    pq.enqueue( { "Zoe", -3 } );
    pq.enqueue( { "Elmo", 10 } );
    pq.enqueue( { "Bert", 6 } );
    EXPECT_EQUAL(pq.size(), 3);
    pq.printDebugInfo("After enqueue 3 elements");

    pq.enqueue( { "Kermit", 5 } );
    EXPECT_EQUAL(pq.size(), 4);
    pq.printDebugInfo("After enqueue one more");

    DataPoint removed = pq.dequeue();
    DataPoint expected = { "Zoe", -3 };
    EXPECT_EQUAL(removed, expected);
    pq.printDebugInfo("After dequeue one");
}

STUDENT_TEST("PQHeap enqueue test"){
    PQHeap pq;
    pq.enqueue({"e", 2.718});
    pq.enqueue({"pi", 3.14});
    pq.enqueue({"phi", 1.618});

    for (int i = 0; i < 3; i++){
        pq.dequeue();
        //pq.validateInternalState();
    }
}

STUDENT_TEST("PQHeap validateInternalState"){
    for (int i= 0; i < 5; i++){
        PQHeap pq;
        fillVector(pq,10);
        pq.validateInternalState();
        EXPECT_NO_ERROR(pq.validateInternalState());
    }
}

STUDENT_TEST("PQArray: operations size/isEmpty/clear") {
    PQHeap pq;

    EXPECT(pq.isEmpty());
    pq.clear();
    EXPECT_EQUAL(pq.isEmpty(), pq.size() == 0);
    pq.enqueue({ "", 7 });
    EXPECT_EQUAL(pq.size(), 1);
    pq.enqueue({ "", 5 });
    EXPECT_EQUAL(pq.size(), 2);
    pq.enqueue({ "", 5 });
    EXPECT_EQUAL(pq.size(), 3);
    pq.clear();
    pq.validateInternalState();
    EXPECT(pq.isEmpty());
    EXPECT_EQUAL(pq.size(), 0);
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }
    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}
