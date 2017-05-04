#ifndef _HELP_H_
#define _HELP_H_
#include <string>
#include <vector>
#include "fact.h"
using namespace std;
vector<string>& split(const string& str,char delim,vector<string>& result);
void printFact(Fact* f);
string deletesub(string &str, const string &sub, int n);
#endif // _HELP_H_
