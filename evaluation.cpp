#include <map>
#include "evaluation.h"

map<string,FunctionCall> functionTable;

bool GreaterThan(DATA_OBJECT* arg1,DATA_OBJECT* arg2){
    if(arg1->type == arg2->type)
        return arg1->value > arg2->value;
    return false;
}
bool SmallerThan(DATA_OBJECT* arg1,DATA_OBJECT* arg2){
    if(arg1->type == arg2->type)
        return arg1->value < arg2->value;
    return false;
}
bool Equal(DATA_OBJECT* arg1,DATA_OBJECT* arg2){
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
    return functionTable[funcName];
}
