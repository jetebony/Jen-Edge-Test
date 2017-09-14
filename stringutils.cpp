#include "stringutils.h"
#include <sstream>
#include <sstream>
#include <algorithm>

namespace integra_utils
{
    typedef vector<string>     TStringList;
    typedef queue<string>      TStringQueue;

    TStringList split(const string &_str, char _delim)
    {
        stringstream ss(_str);
        string       item;
        TStringList tokens;
        while (getline(ss, item, _delim))
            tokens.push_back(item);

        return tokens;
    }

    string trim(const string &_str)
    {
       auto wsfront = find_if_not(_str.begin(),  _str.end(), [] (int c) { return std::isspace(c); } );
       auto wsback  = find_if_not(_str.rbegin(), _str.rend(),[] (int c) { return std::isspace(c); } ).base();
       return (wsback <= wsfront ? string() : string(wsfront, wsback));
    }

    string escapeChars(char _ch, const string &_str)
    {
        string newStr;
        string::const_iterator it = _str.begin();
        while (it != _str.end())
        {
            if ((*it) == _ch)
                newStr += '\\';
            newStr += (*it);

            ++it;
        }
        return newStr;
    }
}
