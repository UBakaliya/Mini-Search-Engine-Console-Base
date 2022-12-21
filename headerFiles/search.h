/**
 *@file search.h
 *@author Uvaish Bakaliya (code implementer)
 *@details 
    Functions interactions entering the search term the program will return the
    matching term url.
    When searching a term using +, - or space --> ' '.
    For '+' will give the intersection of the two terms or multiple terms followed by '+'.
    Furthermore for '-' the program will return removed term or multiple terms.
    And for space "' '" the program will give matching terms or two terms or multiple terms.
*/

#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>

using namespace std;

const string RED_COLOR = "\x1b[31m";
const string GREEN_COLOR = "\x1b[32m";
const string BLUE_COLOR = "\x1b[34m";
const string CYAN_COLOR = "\x1b[36m";

// CleanToken will remove all the punctuations from the string
string cleanToken(string s)
{
    // Remove punctuation from the front of the string
    int i = 0;
    while (i < s.size() && !isalnum(s[i]))
        i++;
    s = s.substr(i);
    // Remove punctuation from the back of the string
    i = s.size() - 1;
    while (i >= 0 && !isalnum(s[i]))
        i--;
    s = s.substr(0, i + 1);
    transform(s.begin(), s.end(), s.begin(), ::tolower); // convert the string into lowercase
    return s;
}

// Gather tokens will remove the punctuations using the cleanToken()
// and add the words into set so that way we can avoid duplicates
set<string> gatherTokens(string text)
{
    stringstream ss(text);
    set<string> tokens; // will store tokens
    while (getline(ss, text, ' '))
    {
        text = cleanToken(text); // remove punctuations
        for (const auto &i : text)
        {
            if (i != ' ')
                tokens.insert(text);
        }
    }
    return tokens;
}

// Build index will map the value to its appropriate key
// in our case the key is the URL and value it the body text
// and will return the count of the mapped values on index
int buildIndex(string filename, map<string, set<string>> &index)
{
    ifstream fIn(filename);
    if (!fIn.fail())
    {
        string url, bodyText;
        set<string> urlSet,   // store the url into url sets so we can add it to our index map
            bodySet;          // store the values in body set so we can remove the duplicates from the body
                              // and assign those words as a keys in side of the map
        int pagesCounter = 0; // count the number of pages
        getline(fIn, url, '\n');
        urlSet.insert(url);
        getline(fIn, bodyText, '\n');
        bodySet = gatherTokens(bodyText);
        while (!fIn.eof())
        {
            // read the set of word and assign those words to a current url set
            for (const auto &key : bodySet)
            {
                for (const auto &value : urlSet)
                    index[key].insert(value);
            }
            urlSet.clear();
            getline(fIn, url, '\n');
            urlSet.insert(url);
            getline(fIn, bodyText, '\n');
            bodySet = gatherTokens(bodyText);
            pagesCounter++; // count the page
        }
        fIn.close();
        return pagesCounter; // return the counted pages
    }
    return 0;
}

// Find the key inside of the map the word it argument that is going to be search
// inside of th index map
void findKeyInMap(map<string, set<string>> index, string word, set<string> &result)
{
    result.clear();
    // find the value
    for (const auto &i : index)
    {
        for (const auto &j : i.second)
        {
            if (i.first == word)
                result.insert(j); // if found then insert it into result argument
        }
    }
}

// query matches will give the +, - or space terms
// results, the function will return set of founded terms (URLs) from index
set<string> queryMatches(
    map<string, set<string>> index, set<string> plus,
    set<string> minus, set<string> space, set<string> firstWordQueries)
{
    set<string> result; // store the tokens
    result.clear();
    // if there are any + then find those
    for (auto p : plus)
    {
        result.clear();
        // using the set_intersection() to find the matching terms across two sets
        set_intersection(firstWordQueries.begin(), firstWordQueries.end(),
                         plus.begin(), plus.end(), inserter(result, result.end()));
        // assign the back firstWordQueries so if there any an more + then it can find it
        firstWordQueries = result;
    }
    // same idea to spaces as well
    for (auto s : space)
    {
        findKeyInMap(index, s, firstWordQueries); // find query first since this is union
        set_union(firstWordQueries.begin(), firstWordQueries.end(),
                  result.begin(), result.end(), inserter(result, result.end()));
    }
    // same idea goes to for difference (-) as a intersection
    for (auto m : minus)
    {
        result.clear();
        set_difference(firstWordQueries.begin(), firstWordQueries.end(),
                       minus.begin(), minus.end(), inserter(result, result.end()));
        firstWordQueries = result;
    }
    return result; // return the queries
}

// find the matching query that is given by the user and return the set of those
// matched queries
set<string> findQueryMatches(map<string, set<string>> &index, string sentence)
{
    // store the result and if the string contains any +, - or space the
    // store those word as well
    set<string> result, plus, minus, space, firstWordQueries;
    string firstWord;
    result.clear();
    firstWordQueries.clear();
    int counter = 0;
    stringstream ss(sentence);

    getline(ss, firstWord, ' '); // get the first word
    firstWord = cleanToken(firstWord);

    // find the matching keys and values to a sentence first word
    findKeyInMap(index, firstWord, firstWordQueries);

    // if the there is only one word then return the matching query of that word
    if (firstWord.length() == sentence.length())
        result = firstWordQueries;

    // if not then
    else
    {
        // go to space first in the string
        while (getline(ss, sentence, ' '))
        {
            // if the sentence starting index if +,- or space then grab that word
            if (sentence[0] == '+')
            {
                sentence = cleanToken(sentence);     // remove +
                findKeyInMap(index, sentence, plus); // find the matching query terms to that word
            }
            else if (sentence[0] == '-')
            {
                sentence = cleanToken(sentence);      // remove -
                findKeyInMap(index, sentence, minus); // find the matching query terms to that word
            }
            // if it is space then
            else if (sentence[0] != '-' && sentence[0] != '+')
            {
                if (counter == 0)
                {
                    space.insert(firstWord);
                    counter++;
                }
                sentence = cleanToken(sentence); // remove ' '
                space.insert(sentence);          // find the matching query terms to that word
            }
        }
        // if there are any multiple terms then find those
        result = queryMatches(index, plus, minus, space, firstWordQueries);
    }
    return result;
}

// run the program by taking the file entry in the argument
void searchEngine(string filename)
{
    map<string, set<string>> index; // will hold the key and the value from the file(read the file into)
                                    // The key is URL and the Value the Body text

    set<string> foundQueries; // to store the founded matching queries
    string queryEntry;        // for query search entry from user
    index.clear();
    foundQueries.clear();
    queryEntry.clear();

    // count the pages from the buildIndex returned value that is the
    int pages = buildIndex(filename, index);
    cout << "Stand by while building index... \n";
    cout << "Indexed " << pages << " pages containing " << index.size() << " unique terms\n\n";
    // get the input of the search query and print the match queries
    while (true)
    {
        cout << GREEN_COLOR << "Enter query sentence (press enter to quit): ";
        cout << RED_COLOR;
        getline(cin, queryEntry);
        cout << GREEN_COLOR;
        // if entry is 'enter' then exit
        if (queryEntry == "")
        {
            cout << "Thank you for searching!" << endl;
            exit(0);
        }
        foundQueries = findQueryMatches(index, queryEntry); // find the query
        // display all the founded queries
        cout << CYAN_COLOR << "Found " << foundQueries.size() << " matching pages\n";
        for (const auto &url : foundQueries)
            cout << BLUE_COLOR << url << endl;
        cout << endl;
    }
}
