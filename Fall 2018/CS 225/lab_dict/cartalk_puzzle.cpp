/**
 * @file cartalk_puzzle.cpp
 * Holds the function which solves a CarTalk puzzler.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include <fstream>

#include "cartalk_puzzle.h"

using namespace std;

/**
 * Solves the CarTalk puzzler described here:
 * http://www.cartalk.com/content/wordplay-anyone.
 * @return A vector of (string, string, string) tuples
 * Returns an empty vector if no solutions are found.
 * @param d The PronounceDict to be used to solve the puzzle.
 * @param word_list_fname The filename of the word list to be used.
 */
vector<std::tuple<std::string, std::string, std::string>> cartalk_puzzle(PronounceDict d,
                                                                         const string &word_list_fname)
{
    vector<std::tuple<std::string, std::string, std::string>> ret;
    vector<std::string> vectorWords;
    ifstream words(word_list_fname);
    string word;
    if (words.is_open())
    {
        while (getline(words, word))
        {
            if (word.length() == 5)
            {
                vectorWords.push_back(word);
            }
        }
    }
    for (auto &Word : vectorWords)
    {
        std::string removeFirst = Word.substr(1);
        std::string removeSecond;
        removeSecond.append(Word.begin(), Word.begin() + 1);
        removeSecond.append(removeFirst.begin() + 1, removeFirst.end());
        if (d.homophones(Word, removeFirst) && d.homophones(Word, removeSecond) && d.homophones(removeFirst, removeSecond))
        {
            std::tuple<std::string, std::string, std::string> answer;
            std::get<0>(answer) = Word;
            std::get<1>(answer) = removeFirst;
            std::get<2>(answer) = removeSecond;
            ret.push_back(answer);
        }
    }
    return ret;
}
