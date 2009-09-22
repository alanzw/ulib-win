#include <ctime>
#include <windows.h>

#include "utimer.h"

UTimer::UTimer()
{
    start_count=0;
    end_count=0;
}

UTimer::~UTimer(){};

void UTimer::begin()
{
    start_count = clock();
}

void UTimer::stop()
{
    end_count = clock();
}

double UTimer::getSeconds()
{
    return (double)( end_count - start_count )/CLOCKS_PER_SEC;
}

void UTimer::clear()
{
    start_count=0;
    end_count=0;
}

#ifdef WINVER
void UTimer::beginQuery()
{
    LARGE_INTEGER litmp;

    bool OK = QueryPerformanceFrequency(&litmp);
    if (OK){
        freqence = litmp.QuadPart;
        QueryPerformanceCounter(&litmp);
        start_count = litmp.QuadPart;
    } else {
 //       std::cout << "Not support...\n";
    }
}

void UTimer::stopQuery()
{
    LARGE_INTEGER litmp;
    QueryPerformanceCounter(&litmp);
    end_count =  litmp.QuadPart;
}

double UTimer::getSecondsQuery()
{
    return (double)( end_count - start_count )/freqence;
}

#endif // WINVER
