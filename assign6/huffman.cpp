#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "vector.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
using namespace std;

/**
 * Given a Queue<Bit> containing the compressed message bits and the encoding tree
 * used to encode those bits, decode the bits back to the original message text.
 *
 * You can assume that tree is a well-formed non-empty encoding tree and
 * messageBits queue contains a valid sequence of encoded bits.
 *
 * Your implementation may change the messageBits queue however you like. There
 * are no requirements about what it should look like after this function
 * returns. The encoding tree should be unchanged.
 */
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    string text;
    EncodingTreeNode* temp = tree;
    while (!messageBits.isEmpty()) {
        // left tree
        if( messageBits.dequeue() == 0){
            temp = temp->zero;
        // right tree
        }else{
            temp = temp->one;
        }
        // check whether current node is leaf node
        if(temp->isLeaf()){
            text += temp->getChar();
            temp = tree;
        }
    }

    return text;
}

/**
 * Reconstruct encoding tree from flattened form Queue<Bit> and Queue<char>.
 *
 * You can assume that the queues are well-formed and represent
 * a valid encoding tree.
 *
 * Your implementation may change the queue parameters however you like. There
 * are no requirements about what they should look like after this function
 * returns.
 */
EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    EncodingTreeNode* tree = nullptr;
    if(treeShape.dequeue() == 1){
        tree = new EncodingTreeNode(nullptr, nullptr);
        tree->zero = unflattenTree(treeShape, treeLeaves);
        tree->one = unflattenTree(treeShape, treeLeaves);
    }else{
        tree = new EncodingTreeNode(treeLeaves.dequeue());
    }
    return tree;
}

/**
 * Decompress the given EncodedData and return the original text.
 *
 * You can assume the input data is well-formed and was created by a correct
 * implementation of compress.
 *
 * Your implementation may change the data parameter however you like. There
 * are no requirements about what it should look like after this function
 * returns.
 */
string decompress(EncodedData& data) {
    auto tree = unflattenTree(data.treeShape, data.treeLeaves);
    auto text = decodeText(tree, data.messageBits);

    deallocateTree(tree);
    return text;
}

/**
 * Constructs an optimal Huffman coding tree for the given text, using
 * the algorithm described in lecture.
 *
 * Reports an error if the input text does not contain at least
 * two distinct characters.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 */
EncodingTreeNode* buildHuffmanTree(string text) {
    Map<char, int> freq;
    // get the frequence of each character
    for(const char & item : text){
        freq[item]++;
    }

    //make a priority queue based on the frequency
    PriorityQueue<EncodingTreeNode*> pq;
    for(const char & key : freq){
        EncodingTreeNode* node = new EncodingTreeNode(key);
        pq.enqueue(node, freq[key]);
    }

    //make tree
    while(!pq.isEmpty()){
        auto zeroP =  pq.peekPriority();
        auto zero = pq.dequeue();

        //check whether there is item left
        if(pq.isEmpty()){
            return zero;
        }
        auto oneP = pq.peekPriority();
        auto one = pq.dequeue();

        // combine two priority item into a new subtree
        auto subTree = new EncodingTreeNode(zero, one);

        //insert subtree into the priority queue
        pq.enqueue(subTree, zeroP + oneP);
    }

    return nullptr;
}


/**
 * @brief buildMap This function build a map based on encoding tree
 * @param tree
 * @param bits
 * @param codeTable
 */
// be caseful about &
void buildMap (EncodingTreeNode* tree, string bits, Map<char, string>& codeTable){
    //1.base case
    if(tree->isLeaf()){
        codeTable[tree->getChar()] = bits;
    }else{
        buildMap(tree->zero, bits + '0', codeTable);
        buildMap(tree->one, bits + '1', codeTable);
    }
}
/**
 * Given a string and an encoding tree, encode the text using the tree
 * and return a Queue<Bit> of the encoded bit sequence.
 *
 * You can assume tree is a valid non-empty encoding tree and contains an
 * encoding for every character in the text.
 */

Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    Map<char, string> codeTable;
    Queue<Bit> messageBits;
    buildMap(tree, "", codeTable);

    for(const char &elem : text){
        for(const char &bit : codeTable[elem]){
            if(bit == '0'){
                messageBits.enqueue(Bit(0));
            }else{
                messageBits.enqueue(Bit(1));
            }
        }
    }
    return messageBits;
}

/**
 * Flatten the given tree into a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment writeup.
 *
 * You can assume the input queues are empty on entry to this function.
 *
 * You can assume tree is a valid well-formed encoding tree.
 */
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    if(tree->isLeaf()){
        treeLeaves.enqueue(tree->getChar());
        treeShape.enqueue(Bit(0));
    }else{
        treeShape.enqueue(Bit(1));
        flattenTree(tree->zero, treeShape, treeLeaves);
        flattenTree(tree->one, treeShape, treeLeaves);
    }
}

/**
 * Compress the input text using Huffman coding, producing as output
 * an EncodedData containing the encoded message and flattened
 * encoding tree used.
 *
 * Reports an error if the message text does not contain at least
 * two distinct characters.
 */
EncodedData compress(string messageText) {
    Queue<Bit> treeShape;
    Queue<char> treeLeaves;

    auto tree = buildHuffmanTree(messageText);
    auto messageBits = encodeText(tree, messageText);
    flattenTree(tree, treeShape, treeLeaves);
    deallocateTree(tree);

    return {treeShape, treeLeaves, messageBits};
}

/* * * * * * Testing Helper Functions Below This Point * * * * * */

