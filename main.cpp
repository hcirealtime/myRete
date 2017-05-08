#include <iostream>
#include <fstream>
#include <process.h>
#include "fact.h"
#include "test.h"
#include "help.h"
#include "evaluation.h"
#include "constant.h"
#include "rtdb.h"
using namespace std;
#define THREAD 1

/**
初始化工作：
1.constant中的全局变量
2.初始化函数表
**/
void init()
{
    InitializeCriticalSectionAndSpinCount(&g_print,1000);
    InitializeCriticalSectionAndSpinCount(&g_cs,1000);
    initFunctionTable();

}
void test(ElementTypeNode* root)
{
    string sf1 = "person name tom age 10";
    string sf2 = "shop name ibm age 10";
    //read string: convert fact to Fact,token splited by blank space
    Fact* fact1 = readFact(sf1);
    Fact* fact2 = readFact(sf2);

    printFact(fact1);
    printFact(fact2);
    //ConnectToAgilor();
    cout << "**********************" << endl;

#if THREAD
    ThreadNode* env1 = new ThreadNode();
    ThreadNode* env2 = new ThreadNode();
    // start beta network thread
    HANDLE hThread1 = (HANDLE)_beginthreadex(NULL,0, MoveOnJoinNetworkThread, env1, 0, NULL);
    //SetThreadAffinityMask(hThread1, 1<<1);
    HANDLE hThread2 = (HANDLE)_beginthreadex(NULL,0, MoveOnJoinNetworkThread, env2, 0, NULL);

    //alpha network:fact to AM
    factToAMOnThread(fact1,root);
    factToAMOnThread(fact2,root);
    Sleep(3000);
#else
    //alpha network:fact to AM
    factToAM(fact1,root);
    factToAM(fact2,root);
#endif

    //DisConnectAgilor();
}
void test(ifstream& fact_file,int num_of_fact)
{
    string R[10] = {"ClassA","ClassB","ClassC","ClassD","ClassE","ClassF","ClassG","ClassH","ClassI","ClassJ"};
    string r[10] = {"Class_a","Class_b","Class_c","Class_d","Class_e","Class_f","Class_g","Class_h","Class_i","Class_j"};
    vector<string> x(R,R+10);
    vector<string> y(r,r+10);
    ElementTypeNode* root = new ElementTypeNode();
    int ruleID = 0;
    buildReteNetWork4Rules(root,x,ruleID);
    buildReteNetWork4Rules(root,y,ruleID);

#if THREAD
    ThreadNode* env1 = new ThreadNode();
    ThreadNode* env2 = new ThreadNode();
    // start beta network thread
    HANDLE hThread1 = (HANDLE)_beginthreadex(NULL,0, MoveOnJoinNetworkThread, env1, 0, NULL);
    HANDLE hThread2 = (HANDLE)_beginthreadex(NULL,0, MoveOnJoinNetworkThread, env2, 0, NULL);
#endif

    string line;
    int num = 1;
    LARGE_INTEGER curTime;
    QueryPerformanceCounter(&curTime);

    printf("start : %lld\n",curTime.QuadPart);
    while(getline(fact_file,line))
    {
        Fact* f = readFactWithBracket(line);
        if(num++ > num_of_fact)break;

#if THREAD
        factToAMOnThread(f,root);
#else
        factToAM(f,root);
#endif
    }
     QueryPerformanceCounter(&curTime);

    printf("end : %lld\n",curTime.QuadPart);
#if THREAD
    Sleep(35000);
#endif
    return ;
}
int main()
{
    //initialize
    init();

    //build network
    //test 1
	//ifstream ifile("rule.txt");
	//AlphaNode* root = buildReteNetWork(ifile);

    //test 2
	//ElementTypeNode* root = buildReteNetWork();
    //test(root);

    //test3 for 8 rules
    ifstream factFile("D:\\ForWork\\C++\\MyRete\\Fact.txt");
    test(factFile,80000);


    //system("pause");
    return 0;
}
