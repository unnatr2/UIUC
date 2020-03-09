// Your code here
#include "potd.h"
#include <vector>

using namespace std;

string getFortune(const string &s)
{
    vector<string> answers = {"As you wish!",
                              "Nec spe nec metu!",
                              "Do, or do not. There is no try.",
                              "This fortune intentionally left blank.",
                              "Amor Fati!"};
    int number = s.length() % answers.size();
    return answers[number];
}