#ifndef _TEST_H_
#define _TEST_H_
#include "rete.h"
#include "evaluation.h"

string readRule(ifstream& ifile);
ElementTypeNode* buildReteNetWork();
AlphaNode* buildReteNetWork(ifstream&);

void buildReteNetWork4Rules(ElementTypeNode* root,vector<string>&,int&);

#endif // _TEST_H_
