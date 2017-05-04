#ifndef _RETE_H_
#define _RETE_H_
#include <windows.h>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <functional>
#include "fact.h"
#include "thread.h"

typedef function<bool (DATA_OBJECT*, DATA_OBJECT*) > FunctionCall;
struct BetaNode;
struct AlphaNode{
    string checkAttriName;
    map<string,AlphaNode*> child;
    bool isAM;
	string ruleAttriValue;
    list<Fact*> alphaMemory;
    vector<BetaNode*> betaNodes;
    AlphaNode(string name = "ELEMENT",bool last = false):checkAttriName(name),isAM(last){}
};
struct AlphaMatch{
    Fact* fact;
    AlphaMatch(Fact* f):fact(f){}
};
struct PartialMatch{
    vector<Fact*> facts;
    long long realiseTime;
};

struct EvaluationNode{
    //bool (*functionName)(DATA_OBJECT*,DATA_OBJECT*);
    FunctionCall functionName;
    int whichFact;
    string argAttriName1;
    string argAttriName2;
    EvaluationNode(FunctionCall name,int which,string name1,string name2):
        functionName(name),whichFact(which),argAttriName1(name1),argAttriName2(name2){}
};
struct BetaNode{
    string ruleToActive;
    bool isRunning;
    bool firstJoin;
    long long numOfInstance;
    CRITICAL_SECTION nodeSection;
    list<ActiveJoinNode*> instance;
	list<AlphaMatch*> rightMemory;
	list<PartialMatch*>leftMemory;
	vector<BetaNode*> childNode;
	int disToEnd;
	long long deadline;
	vector<EvaluationNode*> evaluations;
	BetaNode(bool fj = false):firstJoin(fj)
	{
	    InitializeCriticalSectionAndSpinCount(&nodeSection,1000);
	    numOfInstance = 0;
	    isRunning = false;
	}
};


void factToAM(Fact* fact,AlphaNode* root);
PartialMatch* mergePartialMatch(PartialMatch* lhsBinds,AlphaMatch* alphaMatch);
void networkLeft(PartialMatch* lhsBinds,BetaNode* curNode);
void networkRight(AlphaMatch* rhsBinds,BetaNode* curNode);
bool evaluateOnNode(PartialMatch* lhsBinds,AlphaMatch* alphaMatch,BetaNode* curNode);
#endif // _RETE_H_
