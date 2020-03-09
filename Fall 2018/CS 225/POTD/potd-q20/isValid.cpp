#include <string>
#include <stack>

using namespace std;

bool isValid(string input)
{
    stack<char> st;
    for (unsigned long i = 0; i < input.length(); i++)
    {
        switch (input[i])
        {
        case '{':
        case '[':
        case '(':
            st.push(input[i]);
            break;
        case '}':
            if (st.empty() || st.top() != '{')
            {
                return false;
            }
            st.pop();
            break;
        case ']':
            if (st.empty() || st.top() != '[')
            {
                return false;
            }
            st.pop();
            break;
        case ')':
            if (st.empty() || st.top() != '(')
            {
                return false;
            }
            st.pop();
            break;
        }
    }
    return st.empty();
}