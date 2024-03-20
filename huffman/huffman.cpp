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
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    string decoded = "";
    EncodingTreeNode* currentNode = tree;

    // dequeing bits to traverse the tree

    while (!messageBits.isEmpty()) {
        auto bit = messageBits.dequeue();
        if (bit == 0 && currentNode->zero != nullptr) {
            currentNode = currentNode->zero;
        }
        else if (bit == 1 && currentNode->one != nullptr) {
            currentNode = currentNode->one;
        }

        // each found char add a string

        if (currentNode != nullptr && currentNode->isLeaf()) {
            char a = currentNode->getChar();
            decoded += a;
            currentNode = tree;
        }
    }

    return decoded;
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
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves) {

    if (treeShape.isEmpty() || treeLeaves.isEmpty()) {
        return nullptr;
    }

    // else {
    auto bit = treeShape.dequeue();
    EncodingTreeNode* treeD = new EncodingTreeNode(nullptr, nullptr);

    if (bit == 0) {
        char a = treeLeaves.dequeue();
        treeD->ch = a;
    }
    else if (bit == 1) {
        treeD->zero = unflattenTree(treeShape, treeLeaves);
        treeD->one = unflattenTree(treeShape, treeLeaves);
    }


    return treeD;

    deallocateTree(treeD);

    // }

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
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
string decompress(EncodedData& data) {
    /* TODO: Implement this function. */

    EncodingTreeNode* decoded = unflattenTree(data.treeShape, data.treeLeaves);
    string final = decodeText(decoded, data.messageBits);

    deallocateTree(decoded);

    return final;
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
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodingTreeNode* buildHuffmanTree(string text) {
    /* TODO: Implement this function. */
    EncodingTreeNode* addTogether;
    // ensure that the length is min 2
    if (text.length() < 2) {
        error("input text must contain at least two distinct characters");
    }
    // build a frequency map
    Map<char, double> frequencyTable;


    for (char c : text) {
        frequencyTable[c]++;
    }

    PriorityQueue<EncodingTreeNode*> pq;
    // build a priority queue of encoding trees
    for (char elem : frequencyTable) {
        EncodingTreeNode* toInsert = new EncodingTreeNode(nullptr, nullptr);
        toInsert->ch = elem;
        pq.enqueue(toInsert, frequencyTable[elem]);
    }


    while (!pq.isEmpty()) {

        double priority1 = pq.peekPriority();
        EncodingTreeNode* first = pq.dequeue();

        double priority2 = pq.peekPriority();
        EncodingTreeNode* second = pq.dequeue();

        addTogether = new EncodingTreeNode(first, second);

        if (pq.isEmpty()) {
            return addTogether;
        }
        pq.enqueue(addTogether, priority1+priority2);

    }
    deallocateTree(addTogether);
    return nullptr;
}


void findPaths(EncodingTreeNode* tree, string sequence, Map<char, string>& letters) {

    if (tree == nullptr) {
        return;
    }
    if (tree->isLeaf()) {
        char leaf = tree->getChar();
        letters[leaf] = sequence;
    }
    findPaths(tree->zero, sequence + "0", letters);
    findPaths(tree->one, sequence + "1", letters);

}

/**
 * Given a string and an encoding tree, encode the text using the tree
 * and return a Queue<Bit> of the encoded bit sequence.
 *
 * You can assume tree is a valid non-empty encoding tree and contains an
 * encoding for every character in the text.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {
    /* TODO: Implement this function. */
    string path = "";
    Map<char, string> toFind;

    Queue<Bit> toReturn;

    findPaths(tree, path, toFind);

    for (char c : text) {
        for (char a : toFind[c]) {
            if (a == '0') {
                toReturn.enqueue(0);
            }
            else {
                toReturn.enqueue(1);
            }
        }
    }

    return toReturn;
    deallocateTree(tree);


   // deallocateTree(tree);
}

/**
 * Flatten the given tree into a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment writeup.
 *
 * You can assume the input queues are empty on entry to this function.
 *
 * You can assume tree is a valid well-formed encoding tree.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    /* TODO: Implement this function. */

    // base case
    if (tree == nullptr) {
        return;
    }
    // second base case
    else if (tree != nullptr && !tree->isLeaf()) {
        treeShape.enqueue(1);
    }
    else if (tree->isLeaf()) {
        char a = tree->getChar();
        treeLeaves.enqueue(a);
        treeShape.enqueue(0);
    }

    // recursive case
    flattenTree(tree->zero, treeShape, treeLeaves);
    flattenTree(tree->one, treeShape, treeLeaves);

}

