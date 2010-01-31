//
// CountC.c -- Tracks the number of active client threads
//

#include "httpMT.h"

DWORD gdwClientCount = 0;
CRITICAL_SECTION gcriticalClients;
HANDLE ghNoClients;

////////////////////////////////////////////////////////////

HANDLE InitClientCount(void)
{
	gdwClientCount = 0;

	InitializeCriticalSection(&gcriticalClients);

	//
	// Create the "no clients" signal event object
	//
	ghNoClients = CreateEvent(NULL,		// Security
							 TRUE,		// Manual reset
							 TRUE,		// Initial State
							 NULL);		// Name
	return ghNoClients;
}

////////////////////////////////////////////////////////////

void IncrementClientCount(void)
{
	EnterCriticalSection(&gcriticalClients);
	gdwClientCount++;
	LeaveCriticalSection(&gcriticalClients);
	ResetEvent(ghNoClients);
}

////////////////////////////////////////////////////////////

void DecrementClientCount(void)
{
	EnterCriticalSection(&gcriticalClients);
	if (gdwClientCount > 0)
		gdwClientCount--;
	LeaveCriticalSection(&gcriticalClients);
	if (gdwClientCount < 1)
		SetEvent(ghNoClients);
}

////////////////////////////////////////////////////////////

void DeleteClientCount(void)
{
	DeleteCriticalSection(&gcriticalClients);
	CloseHandle(ghNoClients);
}

