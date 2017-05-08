#include <cstdio>
#include <iostream>
#include "thread.h"
#include "rete.h"
using namespace std;

extern CRITICAL_SECTION g_print;
extern list<BetaNode*> activeBetaNodeList;
extern CRITICAL_SECTION g_cs;

void processFactAlphaMatchOnThread(Fact* fact,AlphaMemory* AMemory)
{
        AMemory->alphaMemory.push_back(fact);
        AlphaMatch* am = new AlphaMatch(fact);
        for(size_t i = 0; i < AMemory->betaNodes.size(); i++)
        {
            //cout<<"one fact"<<endl;
            //networkRight(am,root->betaNodes[i]);
            ActiveJoinNode* oneActiveNode = new ActiveJoinNode();
            oneActiveNode->AM = am;oneActiveNode->PM = NULL;
            oneActiveNode->curNode = (AMemory->betaNodes[i]);
            //oneActiveNode->realiseTime = am->realiseTime;
            AddActiveNode(oneActiveNode);
        }
}
void alphaNetworkMatchOnThread(Fact* fact,AlphaNode* r){

    //DATA_OBJECT* arg1 = fact->attr_val[r->checkAttriName];
    if(r->functionName == NULL || r->functionName(fact->attr_val[r->checkAttriName],r->cmpDATA))
    {
        if(r->AM != NULL)
        {
            processFactAlphaMatchOnThread(fact,r->AM);
        }
        for(size_t i = 0; i < r->child.size();i++)
        {
            alphaNetworkMatchOnThread(fact,r->child[i]);
        }
    }
    return ;
}
void factToAMOnThread(Fact* fact,ElementTypeNode* root){

    AlphaNode* r = root->alphaNodes[fact->elementName];
    alphaNetworkMatchOnThread(fact,r);
    //cout<<"factToAMOnThread"<<endl;

    return ;
}

void AddActiveNode(ActiveJoinNode* oneNode)
{
    EnterCriticalSection(&(oneNode->curNode->nodeSection));

    BetaNode* &curNode = oneNode->curNode;
    curNode->instance.push_back(oneNode);
    curNode->numOfInstance += 1;
    //cout<<"add one: "<< curNode<<endl;
    if(curNode->numOfInstance == 1)
    {
        EnterCriticalSection(&g_cs);
        activeBetaNodeList.push_back(curNode);
        //activeBetaNodeList.push_front(curNode);
        LeaveCriticalSection(&g_cs);
    }
    LeaveCriticalSection(&(oneNode->curNode->nodeSection));

    return;
}

ActiveJoinNode* GetOneBestActiveNode(){

    list<BetaNode*>::iterator it;
    BetaNode* curNode = NULL;
	ActiveJoinNode* rtnNode = NULL;
    EnterCriticalSection(&g_cs);

    for(it = activeBetaNodeList.begin();it != activeBetaNodeList.end();it++){
        if((*it)->isRunning == false){
            (*it)->isRunning = true;
            curNode = *it;
            break;
        }
    }

    if(curNode == NULL)
    {
        LeaveCriticalSection(&g_cs);
        return NULL;
    }

    LeaveCriticalSection(&g_cs);

    EnterCriticalSection(&(curNode->nodeSection));
    if(curNode->numOfInstance >0)
        rtnNode = curNode->instance.front();
	else{
        curNode->isRunning = false;
        //this is a bug ...
        cout<<"error: instance: "<<curNode->numOfInstance<<endl;
		LeaveCriticalSection(&(curNode->nodeSection));
		return NULL;
	}
    curNode->instance.pop_front();
    curNode->numOfInstance -= 1;

    if(curNode->numOfInstance <= 0){
        EnterCriticalSection(&g_cs);
        for(it = activeBetaNodeList.begin();it != activeBetaNodeList.end();it++){
            if((*it)->numOfInstance <= 0){
                activeBetaNodeList.erase(it++);
                break;
            }
        }
        LeaveCriticalSection(&g_cs);
    }
    LeaveCriticalSection(&(curNode->nodeSection));

    return rtnNode;
}

