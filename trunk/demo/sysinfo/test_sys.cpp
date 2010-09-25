#include "resource.h"

#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <assert.h>
using namespace std;

#include "usys.h"
#include "umsg.h"


/// 时间转换
static uint64_t file_time_2_utc(const FILETIME* ftime)
{
	LARGE_INTEGER li;

	assert(ftime);
	li.LowPart = ftime->dwLowDateTime;
	li.HighPart = ftime->dwHighDateTime;
	return li.QuadPart;
}


/// 获得CPU的核数
static int get_processor_number()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return (int)info.dwNumberOfProcessors;
}




int get_cpu_usage()
{
	//cpu数量
	static int processor_count_ = -1;
	//上一次的时间
	static int64_t last_time_ = 0;
	static int64_t last_system_time_ = 0;


	FILETIME now;
	FILETIME creation_time;
	FILETIME exit_time;
	FILETIME kernel_time;
	FILETIME user_time;
	int64_t system_time;
	int64_t time;
	int64_t system_time_delta;
	int64_t time_delta;

	int cpu = -1;


	if(processor_count_ == -1)
	{
		processor_count_ = get_processor_number();
	}

	GetSystemTimeAsFileTime(&now);

	if (!GetProcessTimes(GetCurrentProcess(), &creation_time, &exit_time,
		&kernel_time, &user_time))
	{
		// We don't assert here because in some cases (such as in the Task Manager)
		// we may call this function on a process that has just exited but we have
		// not yet received the notification.
		return -1;
	}
	system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time))/processor_count_;
	time = file_time_2_utc(&now);

	if ((last_system_time_ == 0) || (last_time_ == 0))
	{
		// First call, just set the last values.
		last_system_time_ = system_time;
		last_time_ = time;
		return -1;
	}

	system_time_delta = system_time - last_system_time_;
	time_delta = time - last_time_;

	assert(time_delta != 0);

	if (time_delta == 0)
		return -1;

	// We add time_delta / 2 so the result is rounded.
	cpu = (int)((system_time_delta * 100 + time_delta / 2) / time_delta);
	last_system_time_ = system_time;
	last_time_ = time;
	return cpu;
}



int get_memory_usage(uint64_t* mem, uint64_t* vmem)
{
	PROCESS_MEMORY_COUNTERS pmc;
	if(GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
	{
		if(mem) *mem = pmc.WorkingSetSize;
		if(vmem) *vmem = pmc.PagefileUsage;
		return 0;
	}
	return -1;
}


/*
int get_io_bytes(uint64_t* read_bytes, uint64_t* write_bytes)
{
	IO_COUNTERS io_counter;
	if(GetProcessIoCounters(GetCurrentProcess(), &io_counter))
	{
		if(read_bytes) *read_bytes = io_counter.ReadTransferCount;
		if(write_bytes) *write_bytes = io_counter.WriteTransferCount;
		return 0;
	}
	return -1;
}
*/


int main(int argc, char *argv[])
{

    HGLOBAL h= ::GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, 100);

    int *p = (int *)h;

    cout << p << " : " << GlobalSize(h) << endl;

    h = ::GlobalReAlloc(h, 120, 0);

    p = (int *)h;

    cout << p << " : " << GlobalSize(h) << endl;

    h = ::GlobalReAlloc(h, 140, 0);

    if (!h)
    {
        showError("Error");
    }

    p = (int *)h;

    cout << p << " : " << GlobalSize(h) << endl;

    ::GlobalFree(h);


    USystem usys;

    cout << "Screen Width: " << usys.getScreenWidth()  << " pixels"  << endl;
    cout << "Screen Height: " << usys.getScreenHeight() << " pixels" << endl;

    //
    cout << "Minimized window arranged: " << usys.getArrange() << endl;
    cout << "Arrange mode: "  << endl;
    cout << "    " << usys.getArrangeText() << endl;
    //
    cout << "Boot Mode: " << usys.getBootMode() << " -- " << usys.getBootModeText() << endl;

    //
    cout << "Monitor Number: " << usys.getMonitorNum() << endl;
    //
    cout << "Mouse Number: " << usys.getMouseButtonNum() << endl;

    //
    cout << "Computer Name: " << usys.getComputerName() << endl;

   SYSTEM_INFO siSysInfo;

   // Copy the hardware information to the SYSTEM_INFO structure.

   GetSystemInfo(&siSysInfo);

   // Display the contents of the SYSTEM_INFO structure.

    printf("Hardware information: \n");
    printf("  OEM ID: %u\n", siSysInfo.dwOemId);
    printf("  Number of processors: %u\n",
    siSysInfo.dwNumberOfProcessors);
    printf("  Page size: %u\n", siSysInfo.dwPageSize);
    printf("  Processor type: %u\n", siSysInfo.dwProcessorType);
    printf("  Minimum application address: %lx\n",
    siSysInfo.lpMinimumApplicationAddress);
    printf("  Maximum application address: %lx\n",
    siSysInfo.lpMaximumApplicationAddress);
    printf("  Active processor mask: %u\n",
    siSysInfo.dwActiveProcessorMask);

    //
    char buffer[256];
    getXPKey(buffer);
    printf("XP Key: %s\n", buffer);

    //
    cout << "Press any key to exit..." << endl;

    cin.ignore();

    return 0;
}


