#ifdef _WIN32
#include <Windows.h>
#include <winnt.h>
#include "usleep.h"

void usleep(__int64 microseconds)
{
    static int init = 0;

    if (init == 0)
    {
        init = 1;

        // Increase the accuracy of the timer once.
        const HINSTANCE ntdll = LoadLibrary("ntdll.dll");
	    if (ntdll != NULL)
	    {
            typedef long(NTAPI* pNtQueryTimerResolution)(unsigned long* MinimumResolution, unsigned long* MaximumResolution, unsigned long* CurrentResolution);
		    typedef long(NTAPI* pNtSetTimerResolution)(unsigned long RequestedResolution, char SetResolution, unsigned long* ActualResolution);

		    pNtQueryTimerResolution NtQueryTimerResolution = (pNtQueryTimerResolution)GetProcAddress(ntdll, "NtQueryTimerResolution");
            pNtSetTimerResolution   NtSetTimerResolution   = (pNtSetTimerResolution)  GetProcAddress(ntdll, "NtSetTimerResolution");
		    if (NtQueryTimerResolution != NULL &&
                NtSetTimerResolution   != NULL)
		    {
                // Query for the highest accuracy timer resolution.
                unsigned long minimum, maximum, current;
                NtQueryTimerResolution(&minimum, &maximum, &current);
			    
                // Set the timer resolution to the highest.
			    NtSetTimerResolution(maximum, (char)1, &current);
		    }

		    // We can decrement the internal reference count by one
		    // and NTDLL.DLL still remains loaded in the process.
		    FreeLibrary(ntdll);
	    }
    }

	// Convert from microseconds to 100 of ns, and negative for relative time.
    LARGE_INTEGER sleepPeriod;
	sleepPeriod.QuadPart = -(10*microseconds);

    // Create the timer, sleep until time has passed, and clean up.
    HANDLE timer = CreateWaitableTimerEx(NULL, NULL, CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS);
	SetWaitableTimer(timer, &sleepPeriod, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}
#endif
