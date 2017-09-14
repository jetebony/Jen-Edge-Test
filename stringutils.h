#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include "def.h"

#include <string>
#include <vector>
#include <map>
#include <queue>

using namespace std;
namespace integra_utils
{
    typedef vector<string>      TStringList;
    typedef map<string, string> TStringMap;
    typedef queue<string>       TStringQueue;

    LIBRARY_EXPORT TStringList  split(const           string &_str, char _delim);
    LIBRARY_EXPORT string        trim(const           string &_str);
    LIBRARY_EXPORT string escapeChars(char _ch, const string &_str);

}
#endif // STRINGUTILS_H

