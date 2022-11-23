/* Thread_EventGenenerator.cpp */

#include <Windows.h>
#include "Multi_Thread.h"
#include "HeapPrioQ.h"
#include "SimParam.h"
using namespace std;
using std::this_thread::sleep_for;

void Thread_DataGen(ThreadParam* pParam)
{
	//ThreadParam* pThrdParam;
	HeapPrioQueue<int>* pPriQ_Event = pParam->pPriQ;
	//TA_CirQ<int>* pCirQ_Event = pParam->pTA_CirQ;
	int myAddr = pParam->myAddr;
	int maxRound = pParam->maxRound;
	int targetDataGen = pParam->targetGenData;
	int genData = 0;
	int genDataCount = 0;
	ThreadMon* pThrdMon = pParam->pThrdMon;

	pParam->pThrdMon->mtx_console.lock();
	cout << "Thread_DataGen[" << myAddr << "] is activated now ..." << endl;
	pParam->pThrdMon->mtx_console.unlock();

	for (int round = 0; round < maxRound; round++)
	{
		if (genDataCount >= targetDataGen)
		{
			if (*pThrdMon->pFlagThreadTerminate == TERMINATE)
				break;
			else {
				sleep_for(std::chrono::milliseconds(500));
				continue;
			}
		}

		genData = TOTAL_NUM_DATA - (round * NUM_DATA_GEN + myAddr + 1);

		//while (pPriQ_Event->insert(genData) == NULL)
		//	sleep_for(std::chrono::milliseconds(100));
		//		while (pPriQ_Event->insert(genData) == NULL)
		//		{
		//			pParam->pCS_main->lock();
		//			cout << "PriQ_Event is Full, waiting ..." << endl;
		//			pParam->pCS_main->unlock();
		//			sleep_for(std::chrono::milliseconds(100));
		//		}
		//		/*pParam->pCS_main->lock();
		//		cout << "Successfully inserted into PriQ_Event " << endl;
		//		pParam->pCS_main->unlock();*/
		//
		//		pParam->pCS_thrd_mon->lock();
		//		pThrdMon->genDataArray[pThrdMon->numGenData] = genData;
		//		pThrdMon->numGenData++;
		//		pThrdMon->totalNumGenData++;
		//		pParam->pCS_thrd_mon->unlock();
		//		//Sleep(100 + rand() % 300);
		//		sleep_for(std::chrono::milliseconds(10));
		//	}

		while (pPriQ_Event->insert(genData) == NULL)
		{
			sleep_for(std::chrono::milliseconds(10));
		}

		pParam->pThrdMon->mtx_thrd_mon.lock();
		/*pThrdMon->genDataArray[myAddr][pThrdMon->numGenData] = genData;
		pThrdMon->totalNumGenData++;
		pThrdMon->numGenData++;*/
		pThrdMon->genDataArray[myAddr][pThrdMon->numGenData[myAddr]] = genData;
		pThrdMon->numGenData[myAddr]++;
		pThrdMon->totalNumGenData++;
		//pParam->pCS_thrd_mon->unlock();
		//Sleep(100 + rand() % 300);
		genDataCount++;
		cout << pThrdMon->numGenData[myAddr];
		pParam->pThrdMon->mtx_thrd_mon.unlock();
		sleep_for(std::chrono::milliseconds(10 + rand() % 10));
	}
}