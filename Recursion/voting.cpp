// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "voting.h"
#include <deque.h>
#include "testing/SimpleTest.h"
using namespace std;

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior


int criticalVotes(Vector<int>& participants, int majorityVote, int current, int target, int sum) {

    // base case

    if (current == participants.size() || sum >= majorityVote) {
        return 0;
    }
    else if (current == target) {
        return criticalVotes(participants, majorityVote, current+1, target, sum);

    }

    else {

        int votesResults = 0;
        int sum_with_current = sum + participants[current];

        if (sum_with_current+participants[target] >= majorityVote && sum_with_current < majorityVote) {
            votesResults++;
        }

        // choose/explore without current
        votesResults += criticalVotes(participants, majorityVote, current+1, target, sum);

        // choose/explore with current
        votesResults += criticalVotes(participants, majorityVote, current+1, target, sum_with_current);

        return votesResults;
    }
}

Vector<int> computePowerIndexes(Vector<int>& blocks)
{
    Vector<int> result;
    Vector<int> chosen;
    int sum = 0;
    int finalSum = 0;

    //find majority constant
    for (int elem : blocks) {
        sum += elem;
    }
    int majority = sum/2 + 1;

    // loop through all elements
    for (int i = 0; i < blocks.size(); i++) {
        // int current = blocks[i];
        // int first = 0;
        int sumToProcess = 0;

        int votes = criticalVotes(blocks, majority, 0, i, 0);

        result.add(votes);
        finalSum +=votes;

    }

    for (int i = 0; i < result.size(); i++) {
        if (result[i] > 0) {
            cout << result[i] << " " << finalSum << endl;
            int provisional = floor(((result[i]*1.0/finalSum))*100);
            result[i] = provisional;
        }
    }

    return result;
}

/* * * * * * Test Cases * * * * * */

STUDENT_TEST("Test power index, blocks CA-TX-GA 55-38-16") {
    Vector<int> blocks = {5, 5, 5, 5};
    Vector<int> expected = {25, 25, 25, 25};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

STUDENT_TEST("Test power index, blocks copied") {
    Vector<int> blocks = {49, 48, 2, 1};
    Vector<int> expected = {41, 25, 25, 8};

    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}


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


PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 20; i++) {
        blocks.add(randomInteger(1, 30));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}


PROVIDED_TEST("Test power index, blocks EU post-Nice") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}
