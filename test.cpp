#include <iostream>
#include <string>
#include <CString>
#include <fstream>
#include <cstdio>
#include <sstream>
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
    /**
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
    **/
}
ElementTypeNode* buildReteNetWork()
{
    /**/
    ElementTypeNode* root = new ElementTypeNode();

    AlphaNode* alpha_person = new AlphaNode();
    AlphaNode* alpha_shop = new AlphaNode();
    root->alphaNodes["person"] = alpha_person;
    root->alphaNodes["shop"] = alpha_shop;

    AlphaMemory* am_person = new AlphaMemory(alpha_person);
    AlphaMemory* am_shop = new AlphaMemory(alpha_shop);

    //beta network
    BetaNode* topNode = new BetaNode();

	BetaNode* beta1_1 = new BetaNode(true);
    am_person->betaNodes.push_back(beta1_1);
    topNode->childNode.push_back(beta1_1);

    BetaNode* beta2_1 = new BetaNode(am_shop,beta1_1);
    //根据字符串"Equal"函数名在解析时,找到对应的函数入口地址指针类型的Equal
    beta2_1->evaluations.push_back(new EvaluationNode(getFunctionAddress("Equal"),0,"age","age"));

    BetaNode* beta3_1 = new BetaNode();
    beta2_1->childNode.push_back(beta3_1);
    beta3_1->ruleToActive = "rule";

	return root;
	/**/
}