/**
 * Compress the input text using Huffman coding, producing as output
 * an EncodedData containing the encoded message and flattened
 * encoding tree used.
 *
 * Reports an error if the message text does not contain at least
 * two distinct characters.
 *
 * TODO: Add any additional information to this comment that is necessary to describe
 * your implementation.
 */
EncodedData compress(string messageText) {
    /* TODO: Implement this function. */
    EncodedData toReturn;

    if (messageText.length() < 2) {
        error("input text must contain at least two distinct characters");
    }

    EncodingTreeNode* newTree = buildHuffmanTree(messageText);

    Queue<Bit> stringOfBits = encodeText(newTree, messageText);

    Queue<Bit> shapeOfTree;
    Queue<char> leaves;

    flattenTree(newTree, shapeOfTree, leaves);

    toReturn.treeLeaves = leaves;
    toReturn.treeShape = shapeOfTree;
    toReturn.messageBits = stringOfBits;

    deallocateTree(newTree);

    return toReturn;

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
    EncodingTreeNode* newNode = new EncodingTreeNode(new EncodingTreeNode ('T'),
    new EncodingTreeNode(new EncodingTreeNode(new EncodingTreeNode('R'), new EncodingTreeNode('S')),
                                                                          new EncodingTreeNode('E')));

    return newNode;
}

void deallocateTree(EncodingTreeNode* t) {
    /* TODO: Implement this utility function needed for testing. */
    if (t == nullptr) {
        return;
    }
    else {
        deallocateTree(t->zero);
        deallocateTree(t->one);
        delete t;
    }
}

bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
    /* TODO: Implement this utility function needed for testing. */
    if ((a == nullptr && b != nullptr) || (a != nullptr && b == nullptr)) {
        return false;
    }
    else if (a != nullptr && b != nullptr && a->ch != b->ch) {
        return false;
    }
    else if (a != nullptr && b != nullptr) {
        return areEqual(a->zero, b->zero) && areEqual(a->one, b->one);
    }
    return true;
}

/* * * * * * Test Cases Below This Point * * * * * */

/* TODO: Write your own student tests. */


STUDENT_TEST("create and deallocate a tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    deallocateTree(tree);
}

STUDENT_TEST("empty and one leaf") {
    EncodingTreeNode* tree = new EncodingTreeNode(nullptr, nullptr);
    EncodingTreeNode* leaf = new EncodingTreeNode(new EncodingTreeNode ('T'), nullptr);

    EXPECT(!areEqual(tree, leaf));

    deallocateTree(tree);
    deallocateTree(leaf);
}


STUDENT_TEST("different trees") {
    EncodingTreeNode* tree = createExampleTree();
    EncodingTreeNode* leaf = new EncodingTreeNode(new EncodingTreeNode ('T'), nullptr);

    EXPECT(!areEqual(tree, leaf));

    deallocateTree(tree);
    deallocateTree(leaf);
}

STUDENT_TEST("same leafs") {
    EncodingTreeNode* leaf1 = new EncodingTreeNode(new EncodingTreeNode ('T'), nullptr);
    EncodingTreeNode* leaf = new EncodingTreeNode(new EncodingTreeNode ('T'), nullptr);

    EXPECT(areEqual(leaf1, leaf));

    deallocateTree(leaf1);
    deallocateTree(leaf);
}


STUDENT_TEST("same leafs") {
    EncodingTreeNode* leaf1 = new EncodingTreeNode(new EncodingTreeNode ('T'), new EncodingTreeNode ('S'));
    EncodingTreeNode* leaf = new EncodingTreeNode(new EncodingTreeNode ('T'), new EncodingTreeNode ('S'));

    EXPECT(areEqual(leaf1, leaf));

    deallocateTree(leaf1);
    deallocateTree(leaf);
}


STUDENT_TEST("same trees") {
    EncodingTreeNode* tree = createExampleTree();
    EncodingTreeNode* tree2 = createExampleTree();

    EXPECT(areEqual(tree, tree2));

    deallocateTree(tree);
    deallocateTree(tree2);
}


STUDENT_TEST("tree and subtree") {
    EncodingTreeNode* tree = createExampleTree();
    EncodingTreeNode* subtree = new EncodingTreeNode(new EncodingTreeNode(new EncodingTreeNode('R'), new EncodingTreeNode('S')),
                                                     new EncodingTreeNode('E'));

    EXPECT(!areEqual(tree, subtree));

    deallocateTree(tree);
    deallocateTree(subtree);
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
