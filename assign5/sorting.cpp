#include "listnode.h"
#include "vector.h"
#include "testing/SimpleTest.h"
#include "sorting.h"
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment about your implementation of the function.
 */
void quickSort(ListNode*& front) {
    ListNode* less = nullptr;
    ListNode* more = nullptr;
    ListNode* privot = nullptr;

    //1.Base case
    if (front == nullptr || front->next == nullptr){
        return;
    }

    //2.recrusion
    partition(front, less, privot, more);
    quickSort(less);
    quickSort(more);
    concatenate(front, less, privot, more);

}


/**
 * @brief partition;separate linkedlist into three subLinkedList, lower than, greater than, and equal to basement
 * @param front
 * @param list
 * @param less
 * @param privot
 * @param more
 */
void partition(ListNode*& list, ListNode*& less, ListNode*& privot, ListNode*& more){
    int head = list->data;
    while(list != nullptr){
        if(list->data > head){
            // remove and save current node;
            ListNode *temp = list;
            list = list->next;
            temp->next = more;
            more = temp;
        }else if(list->data < head){
            ListNode *temp = list;
            list = list->next;
            temp->next = less;
            less = temp;
        }else{
            ListNode *temp = list;
            list = list->next;
            temp->next = privot;
            privot = temp;
        }
    }
}

/**
 * @brief concatenate; combine subLinkedlist into one linkedlist
 * @param list
 * @param less
 * @param privot
 * @param more
 */
void concatenate(ListNode*& list, ListNode*& less, ListNode*& privot, ListNode*& more){
    ListNode* privotTail = getTail(privot);
    if(less != nullptr){
        list = less;
        //less = nullptr;
        ListNode* listTail = getTail(list);
        listTail->next = privot;
        //privot = nullptr;
        privotTail->next = more;
    }else{
        list = privot;
        //privot = nullptr;
        ListNode* listTail = getTail(list);
        listTail->next = more;
        //more = nullptr;
    }
}

/**
 * @brief getTail: get the tail of the linked list
 * @param list
 */
ListNode* getTail (ListNode* list){
    ListNode* listTail = list;
    while (listTail->next != nullptr) {
        listTail = listTail->next;
    }
    return listTail;
}

/* * * * * * Another method for quick sort * * * * * */
/**
 * @brief sort
 * @param front
 */
void sort(ListNode* front){
    ListNode* currentNode = front;
    //get the tail of the current list
    while (currentNode->next != nullptr) {
        currentNode = currentNode->next;
    }
    subSort(front,currentNode);
}

/**
 * @brief subSort
 * @param front
 * @param end
 */
void subSort(ListNode* front, ListNode* end){
    //1.Base case
    if(front == nullptr || front->next == nullptr || front == end){
        return;
    }

    int head = front->data;
    ListNode* left = front->next;
    ListNode* right = front->next;
    ListNode* left_pre = front;
    while (right != end) {
       //data lower  than basement
        if(right->data < head){
            swap(left,right);
            left_pre = left;
            left = left->next;
        }
       //data greater than basement
        right = right->next;
    }
    swap(front,left_pre);

    //2.recursion
    subSort(front, left_pre);
    subSort(left, end);
}

/**
 * @brief swap : exchange the data between two listnodes
 * @param i
 * @param j
 */
void swap(ListNode*i, ListNode*j){
    int temp = i->data;
    i->data = j->data;
    j->data = temp;
}

/* * * * * * Test Code Below This Point * * * * * */

/* TODO: Write your own student tests below to test your sorting helper
 * functions and overall sorting algorithm correctness. We have provided
 * the skeleton of some tests below to help you get started with this
 * process.
 */

/*
 * We have provided this utility function as a convenience. This
 * function traverses the linked list represented by the provided
 * front pointer and prints out the data stored in the list along
 * the way in a convenient format. You may find it to be helpful to
 * use this function as a debugging aid.
 */
void printList(ListNode* front) {
   cout << "{";
   for (ListNode *cur = front; cur != nullptr; cur = cur->next) {
       cout << cur->data;
       if (cur->next != nullptr){
           cout << ", ";
       }
   }
   cout << "}" << endl;
}

/*
 * This utility function deallocates the memory for all the nodes in a
 * given linked list. It can be used to recycle the memory allocated
 * during a test case and avoid memory leaks.
 */
