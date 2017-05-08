#include <map>
#include <iostream>
#include <cstdlib>
#include "evaluation.h"
#include "help.h"

map<string,FunctionCall> functionTable;

bool GreaterThan(DATA_OBJECT* arg1,DATA_OBJECT* arg2){

    if(arg1->type == arg2->type){
        //cout<<(atoi(arg1->value.c_str()) > atoi(arg2->value.c_str()))<<endl;
        return atoi(arg1->value.c_str()) > atoi(arg2->value.c_str());
    }
    return false;
}
bool SmallerThan(DATA_OBJECT* arg1,DATA_OBJECT* arg2){
    if(arg1->type == arg2->type)
        return arg1->value < arg2->value;
    return false;
}
bool Equal(DATA_OBJECT* arg1,DATA_OBJECT* arg2){
    //cout<<"Equal"<<endl;
    if(arg1->type == arg2->type)
        return arg1->value == arg2->value;
    return false;
}
void initFunctionTable(){
    functionTable["Equal"] = Equal;
    functionTable["GreaterThan"] = GreaterThan;
    functionTable["SmallerThan"] = SmallerThan;
}

FunctionCall getFunctionAddress(string funcName)
{
    if(functionTable.find(funcName) == functionTable.end())
        cout<<"raise error in get FunctionAddress"<<endl;
    return functionTable[funcName];
}
