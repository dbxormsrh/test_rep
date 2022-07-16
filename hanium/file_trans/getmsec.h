#include <stdio.h>
#include <time.h>
#include <sys/timeb.h>
void getmsec(){
        struct timeb itb;
        struct tm *lt;
        ftime(&itb);
        lt = localtime(&itb.time);
        printf("%02d:%02d:%02d.%03d\n", lt->tm_hour, lt->tm_min, lt->tm_sec, itb.millitm);
}


