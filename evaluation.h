#ifndef _EVALUATION_H_
#define _EVALUATION_H_
#include <string>
#include <functional>

#include "fact.h"
using namespace std;

typedef function<bool (DATA_OBJECT*, DATA_OBJECT*) > FunctionCall;

void initFunctionTable();

bool GreaterThan(DATA_OBJECT* arg1,DATA_OBJECT* arg2);
bool SmallerThan(DATA_OBJECT* arg1,DATA_OBJECT* arg2);
bool Equal(DATA_OBJECT* arg1,DATA_OBJECT* arg2);

FunctionCall getFunctionAddress(string funcName);

#endif // _EVALUATION_H_
