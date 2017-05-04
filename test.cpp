#include <iostream>
#include <string>
#include <CString>
#include <fstream>
#include "test.h"
#include "help.h"
using namespace std;
#include "test.h"
using namespace std;

/**
build the beta network
这里只是一个规则:person类型和shop类型的age属性值相等
rule :
    IF
        person name ?name1 age ?age1
        shop name ?name2 age ?age2
        (test (= ?age1 ?age2))
    THEN
        do somethings

**/

AlphaNode* buildReteNetWork(ifstream& ifile)
{
	AlphaNode* root = new AlphaNode();
	BetaNode* topNode = new BetaNode();
	int acount = 0;
	int bcount = 0;
	AlphaNode* alpha[50];
	BetaNode* beta[50];

	//ifstream ifile(ifile);
	string rule_str = readRule(ifile);  //对每条规则，有一条从alpha,beta根节点导出的线

	vector<string> line;
	split(rule_str, '\n', line);   //   person name ?name1 age ?age1

	for (int i = 0; i < line.size(); i++)
	{
		vector<string> token;
		split(line[i], ' ', token);
		if (token[0] != "(test" && token[0] != "")  //建立alpha网络首层
		{
			for (int j = 1; j < token.size(); j += 2)
			{
				int k;
				for (k = 0; k < acount; k++)
				{
					if (alpha[k]->checkAttriName == token[j])
						break;
				}
				if (k == acount)  //如果当前alpha节点没有存在，则新建
				{
					alpha[acount] = new AlphaNode(token[j]);
					alpha[acount]->ruleAttriValue = token[j + 1];
					acount++;
				}

			}
		}
		else if (token[0] == "(test")  //处理test部分 (test (= ?age1 ?age2))
		{
			map<string, string> my_operator;
			my_operator.insert(make_pair("=", "Equal"));
			my_operator.insert(make_pair(">", "GreaterThan"));
			my_operator.insert(make_pair("<", "SmallerThan"));

			for (int i = 1; i < token.size(); i++)   //去掉（）
			{
				token[i] = deletesub(token[i], "(", 1);
				token[i] = deletesub(token[i], ")", 1);
			}


			BetaNode* beta1_1 = new BetaNode();
			beta1_1->firstJoin = true;
			BetaNode* beta2_1 = new BetaNode();
			//根据字符串"Equal"函数名在解析时,找到对应的函数入口地址指针类型的Equal
			FunctionCall funcAddress;
			funcAddress = getFunctionAddress(my_operator[token[1]]);
			beta2_1->evaluations.push_back(new EvaluationNode(funcAddress, 0, "age", "age"));  //?
			BetaNode* beta3_1 = new BetaNode(true);
			beta3_1->ruleToActive = "rule";
			beta[bcount++] = beta1_1;
			beta[bcount++] = beta2_1;
			beta[bcount++] = beta3_1;
		}
		root->child[token[0]] = alpha[i];
	}
	//将alpha首层节点的后继节点设置为true，结束
	for (int i = 0; i < acount; i++)
	{
		alpha[acount + i] = new AlphaNode("NULL", true);
		alpha[i]->child["ALL"] = alpha[acount + i];
		alpha[acount + i]->betaNodes.push_back(beta[i]);
	}
	for (int i = 0; i < bcount; i += 3)
	{
		topNode->childNode.push_back(beta[i]);
		beta[i]->childNode.push_back(beta[i + 1]);
		beta[i + 1]->childNode.push_back(beta[i + 2]);
	}
	return root;

}
AlphaNode* buildReteNetWork()
{
	
    AlphaNode* root = new AlphaNode();
    AlphaNode* alpha1_1 = new AlphaNode("name");
    AlphaNode* alpha1_2 = new AlphaNode("age");

    root->child["person"] = alpha1_1;   // ???? 为什么要这样给
    root->child["shop"] = alpha1_2;

    AlphaNode* alpha2_1 = new AlphaNode("NULL",true);
    AlphaNode* alpha2_2 = new AlphaNode("NULL",true);
    alpha1_1->child["ALL"] = alpha2_1;
    alpha1_2->child["ALL"] = alpha2_2;

    BetaNode* topNode = new BetaNode();
    
	BetaNode* beta1_1 = new BetaNode();
    beta1_1->firstJoin = true;
    BetaNode* beta2_1 = new BetaNode();
    //根据字符串"Equal"函数名在解析时,找到对应的函数入口地址指针类型的Equal
    FunctionCall funcAddress;
    funcAddress = getFunctionAddress("Equal");
    beta2_1->evaluations.push_back(new EvaluationNode(funcAddress,0,"age","age"));
    BetaNode* beta3_1 = new BetaNode(true);
    beta3_1->ruleToActive = "rule";

    alpha2_1->betaNodes.push_back(beta1_1);
    alpha2_2->betaNodes.push_back(beta2_1);
    topNode->childNode.push_back(beta1_1);
    beta1_1->childNode.push_back(beta2_1);
    beta2_1->childNode.push_back(beta3_1);

	return root;
}

string readRule(ifstream& ifile){
	string line;
	int line_count = 1;
	int flag = 0;
	string rule;
	while (getline(ifile, line)){
		//cout<<line<<endl;
		line = line.substr(line.find_first_not_of(" "), 
			               line.find_last_not_of(" ") - line.find_first_not_of(" ") + 1); //去掉字符串前后空格
		if (line == "IF")
			flag = 1;
		else if (line == "THEN")
			break;
		else if (flag == 1)//读取条件部分
		{
			rule += line;
			rule += "\n";
		}
	} // for
	//cout << rule << endl;
	return rule;
}
