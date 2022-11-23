/* Multi_thread.h */

#ifndef MULTI_THREAD__H
#define MULTI_THREAD__H

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <thread>
#include <mutex>
#include <process.h>
#include <string>
#include "HeapPrioQ.h"
#include "T_Array.h"
#include "SimParam.h"
using namespace std;

enum ROLE { EVENT_GENERATOR, EVENT_HANDLER };
enum THREAD_FLAG { INITIALIZE, RUN, TERMINATE };

typedef struct
{
	mutex mtx_thrd_mon;
	mutex mtx_console;
	int numGenData[NUM_DATA_GEN];
	int numProcData[NUM_DATA_PROC];
	int totalNumGenData;
	int totalNumProcData;
	int genDataArray[NUM_DATA_GEN][TOTAL_NUM_DATA]; // used for monitoring only
	int procDataArray[NUM_DATA_PROC][TOTAL_NUM_DATA]; // used for monitoring only
	THREAD_FLAG* pFlagThreadTerminate;
} ThreadMon;

typedef struct
{
	HeapPrioQueue<int>* pPriQ;
	int myAddr;
	int maxRound;
	int targetGenData;
	ThreadMon* pThrdMon;
} ThreadParam;

void Thread_DataGen(ThreadParam* pParam);
void Thread_DataProc(ThreadParam* pParam);

#endif