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
};


#endif
