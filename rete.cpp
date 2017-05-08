#include <iostream>
#include <cstdio>
#include "rete.h"

void processFactAlphaMatch(Fact* fact,AlphaMemory* AMemory)
{
        AMemory->alphaMemory.push_back(fact);
        AlphaMatch* am = new AlphaMatch(fact);
        for(size_t i = 0; i < AMemory->betaNodes.size(); i++)
        {
            //cout<<"one fact"<<endl;
            networkRight(am,AMemory->betaNodes[i]);
        }
        return ;
}
void alphaNetworkMatch(Fact* fact,AlphaNode* r){

    //DATA_OBJECT* arg1 = fact->attr_val[r->checkAttriName];
    if(r->functionName == NULL || r->functionName(fact->attr_val[r->checkAttriName],r->cmpDATA))
    {
        if(r->AM != NULL)
        {
            processFactAlphaMatch(fact,r->AM);
        }
        for(size_t i = 0; i < r->child.size();i++)
        {
            alphaNetworkMatch(fact,r->child[i]);
        }
    }

    return ;
}
void factToAM(Fact* fact,ElementTypeNode* root){

    AlphaNode* r = root->alphaNodes[fact->elementName];
    alphaNetworkMatch(fact,r);

    //cout<<root->alphaMemory.size()<<endl;
    return ;
}

PartialMatch* mergePartialMatch(PartialMatch* lhsBinds,AlphaMatch* alphaMatch)
{
     PartialMatch* links = new PartialMatch();
     //cout<<"******merge*********"<<endl;

     if(lhsBinds != NULL){
        for(size_t i = 0; i < lhsBinds->facts.size();i++){
            links->facts.push_back(lhsBinds->facts[i]);
        }
        //links->realiseTime = min(lhsBinds->realiseTime,alphaMatch->realiseTime);
    }
    if(alphaMatch != NULL){
        links->facts.push_back(alphaMatch->fact);

    }

     return links;
}
bool evaluateOnNode(PartialMatch* lhsBinds,AlphaMatch* alphaMatch,BetaNode* curNode)
{
    if(curNode->evaluations.size() == 0) return true;
    vector<EvaluationNode*> &funcVec = curNode->evaluations;
    //cout<<"evaluateOnNode"<<endl;
    for(size_t i = 0; i < funcVec.size();i++)
    {
        DATA_OBJECT* arg1 = lhsBinds->facts[funcVec[i]->whichFact]->attr_val[funcVec[i]->argAttriName1];
        DATA_OBJECT* arg2 = alphaMatch->fact->attr_val[funcVec[i]->argAttriName2];

        if(!funcVec[i]->functionName(arg1,arg2))return false;
    }
    return true;
}
void networkRight(AlphaMatch* rhsBinds,BetaNode* curNode)
{
    curNode->rightMemory.push_back(rhsBinds);
	list<PartialMatch*> &leftMemory = curNode->leftMemory;
    //cout<<"networkRight"<<endl;

    if(curNode->firstJoin){
        PartialMatch* links = mergePartialMatch(NULL,rhsBinds);
        for(size_t k = 0; k < curNode->childNode.size();k++)
		{
            networkLeft(links,curNode->childNode[k]);
        }
        return ;
    }

	for (list<PartialMatch*>::iterator it = leftMemory.begin(); it != leftMemory.end();it++)
	{
        if(evaluateOnNode(*it,rhsBinds,curNode))
        {
            PartialMatch* links = mergePartialMatch(*it,rhsBinds);
            for(size_t k = 0; k < curNode->childNode.size();k++)
			{
                networkLeft(links,curNode->childNode[k]);

            }
        }
    }
    return ;
}
void networkLeft(PartialMatch* lhsBinds,BetaNode* curNode)
{
    curNode->leftMemory.push_back(lhsBinds);
    //cout<<"networkLeft"<<endl;
    if(curNode->ruleToActive.size()){
        LARGE_INTEGER curTime;
        QueryPerformanceCounter(&curTime);
        //printf("%s %lld\n",curNode->ruleToActive,curTime.QuadPart);
        cout<<curNode->ruleToActive<<" "<<curTime.QuadPart<<endl;
        return ;
    }

	list<AlphaMatch*> &rightMemory = curNode->rightMemory;

	for (list<AlphaMatch*>::iterator it = rightMemory.begin(); it != rightMemory.end();it++)
    {

        if(evaluateOnNode(lhsBinds,*it,curNode))
		{
			PartialMatch* links = mergePartialMatch(lhsBinds, *it);
            for(size_t k = 0; k < curNode->childNode.size();k++)
			{
                networkLeft(links,curNode->childNode[k]);
            }
        }
    }
    return ;
}
