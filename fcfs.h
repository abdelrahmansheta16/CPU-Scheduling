#ifndef fcfs_h
#define fcfs_h

#include<iostream>
#include<string>
#include <queue>
#include "process.h"
using namespace std;

string fcfs(process p[], int maxtime, int nump)
{
    string timeline = "";
    queue<process> q;
    for (int i = 0; i < sizeof(maxtime); i++)
    {
        for (int j = 0; j < nump; j++)
        {
            if (p[j].arrival == i)
            {
                q.push(p[j]);
            }
            
        }

        if (!q.empty())
        {
            if (q.front().remainingtime == q.front().service)
            {
                q.front().start = i;
            }
            
            if (q.front().remainingtime != 0)
            {
                timeline.append(q.front().name,1);
                q.front().remainingtime--;
            }

            if (q.front().remainingtime == 0)
            {
                q.front().finish = i;
                q.front().turnaround = q.front().finish - q.front().arrival;
                q.front().normturn = q.front().turnaround/q.front().service;
                q.pop();
            }
            
        }
        
        
    }
    
    return timeline;

}

#endif