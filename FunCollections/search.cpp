// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
string cleanToken(string s)
{
    int j = 0;
    string new_string;
    for (int i = 0; i < s.size(); i++) {
        if (isalpha(s[i])) {
            new_string += tolower(s[i]);
            j++;
        }
        else {
           new_string += tolower(s[i]);
        }
    }

    while (ispunct(new_string[0])) {
        new_string = new_string.substr(1);
    }
    while (ispunct(new_string.back())) {
        new_string = new_string.substr(0, new_string.length() - 1);
    }

    if (j == 0) {
        return "";
    }
    return new_string;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    Vector<string> rawTokens = stringSplit(text, " ");
    for (string token : rawTokens) {
        string processed = cleanToken(token);
        if (processed != "") {
            tokens.add(processed);
        }
    }
    return tokens;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{

    /* The following code reads data from the file into a Vector
 * of strings representing the lines of the file.
 */
    ifstream in;

    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);

    Vector<string> lines;
    readEntireFile(in, lines);

    int nPages = 0;

    int numRows = lines.size();
    for (int r = 0; r < numRows; r+=2) {
        Set<string> tokens = gatherTokens(lines[r+1]);
        for (string word : tokens) {
            index[word].add(lines[r]);
        }
        nPages++;
    }

    return nPages;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    // TODO: your code here

    Vector<string> parseQuery = stringSplit(query, " ");

    if (size(parseQuery) == 1) {
        string clean = cleanToken(query);
        result = index.get(clean);
    }

    else {
        for (string elem : parseQuery) {
            if (startsWith(elem, "+")) {
                string clean = cleanToken(elem);
                Set<string> currentResult = index.get(clean);
                result.intersect(currentResult);
            }

            else if (startsWith(elem, "-")) {
                string clean = cleanToken(elem);
                Set<string> currentResult = index.get(clean);
                result.difference(currentResult);

            }

            else {
                string clean = cleanToken(elem);
                Set<string> currentResult = index.get(clean);
                result.unionWith(currentResult);

            }
        }
    }

    return result;
}

// TODO: Add a function header comment here to explain the
// behavior of the function and how you implemented this behavior
void searchEngine(string dbfile)
{
    // TODO: your code here
    Map <string, Set<string>> indexedPages;
    cout << "Stand by while building index..." << endl;
    int pages = buildIndex(dbfile, indexedPages);

    cout << "Indexed " << pages << " pages containing " << indexedPages.size() << " unique terms." << endl;

    string query;

    while (true) {
        cout << "Enter query sentence (RETURN to quit): ";
        getline(cin, query);

        // if (query == "\n" || query.empty()) {
        //     cout << "All done!";
        //     return;
        // }
        Set<string> result = findQueryMatches(indexedPages, query);
        cout << "Found " << result.size() << " matching pages \n" << result << endl;
    }

    return;
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}


// TODO: add your test cases here
STUDENT_TEST("cleanToken on double punctuation"){
    EXPECT_EQUAL(cleanToken("<<section>>"), "section");
}

STUDENT_TEST("cleanToken on punctuation"){
    EXPECT_EQUAL(cleanToken("**"), "");
}


STUDENT_TEST("cleanToken on excessive punctuation"){
    EXPECT_EQUAL(cleanToken("Hello!!!!!!!!!!"), "hello");
}


STUDENT_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish **");
    EXPECT_EQUAL(tokens.size(), 5);
}



PROVIDED_TEST("buildIndex from website.txt, 32 pages") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/website.txt", index);
    EXPECT_EQUAL(nPages, 32);
    EXPECT(index.containsKey("faithfully"));
}


STUDENT_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "milk i");
    EXPECT_EQUAL(matchesRedOrFish.size(), 2);
    Set<string> matchesRedAndFish = findQueryMatches(index, "milk +i");
    EXPECT_EQUAL(matchesRedAndFish.size(), 0);
}
