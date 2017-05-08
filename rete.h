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
struct AlphaNode;
struct AlphaMemory;
struct AlphaNode{
    string checkAttriName;
    vector<AlphaNode*> child;
    AlphaMemory* AM;
    FunctionCall functionName;
    DATA_OBJECT* cmpDATA;
    string cmpAttri;
    AlphaNode(FunctionCall fname = NULL,AlphaMemory* am = NULL):functionName(fname),AM(am){}
    AlphaNode(FunctionCall fname,string aName):functionName(fname),checkAttriName(aName){}
    AlphaNode(AlphaMemory* am):AM(am){}
};
struct ElementTypeNode{
    map<string,AlphaNode*> alphaNodes;
};
struct AlphaMemory{
    list<Fact*> alphaMemory;
    vector<BetaNode*> betaNodes;
    AlphaMemory(AlphaNode* fatherNode){fatherNode->AM = this;}
    AlphaMemory(){}
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
	void init(){
        InitializeCriticalSectionAndSpinCount(&nodeSection,1000);
	    numOfInstance = 0;
	    isRunning = false;
	}
	BetaNode(bool fj = false):firstJoin(fj)
	{
        init();
	}
	BetaNode(AlphaMemory* rightInput,BetaNode* leftInput,bool fj = false):firstJoin(fj){
	    init();
	    rightInput->betaNodes.push_back(this);
	    leftInput->childNode.push_back(this);
	}
};


void factToAM(Fact* fact,ElementTypeNode* root);
PartialMatch* mergePartialMatch(PartialMatch* lhsBinds,AlphaMatch* alphaMatch);
void networkLeft(PartialMatch* lhsBinds,BetaNode* curNode);
void networkRight(AlphaMatch* rhsBinds,BetaNode* curNode);
bool evaluateOnNode(PartialMatch* lhsBinds,AlphaMatch* alphaMatch,BetaNode* curNode);
#endif // _RETE_H_