void buildReteNetWork4Rules(ElementTypeNode* root,vector<string>& element_names,int& ruleID)
{
    //buidl alpha network
    AlphaNode* alphaa = new AlphaNode();
    root->alphaNodes[element_names[0]] = alphaa;   // ???? 为什么要这样给
    AlphaNode* alphaa2 = new AlphaNode(getFunctionAddress("GreaterThan"),"timestamp");
    alphaa2->cmpDATA = new DATA_OBJECT("500");
    alphaa->child.push_back(alphaa2);
    AlphaMemory* alpha1_a = new AlphaMemory(alphaa2);
    AlphaMemory* alpha1_a_no = new AlphaMemory(alphaa);

    AlphaNode* alphab = new AlphaNode();
    root->alphaNodes[element_names[1]] = alphab;
    AlphaMemory* alpha1_b = new AlphaMemory(alphab);

    AlphaNode* alphac = new AlphaNode();
    root->alphaNodes[element_names[2]] = alphac;
    AlphaMemory* alpha1_c = new AlphaMemory(alphac);

    AlphaNode* alphad = new AlphaNode();
    root->alphaNodes[element_names[3]] = alphad;
    AlphaMemory* alpha1_d = new AlphaMemory(alphad);

    AlphaNode* alphae = new AlphaNode();
    root->alphaNodes[element_names[4]] = alphae;
    AlphaMemory* alpha1_e = new AlphaMemory(alphae);

    AlphaNode* alphaf = new AlphaNode();
    root->alphaNodes[element_names[5]] = alphaf;
    AlphaMemory* alpha1_f = new AlphaMemory(alphaf);

    AlphaNode* alphag = new AlphaNode();
    root->alphaNodes[element_names[6]] = alphag;
    AlphaMemory* alpha1_g = new AlphaMemory(alphag);

    AlphaNode* alphah = new AlphaNode();
    root->alphaNodes[element_names[7]] = alphah;
    AlphaMemory* alpha1_h = new AlphaMemory(alphah);

    AlphaNode* alphai = new AlphaNode();
    root->alphaNodes[element_names[8]] = alphai;

    AlphaNode* alphai2 = new AlphaNode(getFunctionAddress("GreaterThan"),"timestamp");
    alphai2->cmpDATA = new DATA_OBJECT("500");
    alphai->child.push_back(alphai2);
    AlphaMemory* alpha1_i = new AlphaMemory(alphai2);

    AlphaNode* alphaj = new AlphaNode();
    root->alphaNodes[element_names[9]] = alphaj;
    AlphaMemory* alpha1_j = new AlphaMemory(alphaj);

    //build beta network
    BetaNode* topNode = new BetaNode();
    //rule1
    /**/
    BetaNode* beta1_1 = new BetaNode(true);
    alpha1_a->betaNodes.push_back(beta1_1);
    topNode->childNode.push_back(beta1_1);

    BetaNode* beta1_2 = new BetaNode(alpha1_b,beta1_1);
    beta1_2->evaluations.push_back(new EvaluationNode(getFunctionAddress("Equal"),0,"val1","val1"));
    beta1_2->evaluations.push_back(new EvaluationNode(getFunctionAddress("Equal"),0,"val2","val2"));

    BetaNode* beta1_3 = new BetaNode(alpha1_c,beta1_2);
    beta1_3->evaluations.push_back(new EvaluationNode(getFunctionAddress("Equal"),1,"val2","val2"));

    BetaNode* beta1_4 = new BetaNode(alpha1_d,beta1_3);
    beta1_4->evaluations.push_back(new EvaluationNode(getFunctionAddress("Equal"),2,"val3","val3"));

    BetaNode* beta1_5 = new BetaNode();
    beta1_4->childNode.push_back(beta1_5);
    beta1_5->ruleToActive = "rule" + patch::to_string(++ruleID);

    //rule2
    BetaNode* beta2_2 = new BetaNode(alpha1_c,beta1_1);
    beta2_2->evaluations.push_back(new EvaluationNode(getFunctionAddress("Equal"),0,"timestamp","timestamp"));
    beta2_2->evaluations.push_back(new EvaluationNode(getFunctionAddress("Equal"),0,"val2","val2"));

    BetaNode* beta2_3 = new BetaNode(alpha1_d,beta2_2);
    beta2_3->evaluations.push_back(new EvaluationNode(getFunctionAddress("Equal"),1,"val1","val1"));

    BetaNode* beta2_4 = new BetaNode(alpha1_e,beta2_3);
    beta2_4->evaluations.push_back(new EvaluationNode(getFunctionAddress("Equal"),2,"val2","val2"));

    BetaNode* beta2_5 = new BetaNode(alpha1_f,beta2_4);
    beta2_5->evaluations.push_back(new EvaluationNode(getFunctionAddress("Equal"),3,"val3","val3"));

    BetaNode* beta2_6 = new BetaNode();
    beta2_5->childNode.push_back(beta2_6);
    beta2_6->ruleToActive = "rule"+ patch::to_string(++ruleID);

    //rule3
    BetaNode* beta3_2 = new BetaNode(alpha1_b,beta1_1);
    beta3_2->evaluations.push_back(new EvaluationNode(getFunctionAddress("Equal"),0,"val1","val1"));
    beta3_2->evaluations.push_back(new EvaluationNode(getFunctionAddress("Equal"),0,"val2","val2"));

    BetaNode* beta3_3 = new BetaNode(alpha1_g,beta3_2);
    beta3_3->evaluations.push_back(new EvaluationNode(getFunctionAddress("Equal"),1,"timestamp","timestamp"));

    BetaNode* beta3_4 = new BetaNode(alpha1_h,beta3_3);
    beta3_4->evaluations.push_back(new EvaluationNode(getFunctionAddress("Equal"),2,"val1","val1"));

    BetaNode* beta3_5 = new BetaNode(alpha1_d,beta3_4);
    beta3_5->evaluations.push_back(new EvaluationNode(getFunctionAddress("Equal"),3,"val2","val2"));

    BetaNode* beta3_6 = new BetaNode();
    beta3_5->childNode.push_back(beta3_6);
    beta3_6->ruleToActive = "rule"+ patch::to_string(++ruleID);

    /**/
    //rule4
    BetaNode* beta4_1 = new BetaNode(true);
    alpha1_i->betaNodes.push_back(beta4_1);
    topNode->childNode.push_back(beta4_1);

    BetaNode* beta4_2 = new BetaNode(alpha1_j,beta4_1);
    beta4_2->evaluations.push_back(new EvaluationNode(getFunctionAddress("Equal"),0,"timestamp","timestamp"));
    beta4_2->evaluations.push_back(new EvaluationNode(getFunctionAddress("Equal"),0,"val2","val2"));

    BetaNode* beta4_3 = new BetaNode(alpha1_a_no,beta4_2);
    beta4_3->evaluations.push_back(new EvaluationNode(getFunctionAddress("Equal"),1,"val1","val1"));

    BetaNode* beta4_4 = new BetaNode();
    beta4_3->childNode.push_back(beta4_4);
    beta4_4->ruleToActive = "rule"+ patch::to_string(++ruleID);

    /**/
	return;
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
