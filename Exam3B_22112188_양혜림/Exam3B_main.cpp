/*
- 파일명: Exam3B_main.cpp
- 프로그램의 목적 및 기본 기능
  : class T_Array, class CompleteBinaryTree, class HeapPrioQ
- 프로그램 작성자: 양혜림 (22.11.19)
- 최종 수정: 양혜림 (22.11.19)
*/

#include <time.h>
#include <conio.h>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <fstream>
#include "Multi_Thread.h"
#include "SimParam.h"
#include "HeapPrioQ.h"

#define INITIAL_CBT_CAPA 100

void main()
{
	HeapPrioQueue<int> heapPriQ_Event(30, string("HeapPriorityQueue_Event"));
	ThreadParam thrdParam_Gen[NUM_DATA_GEN], thrdParam_Proc[NUM_DATA_PROC];
	mutex cs_main, cs_thrd_mon;
	ThreadMon thrdMon;
	HANDLE consHndlr;
	THREAD_FLAG thrd_flag = RUN;
	// int numEventGenerated, numEventProcessed, num_events_in_PrioQ;

	thread thrd_dataProc[NUM_DATA_PROC];
	thread thrd_dataGen[NUM_DATA_GEN];
	HeapPrioQueue<int> HeapPriQ_int(INITIAL_CBT_CAPA, string("Heap_Priority_Queue_Int"));

	for (int p = 0; p < NUM_DATA_PROC; p++)
	{
		for (int i = 0; i < TOTAL_NUM_DATA; i++)
			thrdMon.procDataArray[p][i] = -1;
		thrdMon.numProcData[p] = 0;
		thrdParam_Proc[p].myAddr = p;
		thrdParam_Proc[p].pPriQ = &HeapPriQ_int;
		thrdParam_Proc[p].targetGenData = NUM_DATA_PER_GEN;
		thrdParam_Proc[p].maxRound = MAX_ROUNDS;
		thrdParam_Proc[p].pThrdMon = &thrdMon;
		thrd_dataProc[p] = thread(Thread_DataProc, &thrdParam_Proc[p]);
	}
	for (int g = 0; g < NUM_DATA_GEN; g++)
	{
		for (int i = 0; i < TOTAL_NUM_DATA; i++)
			thrdMon.genDataArray[g][i] = -1;
		thrdMon.numGenData[g] = 0;
		//thrdParam_Gen[g].dataList = dataList;
		thrdParam_Gen[g].myAddr = g;
		thrdParam_Gen[g].pPriQ = &HeapPriQ_int;
		thrdParam_Gen[g].targetGenData = NUM_DATA_PER_GEN;
		thrdParam_Gen[g].maxRound = MAX_ROUNDS;
		thrdParam_Gen[g].pThrdMon = &thrdMon;
		thrd_dataGen[g] = thread(Thread_DataGen, &thrdParam_Gen[g]);
	}
	thrdMon.mtx_console.lock();
	cout << "Testing " << HeapPriQ_int.getName() << "with " << NUM_DATA_GEN << " data generators and ";
	cout << NUM_DATA_PROC << " data processors" << endl;
	thrdMon.mtx_console.unlock();
	for (int round = 0; round < MAX_ROUNDS; round++)
	{
		thrdMon.mtx_console.lock();
		cout << "Round (" << setw(3) << round << ") : totalDataGenerated = " << setw(3) << thrdMon.totalNumGenData;
		cout << ", totalDataProcessed = " << setw(3) << thrdMon.totalNumProcData << endl;
		for (int g = 0; g < NUM_DATA_GEN; g++)
		{
			//cout << "thrdDataGen[" << g << "] generated " << thrdMon.numGenData[g] << " data" << endl;
		}
		for (int p = 0; p < NUM_DATA_PROC; p++)
		{
			//cout << "thrdDataProc[" << p << "] processed " << thrdMon.numProcData[p] << " data" << endl;
		}
		thrdMon.mtx_console.unlock();
		if (thrdMon.totalNumProcData >= TOTAL_NUM_DATA)
		{
			thrd_flag = TERMINATE;
			break;
		}
		Sleep(100);
	}
	for (int g = 0; g < NUM_DATA_GEN; g++)
	{
		thrd_dataGen[g].join();
	}
	for (int p = 0; p < NUM_DATA_PROC; p++)
	{
		thrd_dataProc[p].join();
	}
	int count = 0;
	for (int g = 0; g < NUM_DATA_GEN; g++)
	{
		cout << "Thread_Gen[" << g << "] generated " << thrdMon.numGenData[g] << " data :" << endl;
		count = 0;
		for (int i = 0; i < thrdMon.numGenData[g]; i++)
		{
			cout << setw(5) << thrdMon.genDataArray[g][i];
			count++;
			if ((count % 20) == 0)
				cout << endl;
		}
		cout << endl;
	}
	for (int p = 0; p < NUM_DATA_PROC; p++)
	{
		cout << "Thread_Proc[" << p << "] processed " << thrdMon.numProcData[p] << " data :" << endl;
		count = 0;
		for (int i = 0; i < thrdMon.numProcData[p]; i++)
		{
			cout << setw(5) << thrdMon.procDataArray[p][i];
			count++;
			if ((count % 20) == 0)
				cout << endl;
		}
		cout << endl;
	}
 
} // end main();
