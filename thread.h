#ifndef _THREAD_H_
#define _THREAD_H_
#include "fact.h"
using namespace std;
class BetaNode;
class AlphaMatch;
class PartialMatch;
class AlphaNode;
class ElementTypeNode;
struct ThreadNode
{
    int threadID;
};

struct ActiveJoinNode
{
    BetaNode* curNode;
    AlphaMatch* AM;
    PartialMatch* PM;
    long long realiseTime;
};
unsigned int __stdcall		  MoveOnJoinNetworkThread(void*);
void AddActiveNode(ActiveJoinNode*);
ActiveJoinNode* GetOneBestActiveNode();
void networkRightOnThread(AlphaMatch*,BetaNode*);
void networkLeftOnThread(PartialMatch*,BetaNode*);
void factToAMOnThread(Fact*,ElementTypeNode*);

#endif // _THREAD_H_
