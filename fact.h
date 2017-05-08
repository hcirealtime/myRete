#ifndef _FACT_H_
#define _FACT_H_
#include <string>
#include <map>
#include <unordered_map>
#include <fstream>
using namespace std;

struct DATA_OBJECT{
    string value;
    string type;
    DATA_OBJECT(string v,string t = "INT"):value(v),type(t){}
};
struct Fact{
    string elementName;
    map<string,DATA_OBJECT*> attr_val; //key is attrianme val is val and its type

    Fact(){}
    Fact(string name):elementName(name){}
};

Fact* readFact(const string& fact_str);
Fact* readFactWithBracket(const string& fact_str);

#endif // _FACT_H_
