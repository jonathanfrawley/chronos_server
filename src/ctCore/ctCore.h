#ifndef  CTCORE_H
#define  CTCORE_H

#define CT_NEW new
#define CT_SAFE_DELETE(x) if(x) delete x; x=NULL;
#define CT_SAFE_DELETE_ARRAY(x) if (x) delete [] x; x=NULL; 

//TODO: Make this more generic. This will only work for POSIX systems, but then again so will pthreads. :)
#include <time.h>

inline void ct_delay(int ms)
{
	struct timespec tv;
	tv.tv_sec = ms/1000;
	tv.tv_nsec = (ms%1000)*1000000;
	nanosleep(&tv, NULL);
}

#endif   // CTCORE_H
