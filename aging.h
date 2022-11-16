#ifndef aging_h
#define aging_h

#include<iostream>
#include<string>
#include <queue>
#include "process.h"
using namespace std;

void aging(process * p[], int maxtime, int nump, int quant, char * timeline[])
{
    vector<process *> q;
    int rq = quant;
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
            if ((q.size() > 1) && (rq == quant))
            {
                float maxp = -1;
                int maxi;
                for (int z = 0; z < q.size(); z++)
                {
                    if (q[z]->priority > maxp)
                    {
                        maxp = q[z]->priority;
                        maxi = z;
                    }
                    
                }

                swap(q[0], q[maxi]);
                
            }
            
            if (q.front()->remainingtime == q.front()->service)
            {
                q.front()->start = i;
            }
            
            if (q.front()->remainingtime != 0)
            {
                *timeline[i] = q.front()->name;
                q.front()->remainingtime--;
                rq--;
            }

            if (q.front()->remainingtime == 0)
            {
                q.front()->finish = i;
                q.front()->turnaround = q.front()->finish - q.front()->arrival;
                q.front()->normturn = q.front()->turnaround/q.front()->service;
                q.erase(q.begin());
                rq = quant;
            }

            if (q.size() > 1)
            {
                for (int ip = 1; ip < q.size(); ip++)
                {
                    q[ip]->priority++;
                }
                
            }
            
            
        }
        else
        {
            *timeline[i] = ' ';
        }
    }
    

}

#endif