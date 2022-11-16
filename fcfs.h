#ifndef fcfs_h
#define fcfs_h

#include<iostream>
#include<string>
#include <queue>
#include "process.h"
using namespace std;

void fcfs(process * p[], int maxtime, int nump, char * timeline[])
{
    queue<process *> q;
    for (int i = 0; i < maxtime; i++)
    {
        for (int j = 0; j < nump; j++)
        {
            if (p[j]->arrival == i)
            {
                q.push(p[j]);
            }
            
        }

        if (!q.empty())
        {
            if (q.front()->remainingtime == q.front()->service)
            {
                q.front()->start = i;
            }
            
            if (q.front()->remainingtime != 0)
            {
                *timeline[i] = q.front()->name;
                q.front()->remainingtime--;
            }

            if (q.front()->remainingtime == 0)
            {
                q.front()->finish = i;
                q.front()->turnaround = q.front()->finish - q.front()->arrival;
                q.front()->normturn = q.front()->turnaround/q.front()->service;
                q.pop();
            }
            
        }
        else
        {
            *timeline[i] = ' ';
        }
        
        
    }
    

}

#endif