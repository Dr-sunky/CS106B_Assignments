/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * Bug: This function doesn't consider whether the first character of string is a letter or not.
 * How to remove this bug: initialize "result", for loop start from "0" instead of "1".
 */
string removeNonLetters(string s) {
    string result ;//= charToString(s[0]);
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            result += s[i];
        }
    }
    return result;
}
/*This function convert letter to the number it repreents
 *
*/
//string encodeLetters (string s) {
//    string newResult = toUpperCase(s);
//    for (int i = 0; i < s.length(); i++){
//        if (newResult[i] == 'A' || newResult[i] == 'E' || newResult[i] == 'I' || newResult[i] == 'O' || newResult[i] == 'U' ||
//               newResult[i] == 'H' || newResult[i] == 'W' || newResult[i] == 'Y'){
//            newResult[i] = '0';
//        }
//        else if (newResult[i] == 'B' || newResult[i] == 'F' || newResult[i] == 'P' || newResult[i] == 'V') {
//            newResult[i] = '1';
//        }
//        else if (newResult[i] == 'C' || newResult[i] == 'G' || newResult[i] == 'J' || newResult[i] == 'K' || newResult[i] == 'Q' ||
//                 newResult[i] == 'S' || newResult[i] == 'X' || newResult[i] == 'Z') {
//            newResult[i] = '2';
//        }
//        else if (newResult[i] == 'D' || newResult[i] == 'T') {
//            newResult[i] = '3';
//        }
//        else if (newResult[i] == 'L') {
//            newResult[i] = '4';
//        }
//        else if (newResult[i] == 'M' || newResult[i] == 'N') {
//            newResult[i] = '5';
//        }
//        else if (newResult[i] == 'R')
//            newResult[i] = '6';
//    }
//    return newResult;
//}

/*This function use "find" to operate string.
 *
*/
string encodeLetters (string s) {
    s = toUpperCase(s);
    string newResult;
    string letters1 ("AEIOUHWY");
    string letters2 ("BFPV");
    string letters3 ("CGJKQSXZ");
    for (int i = 0; i < s.length(); i++){
        if (letters1.find (s[i]) != string::npos ){
            newResult += '0';
        }
        else if (letters2.find(s[i]) != string::npos) {
            newResult += '1';
        }
        else if (letters3.find(s[i]) != string::npos) {
            newResult += '2';
        }
        else if (s[i] == 'D' || s[i] == 'T') {
            newResult += '3';
        }
        else if (s[i] == 'L') {
            newResult += '4';
        }
        else if (s[i] == 'M' || s[i] == 'N') {
            newResult += '5';
        }
        else if (s[i] == 'R')
            newResult += '6';
    }
    return newResult;
}


/*This function merges repeated numbers
*/
//string mergeRepeatNumbers (string s) {
//    string newResult = s;
//    for (int i = 0; i < newResult.length(); i++) {
//        if ((int) newResult[i] == (int) newResult[i+1]) {
//            newResult.erase(i,1);
//        }
//    }
//    for (int i = 0; i < newResult.length(); i++) {
//        if ((int) newResult[i] == (int) newResult[i+1]) {
//            newResult.erase(i,1);
//        }
//    }
//    return newResult;
//}

/*This function coalesce string by using "back" function
 *
*/
string mergeRepeatNumbers (string s) {
    string newResult;
    for (int i = 0; i < s.length(); i++)
        if (newResult.back() == s[i])
            continue;
        else if (newResult.back() != s[i])
            newResult += s[i];

    return newResult;
}

/*This function remove 0 from string
 *
*/
string removeZero (string s) {
    string result;

    for (int i = 0; i < s.length(); i++){
        if (s[i] == '0')
            continue;
        else
            result +=s[i];
    }

	return result;
	}

/*This function judges whether the length of string is 4 or not
 * It returns the result if its length is 4
*/

//string lengthOfencodeIs4 (string s) {
//    string result = s;
//    for (int i = 0; i < 100; i++) {
//        if (result.length() < 4) {
//            result.append("0");
//        }
//        else if (result.length() > 4) {
//            result.erase(result.length() - 1);
//        }
//        else
//            break;
//    }

//        return result;
//}

string lengthOfencodeIs4 (string s) {
    if (s.length() > 4)
        return s.substr(0, 4);
    for (int i = 4 - s.length(); i > 0; i--) {
        s += "0";
    }

    return s;
}


/* This function encode the name to 4 digit code
 */
string soundex(string s) {
    string result;

    result = removeNonLetters(s);

    result = encodeLetters(result);

    result = mergeRepeatNumbers(result);

    result.replace(0, 1, toUpperCase(charToString(s[0])));

    result = removeZero (result);

    result = lengthOfencodeIs4(result);

    return result;
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;
    Vector<string> matchName;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames

    /* TODO: Fill in the remainder of this function. */
    cout << "Please Enter a surname: " << endl;
    string name;
    cin >> name;
    string encodeName = soundex(name);

    cout << "The soundex of this surname is: " << encodeName << endl;

    for (int i = 0; i < databaseNames.size(); i++){
        if (soundex(databaseNames[i]) == encodeName) {
            matchName.add(databaseNames[i]);
            matchName.sort();
        }
    }
    for (int i = 0; i < matchName.size(); i++ ) {
        cout << "Matches from database:" << matchName[i] << endl;
    }
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
    s = "7Xing";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Xing");
}

PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here

STUDENT_TEST("Is Angelou equal to A524") {
    string s = soundex("Angelou");
    EXPECT_EQUAL(s, "A524");
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

STUDENT_TEST("Test encodeletters function") {
    EXPECT_EQUAL(encodeLetters("AbCd"), "0123");
    EXPECT_EQUAL(encodeLetters(" "), "");
}

STUDENT_TEST("Test removeZero function") {
    EXPECT_EQUAL(removeZero("009344"), "9344");
    EXPECT_EQUAL(removeZero("Ar06007"), "Ar67");
}

STUDENT_TEST("Test mergeRepeatNumbers function") {
    EXPECT_EQUAL(mergeRepeatNumbers("aabbccdd"), "abcd");
    EXPECT_EQUAL(mergeRepeatNumbers("100667"), "1067");
    EXPECT_EQUAL(mergeRepeatNumbers("aallbb33"), "alb3");

}
