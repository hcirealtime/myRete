#ifndef _HELP_H_
#define _HELP_H_
#include <string>
#include <vector>
#include <sstream>
#include "fact.h"

vector<string>& split(const string& str,char delim,vector<string>& result);
void printFact(Fact* f);
string deletesub(string &str, const string &sub, int n);
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}
#endif // _HELP_H_