EncodingTreeNode* createExampleTree() {
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    /* TODO: Implement this utility function needed for testing. */
    EncodingTreeNode* r = new EncodingTreeNode('R');
    EncodingTreeNode* s = new EncodingTreeNode('S');
    EncodingTreeNode* e = new EncodingTreeNode('E');
    EncodingTreeNode* t = new EncodingTreeNode('T');
    EncodingTreeNode* r_s = new EncodingTreeNode(r,s);
    EncodingTreeNode* r_s_e = new EncodingTreeNode(r_s, e);
    EncodingTreeNode* t_r_s_e = new EncodingTreeNode(t, r_s_e);

    return t_r_s_e;
}

void deallocateTree(EncodingTreeNode* t) {
    //recrusion
    if(t != nullptr){
        deallocateTree(t->zero);
        deallocateTree(t->one);
        delete t;
    }
}

bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
    //1.Base case
    if (a != nullptr && b == nullptr){
        return false;
    }else if(a == nullptr && b != nullptr){
        return false;

    //2.1 node a ans b are not empty
    }else if(a != nullptr && b != nullptr){
        if(a->isLeaf() && b->isLeaf()){
            if(a->getChar() != b->getChar()){
                return false;
            }
            //2.2 node a and b are both empty
        }else{
            if(!areEqual(a->zero, b->zero) && !areEqual(a->one, b->one)){
                return false;
            }
        }
    }
    return true;
}

/* * * * * * Test Cases Below This Point * * * * * */


STUDENT_TEST("test helper function: createExampleTree"){
    EncodingTreeNode* a = createExampleTree();
    EncodingTreeNode* b = createExampleTree();
    EXPECT(areEqual(a,b));
    deallocateTree(a);
    deallocateTree(b);
}

STUDENT_TEST("empty tree"){
    EncodingTreeNode* a = nullptr;
    EncodingTreeNode* b = nullptr;
    EXPECT(areEqual(a,b));
}

STUDENT_TEST("single leaf tree vs empty tree"){
    EncodingTreeNode* a = new EncodingTreeNode('E');
    EncodingTreeNode* b = nullptr;
    EXPECT(!areEqual(a,b));
    deallocateTree(a);
}

STUDENT_TEST("single leaf tree vs single leaf tree"){
    EncodingTreeNode* a = new EncodingTreeNode('E');
    EncodingTreeNode* b = new EncodingTreeNode('E');
    EXPECT(areEqual(a,b));
    deallocateTree(a);
    deallocateTree(b);
}

STUDENT_TEST("createExampleTree vs single leaf tree"){
    EncodingTreeNode* a = new EncodingTreeNode('E');
    auto b = createExampleTree();
    EXPECT(!areEqual(a,b));

    deallocateTree(a);
    deallocateTree(b);
}

STUDENT_TEST("createExampleTree vs createExampleTree"){
    auto a = createExampleTree();
    auto b = createExampleTree();
    EXPECT(areEqual(a,b));

    deallocateTree(a);
    deallocateTree(b);
}

STUDENT_TEST("test decodeText"){
    auto tree = createExampleTree();
    EXPECT(tree != nullptr);
    Queue<Bit> messageBits = {1,0,0,1,1,1,0,1,1,1,0};
    EXPECT_EQUAL(decodeText(tree,messageBits), "RESET");

    messageBits = {0,1,1,1,0,1,0};
    EXPECT_EQUAL(decodeText(tree,messageBits), "TEST");
    deallocateTree(tree);
}

STUDENT_TEST("test unflattenTree:Q5"){
    Queue<Bit> treeshape = {1,1,0,1,0,0,1,0,0};
    Queue<char> treeLeaves = {'F', 'L', 'E', 'R', 'A'};
    EncodingTreeNode* tree = unflattenTree(treeshape, treeLeaves);
    deallocateTree(tree);

    treeshape = {1,0,1,0,0};
    treeLeaves = {'E', 'W', 'K'};
    tree = unflattenTree(treeshape, treeLeaves);
    deallocateTree(tree);

    treeshape = {1,0,1,1,0,0,0};
    treeLeaves = {'A', 'D', 'B', 'N'};
    tree = unflattenTree(treeshape, treeLeaves);

    deallocateTree(tree);
}

STUDENT_TEST("test encodeText"){
    EncodingTreeNode* tree = createExampleTree();

    Queue<Bit> messageBits = encodeText(tree, "TEST");
    EXPECT_EQUAL(decodeText(tree, messageBits), "TEST");

    messageBits = encodeText(tree, "SET");
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = encodeText(tree, "RESET");
    EXPECT_EQUAL(decodeText(tree, messageBits), "RESET");
    deallocateTree(tree);

}

STUDENT_TEST("test flattenTree"){
    EncodingTreeNode* tree = createExampleTree();
    Queue<Bit> treeShape;
    Queue<char> treeLeaves;


    flattenTree(tree,treeShape, treeLeaves);
    deallocateTree(tree);

    EXPECT(areEqual(unflattenTree(treeShape,treeLeaves), tree));

    deallocateTree(tree);
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("decompress, small example input") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeShape
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}

PROVIDED_TEST("buildHuffmanTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = buildHuffmanTree("STREETTEST");
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  expectedShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);

    EXPECT_EQUAL(treeShape,  expectedShape);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input") {
    EncodedData data = compress("STREETTEST");
    Queue<Bit>  treeShape   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'T', 'R', 'S', 'E' };
    Queue<Bit>  messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 };

    EXPECT_EQUAL(data.treeShape, treeShape);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress") {
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "Nana Nana Nana Nana Nana Nana Nana Nana Batman"
        "Research is formalized curiosity. It is poking and prying with a purpose. – Zora Neale Hurston",
    };

    for (string input: inputs) {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(input, output);
    }
}
