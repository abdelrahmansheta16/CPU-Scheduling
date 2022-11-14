#include<iostream>
#include<string.h>
#include <queue>
#include "process.h"
using namespace std;

void fcfs(process p[], int maxtime, int nump)
{
    char timeline[maxtime];
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
            
            if (q.front().remainingtime == 0)
            {
                q.front().finish = i;
                q.front().turnaround = q.front().finish - q.front().arrival;
                q.front().normturn = q.front().turnaround/q.front().service;
                q.pop();
            }
            else
            {
                timeline[i] = q.front().name;
                q.front().remainingtime--;
            }
            
        }
        
        
    }
    
}

int main()
{

    return 0;
}