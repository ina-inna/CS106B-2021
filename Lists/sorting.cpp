#include "listnode.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;



void partitioner(ListNode*& initial, ListNode*& same, ListNode*& less, ListNode*& more, int pivotElement) {

    while (initial != nullptr) {
        ListNode* storage = initial;
        initial = initial->next;
        if (storage->data < pivotElement) {
            storage->next = less;
            less = storage;
        }
        else if (storage->data > pivotElement) {
            storage->next = more;
            more = storage;
        }
        else {
            storage->next = same;
            same = storage;
        }

    }
    return;

}


void concatenater(ListNode*& initial, ListNode*& less, ListNode*& same, ListNode*& more) {
    if (less != nullptr) {
        initial = less;
        ListNode* temp = initial;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = same;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = more;
        return;

    }
    else {
        initial = same;
        ListNode* temp = initial;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = more;
        return;
    }

}

void quickSort(ListNode*& front) {
    /* TODO: Implement this function. */
    if (front == nullptr || front->next == nullptr) return;

    else {
        int pivot = front->data;
        ListNode* equal = nullptr;

        ListNode* left = nullptr;
        ListNode* right = nullptr;

        partitioner(front, equal, left, right, pivot);

        quickSort(left);
        quickSort(right);

        concatenater(front, left, equal, right);
    }
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
    /* TODO: Implement this function. */
    while (front!= nullptr) {
        ListNode* temp = front->next;
        delete front;
        front = temp;
    }
}

/*
 * This utility function is given a vector of values and constructs
 * an equivalent linked list containing the same values in the same order.
 * It can be used to construct a linked list to use as an input to
 * a function being tested.
 */
ListNode* createList(Vector<int> values){
    /* TODO: Implement this function. */
    if (values.isEmpty()) return nullptr;

    ListNode* front = new ListNode(values[0], nullptr);

    ListNode* temp = front;

    for (int i = 1; i < values.size(); i++) {
        ListNode* newNode = new ListNode(values[i], nullptr);
        temp->next = newNode;
        temp = newNode;
    }

    return front;
}

/*
 * This utility function compares a linked list and a vector for
 * equivalence and returns true if both contain the same values in the
 * same order. It can be used to confirm a linked list's contents match
 * the expected.
 */
bool areEquivalent(ListNode* front, Vector<int> v){
    /* TODO: Implement this function. */
    ListNode* temp = front;

    for (int i = 0; i < v.size(); i++) {
        if (temp == nullptr || temp->data != v[i]) {
            return false;
        }
        else if (i == v.size()-1 && temp->next != nullptr) {
            return false;
        }
        temp = temp->next;
    }
    return true;
}

STUDENT_TEST("This is a skeleton to demonstrate an end-to-end student test."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {2, 4, 1, 3};
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




STUDENT_TEST("Positive and negative integers."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {10, 5, 2, 0, -100, 7, 8, 1, 8, 9, -150, 3, 13};
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


STUDENT_TEST("Empty list."){
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



STUDENT_TEST("Single digit."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {1};
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

STUDENT_TEST("Multiple duplicates."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {5, 5, 5, 5, 5, 5, 5, 5, 5};
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



STUDENT_TEST("Reverse order."){
    /* Initialize vector to sequence of values */
    Vector<int> values = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
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



STUDENT_TEST("Large sizes") {
    int startSize = 50000;

    for(int n = startSize; n < 10*startSize; n *= 2) {
        Vector<int> v(n);
        ListNode* list = nullptr;

        /* Create linked list and vector with the same random sequence. */
        for (int i = n-1; i >= 0; i--) {
            v[i] = randomInteger(-10000, 10000);
            list = new ListNode(v[i], list);
        }

        quickSort(list);

        /* Sort the equivalent vector */
        v.sort();

        /* Confirm sorted list matches sorted vector */
        EXPECT(areEquivalent(list, v));       /* Standard vector sort operation is backed
                                               with quicksort algorithm. */

        deallocateList(list);
    }
}




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

