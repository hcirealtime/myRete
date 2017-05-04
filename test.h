#ifndef _TEST_H_
#define _TEST_H_
#include "rete.h"
#include "evaluation.h"

string readRule(ifstream& ifile);
AlphaNode* buildReteNetWork();
AlphaNode* buildReteNetWork(ifstream&);

AlphaNode* buildReteNetWork();

#endif // _TEST_H_
