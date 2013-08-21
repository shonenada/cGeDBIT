#ifndef CMemMonitor_H
#define CMemMonitor_H

#include <Windows.h>
#include <psapi.h>
#pragma comment(lib,"psapi.lib") 



class CMemMonitor
{
public:
    CMemMonitor(void);
    ~CMemMonitor(void);

    static void updateMem();
    static void reset();



    static long maxMem;
    static long maxWorkingSet;
    static long maxPageUsing;

    static int callTime;

    static PROCESS_MEMORY_COUNTERS pmc;

};

#endif