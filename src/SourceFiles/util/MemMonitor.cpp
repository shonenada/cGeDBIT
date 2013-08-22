#include "../../HeaderFiles/util/MemMonitor.h"


CMemMonitor::CMemMonitor(void)
{

}


CMemMonitor::~CMemMonitor(void)
{
}

 long CMemMonitor::maxMem =0;
 long CMemMonitor::maxWorkingSet=0;
 long CMemMonitor::maxPageUsing=0;

 int CMemMonitor::callTime=0;

 PROCESS_MEMORY_COUNTERS CMemMonitor:: pmc;

void CMemMonitor::updateMem()
{
	GetProcessMemoryInfo(GetCurrentProcess(),&pmc,sizeof(pmc));
    callTime++;
    if(pmc.WorkingSetSize+pmc.PagefileUsage >  maxMem)
        maxMem = pmc.WorkingSetSize+pmc.PagefileUsage;
    if(pmc.WorkingSetSize > maxWorkingSet)
        maxWorkingSet = pmc.WorkingSetSize;
    if(pmc.PagefileUsage > maxPageUsing)
        maxPageUsing = pmc.PagefileUsage; 

}

void CMemMonitor::reset()
{
    maxMem = 0;
    maxWorkingSet = 0;
    maxPageUsing = 0;
}