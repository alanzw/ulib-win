#include "resource.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include <pdh.h>
#include <pdhmsg.h>

#ifdef _MSC_VER
#pragma comment(lib, "pdh.lib")
#endif

#ifndef PDH_FMT_NOCAP100
#define PDH_FMT_NOCAP100 ((DWORD) 0x00008000)
#endif

LPCTSTR COUNTER_PATH = _T("\\Processor(0)\\% Processor Time");

CONST ULONG SAMPLE_INTERVAL_MS = 1000;

void DisplayCommandLineHelp(void)
{
    printf("The command line must contain a valid log file name.\n");
}

//------------------------------------------------------------------------------------------------------------------
// Prototype(s)...
//------------------------------------------------------------------------------------------------------------------
int cpuusage(void);

//------------------------------------------------------------------------------------------------------------------
// getcpuload()
//   directly prints the CPU usage on screen. This function need to be called twice with a minimum of 1 seconds
//   delay (msdn guideline) to display something usefull.
//   Also returns the usage in percent 0-100 where 100 means the system is working at maximum capacity.
//   Note for multiprocessor systems:
//   If one CPU is working at max capacity the result will (if using (_total) for PdhAddCounter() ) show a maximum
//   workload of 50% unless the other CPU(s) is also working at max load. 
//------------------------------------------------------------------------------------------------------------------
int getcpuload()
{
	static PDH_STATUS            status;
	static PDH_FMT_COUNTERVALUE  value;
	static HQUERY                query;
	static HCOUNTER              counter;
	static DWORD                 ret;
	static char                  runonce=1;
	char                         cput=0;

	if(runonce)
	{
		status = PdhOpenQuery(NULL, 0, &query);
		if(status != ERROR_SUCCESS)
		{
			printf("PdhOpenQuery() ***Error: 0x%X\n",status);
			return 0;
		}

		PdhAddCounter(query, TEXT("\\Processor(_Total)\\% Processor Time"),0,&counter); // A total of ALL CPU's in the system
		//PdhAddCounter(query, TEXT("\\Processor(0)\\% Processor Time"),0,&counter);    // For systems with more than one CPU (Cpu0)
		//PdhAddCounter(query, TEXT("\\Processor(1)\\% Processor Time"),0,&counter);    // For systems with more than one CPU (Cpu1)
		runonce=0;
		PdhCollectQueryData(query); // No error checking here
		return 0;
	}

	status = PdhCollectQueryData(query);
	if(status != ERROR_SUCCESS)
	{
		printf("PhdCollectQueryData() ***Error: 0x%X\n",status);
		if(status==PDH_INVALID_HANDLE) 
			printf("PDH_INVALID_HANDLE\n");
		else if(status==PDH_NO_DATA)
			printf("PDH_NO_DATA\n");
		else
			printf("Unknown error\n");
		return 0;
	}

	status = PdhGetFormattedCounterValue(counter, PDH_FMT_DOUBLE | PDH_FMT_NOCAP100 ,&ret, &value);
	if(status != ERROR_SUCCESS)
	{
		printf("PdhGetFormattedCounterValue() ***Error: 0x%X\n",status);
		return 0;
	}
	cput = value.doubleValue;

	printf("\n\nCPU Total usage: %3d%%\n",cput);

	return cput;
}

int main(int argc, char **argv)
{
/*
	HQUERY hQuery = NULL;
	HLOG hLog = NULL;
	DWORD dwLogType = PDH_LOG_TYPE_CSV;
    HCOUNTER hCounter = NULL;
	PDH_STATUS pdhStatus;
    DWORD dwCount;
    PDH_STATUS status = ERROR_SUCCESS;
    DWORD dwFormat = PDH_FMT_DOUBLE; 
    PDH_FMT_COUNTERVALUE ItemBuffer;

    if (argc != 2) 
    {
        DisplayCommandLineHelp();
        goto cleanup;
    }

	// Open a query object.
    pdhStatus = PdhOpenQuery(NULL, 0, &hQuery);

    if (pdhStatus != ERROR_SUCCESS)
    {
        printf("PdhOpenQuery failed with 0x%x\n", pdhStatus);
        goto cleanup;
    }
	
	// Add one counter that will provide the data.
    pdhStatus = PdhAddCounter(hQuery,
        COUNTER_PATH,
        0,
        &hCounter);

    if (pdhStatus != ERROR_SUCCESS)
    {
        printf("PdhAddCounter failed with 0x%x\n", pdhStatus);
        goto cleanup;
    }

    // Open the log file for write access.
    pdhStatus = PdhOpenLog(argv[1], 
        PDH_LOG_WRITE_ACCESS | PDH_LOG_CREATE_ALWAYS,
        &dwLogType,
        hQuery,
        0, 
        NULL,
        &hLog);

    if (pdhStatus != ERROR_SUCCESS)
    {
        printf("PdhOpenLog failed with 0x%x\n", pdhStatus);
        goto cleanup;
    }
 
    // Write 10 records to the log file.
    for (dwCount = 0; dwCount < 10; dwCount++) 
    {
        printf("Writing record %d\n", dwCount);

        pdhStatus = PdhUpdateLog (hLog, NULL);
        if (ERROR_SUCCESS != pdhStatus)
        {
            printf("PdhUpdateLog failed with 0x%x\n", pdhStatus);
            goto cleanup;
        }

        // Wait one second between samples for a counter update.
        Sleep(SAMPLE_INTERVAL_MS); 
    }
	
	// Close the log file.
    if (hLog)
        PdhCloseLog (hLog, 0);

    // Close the query.
    if (hQuery)
        PdhCloseQuery(hQuery);
	
    // Opens the log file that Writein Performance Data to a Log File 
    // example created.
    status = PdhOpenQuery(argv[1], 0, &hQuery);

    if (ERROR_SUCCESS != status)
    {
        printf("PdhOpenQuery failed with 0x%x\n", status);
        goto cleanup;
    }
   
    // Add the same counter used when writing the log file.
    status = PdhAddCounter(hQuery, COUNTER_PATH, 0, &hCounter);
   
    if (ERROR_SUCCESS != status)
    {
        printf("PdhAddCounter failed with 0x%x\n", status);
        goto cleanup;
    }

    // Read a performance data record.
    status = PdhCollectQueryData(hQuery);

    if (ERROR_SUCCESS != status)
    {
        printf("PdhCollectQueryData failed with 0x%x\n", status);
        goto cleanup;
    }

    while (ERROR_SUCCESS == status) 
    { 
        // Read the next record
        status = PdhCollectQueryData(hQuery);

        if (ERROR_SUCCESS == status)
        {
            // Format the performance data record.
            status = PdhGetFormattedCounterValue(hCounter,
                dwFormat,
                (LPDWORD)NULL,
                &ItemBuffer);

            if (ERROR_SUCCESS != status)
            {
                printf("PdhGetFormattedCounterValue failed with 0x%x.\n", status);
                goto cleanup;
            }

            printf("Formatted counter value = %.20g\n", ItemBuffer.doubleValue);
        }
        else
        {
            if (PDH_NO_MORE_DATA != status)
            {
                printf("PdhCollectQueryData failed with 0x%x\n", status);
            }
        }
    }

cleanup:
	// Close the log file.
    if (hLog)
        PdhCloseLog (hLog, 0);

    // Close the query.
    if (hQuery)
        PdhCloseQuery(hQuery);
*/
	while(1)
	{
		printf("%d\n", getcpuload());
		Sleep(1000);
	}
	return 0;
}

