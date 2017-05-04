#include <iostream>
#include "fact.h"
#include "help.h"

Fact* readFact(const string& fact_str)
{
    vector<string> token;
    split(fact_str,' ',token);   //sf1 = "person name tom age 10";

    string element_name = token[0];
    Fact* one_fact = new Fact(element_name);

    for(size_t i = 1; i < token.size();i+=2)
    {

        string attri_name = token[i];
        string attri_val  = token[i+1];
        one_fact->attr_val[attri_name] = new DATA_OBJECT(attri_val);
    }
    return one_fact;
}
