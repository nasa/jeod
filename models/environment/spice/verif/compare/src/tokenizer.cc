#include "../include/tokenizer.hh"

using namespace std;

vector<string> split(const string s, char c)
{
    const char * str = s.c_str();
    vector<string> result;

    do
    {
        const char *begin = str;

        while(*str != c && *str)
            str++;

        result.push_back(string(begin, str));
    } while (0 != *str++);

    return result;
}
