#ifndef spn_h
#define spn_h

#include<iostream>
#include<string>
#include <queue>
#include "process.h"
using namespace std;

string spn(process * p[], int maxtime, int nump)
{
    string timeline = "";
    vector<process *> q;
    int running = 0;
    for (int i = 0; i < maxtime; i++)
    {
        for (int j = 0; j < nump; j++)
        {
            if (p[j]->arrival == i)
            {
                q.push_back(p[j]);
            }
            
        }

        if (!q.empty())
        {
            if ((q.size() > 1) && (running == 0))
            {
                int mini = maxtime + 1;
                int minindex = 0;
                for (int z = 0; z < q.size(); z++)
                {
                    if (q[z]->service < mini)
                    {
                        mini = q[z]->service;
                        minindex = z;
                    }
                    
                }
                swap(q[0], q[minindex]);
                
            }
            
            if (q.front()->remainingtime == q.front()->service)
            {
                q.front()->start = i;
                running = 1;
            }
            
            if (q.front()->remainingtime != 0)
            {
                timeline.append(q.front()->name,1);
                q.front()->remainingtime--;
            }

            if (q.front()->remainingtime == 0)
            {
                q.front()->finish = i;
                q.front()->turnaround = q.front()->finish - q.front()->arrival;
                q.front()->normturn = q.front()->turnaround/q.front()->service;
                q.erase(q.begin());
                running = 0;
            }
            
        }
        else
        {
             timeline.append(" ",1);
        }
    }
    
    return timeline;

}

#endif