#ifndef fcfs_h
#define fcfs_h

#include<iostream>
#include<string>
#include <queue>
#include "process.h"

#include <cstring>
#include <algorithm>
#include <iomanip>

using namespace std;

void printTracefcfs(int n, char * timeline[], process * p[], int lastInstance)
{
    cout << "FCFS"
         << "  ";
    for (int i = 0; i < lastInstance+1; i++)
    {
        cout << i % 10 << " ";
    }
    cout << endl;
    for (int i = 0; i < 48; i++)
    {
        cout << "-";
    }
    cout << endl;
    for (int i = 0; i < n; i++)
    {
        cout << p[i]->name << "     ";
        for (int x = 0; x < lastInstance; x++)
        {
            cout << "|";
            if ((*timeline[x] != p[i]->name))
            {
                if ((x < p[i]->arrival))
                {
                    cout << " ";
                }
                else if ((x > p[i]->finish-1))
                {
                    cout << " ";
                }
                else
                {
                    cout << ".";
                }   
            }
            else
            {
                cout << "*";
            }
        }
        cout << "| \n";
    }
    for (int i = 0; i < 48; i++)
    {
        cout << "-";
    }
    cout << "\n";
}
void printStatsfcfs(int n, int total_turnaround, process * p[])
{
    float totalTurnAround = 0;
    cout << "FCFS"
         << "\n";
    cout << "Process    ";
    for (int i = 0; i < n; i++)
    {
        cout << "|  " << p[i]->name << "  ";
    }
    cout << "|";
    cout << "\n";
    cout << "Arrival    ";
    for (int i = 0; i < n; i++)
    {
        cout << "|  " << p[i]->arrival << "  ";
    }
    cout << "|";
    cout << "\n";
    cout << "Service    ";
    for (int i = 0; i < n; i++)
    {
        cout << "|  " << p[i]->service << "  ";
    }
    cout << "| ";
    cout << "Mean|";
    cout << "\n";
    cout << "Finish     ";
    for (int i = 0; i < n; i++)
    {
        cout << "| ";
        (float)p[i]->finish / 10 >= 1 ? cout << "" : cout << " ";
        cout << p[i]->finish << "  ";
    }
    cout << "|";
    cout << "-----|";
    cout << "\n";
    cout << "Turnaround ";
    for (int i = 0; i < n; i++)
    {
        p[i]->turnaround = p[i]->finish - p[i]->arrival;
        cout << "| ";
        (float)p[i]->turnaround / 10 >= 1 ? cout << "" : cout << " ";
        cout << p[i]->turnaround << "  ";
        total_turnaround += p[i]->turnaround;
    }
    cout << "| ";
    cout << fixed << setprecision(2) << (float)total_turnaround / n;
    cout << "|";
    cout << "\n";
    cout << "NormTurn   ";
    for (int i = 0; i < n; i++)
    {
        cout << "| " << fixed << setprecision(2) << (float)p[i]->turnaround / p[i]->service;
        totalTurnAround += (float)p[i]->turnaround / p[i]->service;
    }
    cout << "| ";
    cout << fixed << setprecision(2) << (float)totalTurnAround / n;
    cout << "|\n";
}

void fcfs(process * p[], int maxtime, int nump, char * timeline[], string printmode)
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
                q.front()->finish = i+1;
                q.front()->turnaround = q.front()->finish - q.front()->arrival;
                q.front()->normturn = (float)q.front()->turnaround/(float)q.front()->service;
                q.pop();
            }
            
        }
        else
        {
            *timeline[i] = ' ';
        }
        
        
    }




    if (!printmode.compare("stats") )
    {
    printStatsfcfs(nump, 0, p);
    }
    else
    {
    printTracefcfs(nump, timeline, p, maxtime);
    }
    

}

#endif