void networkRightOnThread(AlphaMatch* rhsBinds,BetaNode* curNode)
{
    curNode->rightMemory.push_back(rhsBinds);
	list<PartialMatch*> &leftMemory = curNode->leftMemory;

    //cout<<"networkRightOnThread"<<endl;
    if(curNode->firstJoin)
    {
        PartialMatch* links = mergePartialMatch(NULL,rhsBinds);
        for(size_t k = 0; k < curNode->childNode.size();k++)
		{
            //networkLeft(links,curNode->childNode[k]);
            //cout<<"right: "<<rhsBinds->fact->elementName<<endl;
            ActiveJoinNode* oneNode = new ActiveJoinNode();
            oneNode->AM = NULL;oneNode->PM = links;
            oneNode->curNode = curNode->childNode[k];
            oneNode->realiseTime = links->realiseTime;
            AddActiveNode(oneNode);
        }
        return ;
    }

	for (list<PartialMatch*>::iterator it = leftMemory.begin(); it != leftMemory.end();it++)
	{

        if(evaluateOnNode(*it,rhsBinds,curNode))
        {
            PartialMatch* links = mergePartialMatch((*it),rhsBinds);
            for(size_t k = 0; k < curNode->childNode.size();k++)
			{
                //networkLeft(links,curNode->childNode[k]);
                ActiveJoinNode* oneNode = new ActiveJoinNode();
                oneNode->AM = NULL;oneNode->PM = links;
                oneNode->curNode = curNode->childNode[k];
                AddActiveNode(oneNode);
            }
        }
    }
    return ;
}
void networkLeftOnThread(PartialMatch* lhsBinds,BetaNode* curNode)
{
    //cout<<"networkLeftOnThread"<<endl;
    curNode->leftMemory.push_back(lhsBinds);
    if(curNode->ruleToActive.size()){
        LARGE_INTEGER curTime;
        QueryPerformanceCounter(&curTime);
        EnterCriticalSection(&g_print);
        printf("%s : %lld\n",curNode->ruleToActive.c_str(),curTime.QuadPart);
        LeaveCriticalSection(&g_print);
        return ;
    }


	list<AlphaMatch*> &rightMemory = curNode->rightMemory;

	for (list<AlphaMatch*>::iterator it = rightMemory.begin(); it != rightMemory.end();it++)
    {
        if(evaluateOnNode(lhsBinds,*it,curNode))
        {

            PartialMatch* links = mergePartialMatch(lhsBinds,*it);
            for(size_t k = 0; k < curNode->childNode.size();k++)
			{
                ActiveJoinNode* oneNode = new ActiveJoinNode();
                oneNode->AM = NULL;oneNode->PM = links;
                oneNode->curNode = curNode->childNode[k];
                oneNode->realiseTime = links->realiseTime;

                AddActiveNode(oneNode);
            }
        }
    }
}
unsigned int __stdcall		  MoveOnJoinNetworkThread(void* pM)
{
    int threadID = ((struct ThreadNode*)pM)->threadID;

    while(true)
    {
        ActiveJoinNode* oneNode = GetOneBestActiveNode();
        if(oneNode == NULL) continue;

        AlphaMatch* AM = oneNode->AM;
        PartialMatch* PM = oneNode->PM;
        BetaNode* curNode = oneNode->curNode;

        //cout<<"get one active node"<<endl;
        if(AM != NULL){
            networkRightOnThread(AM,curNode);
        }
        else if(PM != NULL){
            networkLeftOnThread(PM,curNode);
        }
        else{
            cout<<"raise error in MoveOnJoinNetworkThread"<<endl;
        }
        curNode->isRunning = false;
        delete oneNode;
    }
    return 0;
}
