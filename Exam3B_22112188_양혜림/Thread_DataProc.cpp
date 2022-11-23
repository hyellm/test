/* Thread_EventHandler.cpp */

#include <Windows.h>
#include "Multi_Thread.h"
#include "HeapPrioQ.h"
using namespace std;
using std::this_thread::sleep_for;

void Thread_DataProc(ThreadParam* pParam)
{
	HeapPrioQueue<int>* pPriQ_Event = pParam->pPriQ;
	int myAddr = pParam->myAddr;
	int maxRound = pParam->maxRound;
	int targetEventGen = pParam->targetGenData;
	int* pDeQ_data;
	int deQ_data = 0;
	ThreadMon* pThrdMon = pParam->pThrdMon;

	pParam->pThrdMon->mtx_console.lock();
	cout << "Thread_DataProc[" << myAddr << "] is activated now ..." << endl;
	pParam->pThrdMon->mtx_console.unlock();

	for (int round = 0; round < maxRound; round++)
	{
		if (*pThrdMon->pFlagThreadTerminate == TERMINATE)
			break;

		if (!pPriQ_Event->isEmpty())
		{
			pDeQ_data = pPriQ_Event->removeHeapMin();
			if (pDeQ_data == NULL)
			{
				pParam->pThrdMon->mtx_console.lock();
				//cout << "Thread_DataProc:: removeHeapMin () ==> HeapPrioQ is Empty Now" << endl;
				pParam->pThrdMon->mtx_console.unlock();
			}
			else
			{
				pParam->pThrdMon->mtx_console.lock();
				//cout << "Thread_DataProc:: removeHeapMin (" << setw(3) << *pData_proc << ")" << endl;
				pParam->pThrdMon->mtx_console.unlock();
				pThrdMon->mtx_thrd_mon.lock();
				pThrdMon->procDataArray[myAddr][pThrdMon->numProcData[myAddr]] = *pDeQ_data;
				pThrdMon->totalNumProcData++;
				pThrdMon->numProcData[myAddr]++;
				pThrdMon->mtx_thrd_mon.unlock();
			}
		} // end if
		sleep_for(std::chrono::milliseconds(10 + rand() % 10));
	} // end for
}


