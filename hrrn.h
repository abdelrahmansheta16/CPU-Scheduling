#ifndef hrrn_h
#define hrrn_h

#include<iostream>
#include<string>
#include <queue>
#include "process.h"
using namespace std;

void hrrn(process * p[], int maxtime, int nump, char * timeline[])
{
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
                float maxr = -1;
                int maxi;
                for (int z = 0; z < q.size(); z++)
                {
                    float w = (i - q[z]->arrival + q[z]->service)/q[z]->service;
                    if (w > maxr)
                    {
                        maxr = w;
                        maxi = z;
                    }
                    
                }

                swap(q[0], q[maxi]);
                
            }
            
            if (q.front()->remainingtime == q.front()->service)
            {
                q.front()->start = i;
                running = 1;
            }
            
            if (q.front()->remainingtime != 0)
            {
                *timeline[i] = q.front()->name;
                q.front()->remainingtime--;
            }

            if (q.front()->remainingtime == 0)
            {
                q.front()->finish = i+1;
                q.front()->turnaround = q.front()->finish - q.front()->arrival;
                q.front()->normturn = (float)q.front()->turnaround/(float)q.front()->service;
                q.erase(q.begin());
                running = 0;
            }
            
        }
        else
        {
            *timeline[i] = ' ';
        }
    }

}

#endif