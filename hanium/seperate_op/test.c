#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>

#define w 352
#define h 288
#define FRAME_NUM 2000
#define BUFF_SIZE 1024

void getmsec();

char* getCID(){
	time_t     tm_time;
	struct tm* st_time;
    
	time(&tm_time);
	st_time = localtime(&tm_time);
    static char date[BUFF_SIZE];
    strftime(date, BUFF_SIZE, "%Y.%m.%d-%l:%M:%S", st_time);;
	return date;
}

void getmsec(){
    struct timeb itb;
    struct tm *lt;
    ftime(&itb);
    lt = localtime(&itb.time);
    printf("%02d:%02d:%02d.%03d\n", lt->tm_hour, lt->tm_min, lt->tm_sec, itb.millitm);
}