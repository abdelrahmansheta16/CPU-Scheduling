#ifndef process_h
#define process_h

struct process
{
    char name;
    int arrival;
    int service;
    int remainingtime;
    int finish;
    int start;
    int turnaround;
    float normturn;
    int priority;
    int pid;
    int waiting_time = 0;
    int isWaiting = 0;
    int response_time;
};


#endif
