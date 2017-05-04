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
#define THREAD 0

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
int main()
{
    //initialize
    init();
    //build network
	ifstream ifile("rule.txt");
	AlphaNode* root = buildReteNetWork(ifile);
	//AlphaNode* root = buildReteNetWork();
    string sf1 = "person name tom age 10";
    string sf2 = "shop name ibm age 10";
    //read string: convert fact to Fact,token splited by blank space
    Fact* fact1 = readFact(sf1);
    Fact* fact2 = readFact(sf2);

    printFact(fact1);
    printFact(fact2);
    ConnectToAgilor();
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
    //factToAM(fact1,root);
    //factToAM(fact2,root);
#endif

    Sleep(3000);
    DisConnectAgilor();

	system("pause");

    return 0;
}
