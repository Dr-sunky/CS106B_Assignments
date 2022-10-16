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


/* This function clean the punctuations at the start and end of string,
 * make sure this is at least one letter in string,
 * return lower case of the string
 */
string cleanToken(string s)
{
    //1. remove punctuation at the start and end of string

    // low efficient
//    while(true){
//        int i = 0;
//        if (ispunct(s[i])){
//            s.erase(0,1);
//        }
//        else
//            break;
//     }
//     while (true) {
//         if (ispunct(s[s.size()-1])){
//             s = s.substr(0,s.size() - 1);
//         }
//         else
//             break;
//     }

    int indexI = 0;
    int indexJ = 0;
    for (int i = 0; i < s.size(); i++) {
        if (ispunct(s[i])){
            indexI += 1 ;
        }
        if (ispunct(s[s.size()-1-i])){
            indexJ += 1;
        }
        else
            break;
    }
    s = s.substr(indexI,s.size() - indexJ - indexI);


    //2. make sure there is at leaset one letter in string
    for (int i = 0; i < s.size(); i++) {
        if (isalpha(s[i])){
            break;
        }
        else if (i == s.size() - 1){
            s.clear();
        }
    }
    //3. change letter from uppercase to lowercase
    return toLowerCase(s);
}

/*This function gets all strings which are different from each other from text.
 *
 */
Set<string> gatherTokens(string text)
{
    //1.split string into a series of sub strings based on delimiters
    Vector<string> subString = stringSplit(text, " ");

    //2.clean all sub strings
    Set<string> tokens;

    for (int i = 0; i < subString.size(); i++) {
        tokens.add(cleanToken(subString[i]));
    }

    //3.remove empty strings
    tokens.remove("");

    return tokens;
}

// This fucntion build index between text and url, and return the number of url.
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    /*1. The following code reads data from dbfile into a container
     *
     */
    ifstream infile;
    if (!openFile(infile, dbfile))
        error("Cannot open file named " + dbfile);

    Vector<string> lines;
    readEntireFile(infile, lines);

    /* 2. Now that the has already been put into a vector,
     * build a positive index first
     */
    Map<string, Set<string>> nIndex;
    int n = lines.size();
    for (int i = 0; i < n; i+=2){
        string url = lines[i];
        nIndex.put(url, gatherTokens(lines[i+1]));
    }
    /*3. converse index from positive to negative
     */

    for (const string &key : nIndex){

        //3.1 creat a set container to collect new key
        Set<string> webUrl;

        //3.2 get value under current key
        Set<string> copy =  nIndex[key];

        // 3.3 creat new negative index
        //3.3.1 search each element inside each key
        for (const string &elem : copy){
            // 3.3.2 element exist, add current key into related weburl
            if (index.containsKey(elem)){
                index[elem].add(key);
            }
            // 3.3.3 creat new key and value
            else{
                webUrl.add(key);
                index.put(elem,webUrl);
            }

        }
    }

    int nPages = n / 2;

    return nPages;
}


// This function reutrn result according to the input query,
// the query can be a single word, or more than two words connected by "+" or "-"
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query)
{
    Set<string> result;
    Set<string> subResult;
    //1.check the type of input query

    //2.split string into sigle
    Vector<string> subQuery = stringSplit(query, " ");

    //3. creat a set type keywords
    Set<string> keyWords;
    Set<string> cleanKeyWords;

    if (query.size() == 0){
        error("Please input at least one correct keyword");
    }
    else if (ispunct(subQuery[0].front())){
        error("The first letter of first keyword should be letter");
    }
    else if (subQuery.size() == 1){
        result.unionWith(index[cleanToken(subQuery[0])]);
    }
    else {

        // 4.check first letter of each subquery, "+" or "-" or " "

        for (int i = 0; i < subQuery.size(); i++) {
            //4.1 if ' ', or
            if (!ispunct(subQuery[i].front())){
                result.unionWith(index[cleanToken(subQuery[i])]);
            }
            //4.2 if '+', and
            else if (subQuery[i].front() == '+'){
                result.intersect(index[cleanToken(subQuery[i])]);
            }
            //4.3 if '-', without
            else if (subQuery[i].front() == '-'){
                result.difference(index[cleanToken(subQuery[i])]);
            }

        }
    }

    return result;
}

// This function builds a search engine by using the functions above
void searchEngine(string dbfile)
{
    //1. creat negative index
    cout << "Stand by while building index..." << endl;
    Map<string, Set<string>> index;
    int nPages = buildIndex(dbfile, index);
    cout << "Indexed " << nPages << " pages containing " << index.size() << "unique terms" << endl;

    while (true) {
        //2.enter keywords
        cout << "Enter query sentence (RETURN/ENTER to quit): " << endl;
        string query;
        Set<string> result;
        getline(cin, query);

        //3.keywords cannot be empty
        if (query.size() == 0){
            break;
        }

        result = findQueryMatches(index, query);

        cout << "Found " << result.size() << " matching pages" << endl;
        cout << result << "\r"<< endl;
    }

}

/* * * * * * Test Cases * * * * * */

//PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
//    EXPECT_EQUAL(cleanToken("hello"), "hello");
//    EXPECT_EQUAL(cleanToken("WORLD"), "world");
//    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
//}

//PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
//    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
//    EXPECT_EQUAL(cleanToken("<~woRLD!?"), "world");
//}

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
    //EXPECT_EQUAL(tokens.size(), 5);
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
STUDENT_TEST("cleanToken on empty strings"){
    //EXPECT_EQUAL(cleanToken("**"), "");
    EXPECT_EQUAL(cleanToken(" word"), " word");
    EXPECT_EQUAL(cleanToken("<<Hello!!"), "hello");
    EXPECT_EQUAL(cleanToken("<<!!Hello!!&!"), "hello");
}

STUDENT_TEST("cleanToken on one-letter strings"){
    EXPECT_EQUAL(cleanToken("C6"), "c6");
    EXPECT_EQUAL(cleanToken("3Q"), "3q");
}

STUDENT_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go / gophers"), expected);
}

STUDENT_TEST("gatherTokens from long strings, empty, punctuation, mixed case") {
    Set<string> expected = {"go", "one", "gophers"};
    EXPECT_EQUAL(gatherTokens("gO go go / oNe   " " && gophers"), expected);
}

STUDENT_TEST("Time operation on building negative index") {
    Map<string, Set<string>> index;
    buildIndex("res/website.txt", index);
    TIME_OPERATION(index.size(), buildIndex("res/website.txt", index));
}

STUDENT_TEST("findQueryMatches from tiny.txt, multipul words query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red -FISH");
    EXPECT_EQUAL(matchesRed.size(), 1);
    EXPECT(matchesRed.contains("www.rainbow.org"));
    Set<string> matchesHippo = findQueryMatches(index, "FISH -red");
    EXPECT_EQUAL(matchesHippo.size(), 2);
    EXPECT(matchesHippo.contains("www.bigbadwolf.com"));
    EXPECT(matchesHippo.contains("www.shoppinglist.com"));
    EXPECT_ERROR(findQueryMatches(index,"+red fish"));
    EXPECT_ERROR(findQueryMatches(index, ""));
}
