/*
 * TODO: remove and replace this file header comment
 * You will edit and turn in this file.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
int points(string str) {
    /* TODO: Implement this function. */
    return str.length() - 3;
}


string make_key(int row, int col) {
    return to_string(row) + "_" + to_string(col);
}

void wordsFound(Grid<char>& board, int row, int col, string str, Lexicon &vocab, Set<string> &explored, Set<string> &wordsToFind ) {
    // cout << row << col << str << " " << pointsToAdd << endl;
    if (explored.contains(make_key(row, col)))
    {
        return;
    }

    explored.add(make_key(row, col));

    if (!board.inBounds(row,col) || !vocab.containsPrefix(str) || !isalpha(board[row][col])) {
        return;
    }

    else {
        str += board[row][col];
        // cout << "word found " << str << endl;
        if (str.length() >= 4 && vocab.contains(str) && !wordsToFind.contains(str)) {
            wordsToFind.add(str);
        }
        for (int i = -1; i < 2; i++)
            for(int j = -1; j < 2; j++) {
                if (i== 0 && j == 0) {
                    continue;
                }
                if (!explored.contains(make_key(row+i, col+j)))
                {
                    wordsFound(board, row+i, col+j, str, vocab, explored, wordsToFind);
                    explored.remove(make_key(row+i, col+j));
                }
            }

        return;
    }
    
}
/*
 * TODO: Replace this comment with a descriptive function
 * header comment.
 */
int scoreBoard(Grid<char>& board, Lexicon& lex) {
    /* TODO: Implement this function. */
    Set<string> wordsToAdd;
    string current = "";
    int sum = 0;

    int r = board.numRows();
    int c = board.numCols();
    
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            Set<string> explored;
            wordsFound(board, i, j, current, lex, explored, wordsToAdd);
        }
    }

    for (string word : wordsToAdd) {
        cout << "word: " << word << endl;
        sum += points(word);
    }

    return sum;
}

/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}




PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}


PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}


PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}


STUDENT_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'K','L','I','S'},
                        {'V','T','E','D'},
                        {'D','T','N','E'},
                        {'Z','E','A','A'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 197);
}


STUDENT_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'R','C','T','E'},
                        {'H','S','A','S'},
                        {'O','I','N','R'},
                        {'B','V','I','I'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 239);
}

STUDENT_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'C','U','N','L'},
                        {'L','A','S','N'},
                        {'M','D','R','O'},
                        {'F','S','W','L'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 165);
}



STUDENT_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'T','T','N','Y','A'},
                        {'U','I','O','A','L'},
                        {'S','O','F','A','N'},
                        {'T','O','G','L','F'},
                        {'I','X','E','S','A'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 175);
}
