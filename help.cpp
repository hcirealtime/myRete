#include <iostream>
#include <sstream>

#include "help.h"

vector<string>& split(const string& str,char delim,vector<string>& result)
{
    string::size_type pos1 = 0,pos2 = str.find(delim);
    while(string::npos != pos2){
        result.push_back(str.substr(pos1,pos2-pos1));

        pos1 = pos2 + 1;
        pos2 = str.find(delim,pos1);

    }
    result.push_back(str.substr(pos1));
    return result;
}
void printFact(Fact* f){
    cout<<"fact:"<<endl;

    cout<<f->elementName<<endl;
    map<string,DATA_OBJECT*>::iterator it = f->attr_val.begin();
    for(;it != f->attr_val.end();it++){
        cout<<it->first<<":"<<it->second->value<<":"<<it->second->type<<endl;
    }

}
string deletesub(string &str, const string &sub, int n)
{
	int m, flag = 0, num = 0;
	while (flag == 0)
	{
		m = str.find(sub);
		if (m<0)
			flag = 1;
		else
		{
			str.erase(m, n);
			num++;
		}
	}
	return str;
}