void deallocateList(ListNode* front) {
    if(front != nullptr){
        while(front != nullptr){
            ListNode* temp = front->next;
            delete front;
            front = temp;
        }
    }
}

/*
 * This utility function is given a vector of values and constructs
 * an equivalent linked list containing the same values in the same order.
 * It can be used to construct a linked list to use as an input to
 * a function being tested.
 */
ListNode* createList(Vector<int> values){
    ListNode* head = nullptr;

    for(int i = values.size() - 1; i >= 0; i--){
        head = new ListNode (values[i], head);
    }
    return head;
}


/**
 * @brief LengthOfLinkedList: THis utility function teturn the length of the linkedlist
 * @param front
 * @return
 */
int LengthOfLinkedList(ListNode* front){
    ListNode* count = front;
    int n = 0;
    while (count !=nullptr) {
        n++;
        count = count->next;
    }
    return n;
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in the
 * same order. It can be used to confirm a linked list's contents match
 * the expected.
 */
bool areEquivalent(ListNode* front, Vector<int> v){

    //compare the size of both vector and linkedlist
    int length = LengthOfLinkedList(front);

    if( length != v.size()){
        return false;
    }else{
        ListNode* temp = front;
        for(int i = 0; i < v.size(); i++){
            if(temp->data != v[i]){
                return false;
            }
            temp = temp->next;
        }
    }
    return true;
}

/**Test**/

PROVIDED_TEST("This tests some of the testing helper functions you are recommended to implement."){
    /* Creates a small test list containing the values 1->2->3. */
    ListNode* testList = nullptr;
    testList = new ListNode(3, testList);
    testList = new ListNode(2, testList);
    testList = new ListNode(1, testList);

    /* Test equality comparison function. */
    EXPECT(areEquivalent(testList, {1, 2, 3}));
    EXPECT(!areEquivalent(testList, {1, 2}));
    EXPECT(!areEquivalent(testList, {1, 2, 3, 4}));
    EXPECT(!areEquivalent(testList, {1, 3, 2}));

    /* Test create list function. */
    ListNode* studentList = createList({1, 2, 3, 4});
    printList(studentList);
    ListNode* cur = studentList;
    for (int i = 1; i <= 4; i++){
        EXPECT(cur != nullptr);
        EXPECT_EQUAL(cur->data, i);
        cur = cur->next;
    }

    /* Test deallocate list functions. There should be no memory leaks from this test. */
    deallocateList(studentList);
    deallocateList(testList);
}

/*
 * The provided test case demonstrates a comparsion between the same
 * algorithm (quicksort) operating on a linked list versus a vector.
 */
PROVIDED_TEST("Time linked list quicksort vs vector quicksort") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        /* NOTE: This test does not check correctness of the linked list sort
         * function. It only times the two operations to compare relative speed. */
        TIME_OPERATION(n, quickSort(list));
        TIME_OPERATION(n, v.sort());        /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}


/**** My own test ****/
STUDENT_TEST("This is test for createList and deallocateList"){
    Vector<int> values  = {4,1,2,5,3};
    ListNode* testlist = createList(values);

    printList(testlist);
    deallocateList(testlist);
}

STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {1, 2, 3, 4};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test, empty."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test, single element."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {2};
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test: Reverse."){
    /* Initialize vector to sequence of values */
    Vector<int> values;
    for (int i = 200; i > 0; i--) {
        values.add(i);
    }
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test: Positive."){
    /* Initialize vector to sequence of values */
    Vector<int> values;
    for (int i = 0; i < 200; i++) {
        values.add(i);
    }
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test: repeat."){
    /* Initialize vector to sequence of values */
    Vector<int> values;
    for (int i = 0; i < 200; i++) {
        values.add(7);
    }
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test: random."){
    /* Initialize vector to sequence of values */
    Vector<int> values;
    for (int i = 0; i < 100; i++) {
        values.add(randomInteger(0,100));
    }
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test: positive and negative."){
    /* Initialize vector to sequence of values */
    Vector<int> values;
    for (int i = 0; i < 100; i++) {
        values.add(randomInteger(-100,100));
    }
    /* Create equivalent list to use as test input */
    ListNode* list = createList(values);

    /* Sort the list */
    quickSort(list);

    /* Sort the equivalent vector */
    values.sort();

    /* Confirm sorted list matches sorted vector */
    EXPECT(areEquivalent(list, values));

    /* Avoid memory leaks by deallocating list */
    deallocateList(list);
}

