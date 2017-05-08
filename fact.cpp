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

Fact* readFactWithBracket(const string& fact_str)
{
    string str,attri,val;
    int num = 0;
    Fact* one_fact = new Fact();

    for(size_t i = 0; i < fact_str.size();i++)
    {
        if(fact_str[i] == '(')
        {
            str = "";
        }
        else if(fact_str[i] == ' ')
        {
            if(num == 0)
            {
                one_fact->elementName = str;
            }
            else{
                attri = str;
            }
            num += 1;
            str = "";
        }
        else if(fact_str[i] == ')' && str.size() > 0)
        {
            one_fact->attr_val[attri] = new DATA_OBJECT(str);
            //cout<<"attri:"<<attri<<" val : "<<str<<endl;
            num += 1;
            str  = "";
        }
        else{
            str += fact_str[i];
        }
    } // for
    return one_fact;
}

