#ifndef rr_h
#define rr_h

#include <iostream>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <cstring>
using namespace std;

#include "process.h"

void printTrace(int n, int total_waiting_time[100][100], int lastInstance)
{
    cout << endl;
    cout << "FB-1"
         << "\t"
         << " ";
    for (int i = 0; i < lastInstance; i++)
    {
        cout << i % 10 << " ";
    }
    cout << endl;
    for (int i = 0; i < 50; i++)
    {
        cout << "-";
    }
    cout << endl;
    for (int i = 0; i < n; i++)
    {
        cout << static_cast<char>('A' + i) << "\t";
        for (int x = 0; x < lastInstance; x++)
        {
            cout << "|";
            if (total_waiting_time[i][x] == 0)
            {
                cout << " ";
            }
            else if (total_waiting_time[i][x] == 1)
            {
                cout << ".";
            }
            else if (total_waiting_time[i][x] == 2)
            {
                cout << "*";
            }
        }
        cout << "|\n";
    }
    for (int i = 0; i < 50; i++)
    {
        cout << "-";
    }
}
void printStats(int n, int total_turnaround, process * p[])
{
    float totalTurnAround = 0;
    cout << endl;
    cout << "FB-1"
         << "\n";
    cout << "Process    ";
    for (int i = 0; i < n; i++)
    {
        cout << "|  " << static_cast<char>('A' + i) << "  ";
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
    cout << "|";
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
    cout << "|";
}
bool compare1(process * p1, process * p2)
{
    return p1->arrival < p2->arrival;
}


bool compare2(process * p1, process * p2)
{
    return p1->pid < p2->pid;
}


void rr(int n, int tq, int lastInstance, process * p[], string printmode)
{
    float avg_turnaround;
    float avg_waiting_time;
    float avg_response_time;
    float cpu_utilisation;
    int total_turnaround = 0;
    int total_waiting_time[100][100];
    int total_response_time = 0;
    int total_idle_time = 0;
    float throughput;
    int remaining_qt[10];
    int idx;


    
    memset(total_waiting_time, 0, sizeof(total_waiting_time));

    for (int i = 0; i < 10; i++)
    {
        remaining_qt[i] = tq;
    }


    sort(p, p + n, compare1);


    queue<int> q;
    int currentTime;
    int currentExecProcess = -1;
    for (int i = 0; i < lastInstance; i++)
    {
        currentTime = i;
        for (int x = 0; x < n; x++)
        {
            if (p[x]->arrival == currentTime)
            {
                q.push(x);
                p[x]->isWaiting = 1;
            }
            if (p[x]->isWaiting)
            {
                total_waiting_time[x][currentTime] = 1;
            }
        }
        if (currentExecProcess < 0)
        {
            if (!q.empty())
            {
                currentExecProcess = q.front();
                q.pop();
                p[currentExecProcess]->remainingtime--;
                remaining_qt[currentExecProcess]--;
                total_waiting_time[currentExecProcess][currentTime] = 2;
            }
        }
        else
        {
            if (remaining_qt[currentExecProcess] > 0 && p[currentExecProcess]->remainingtime > 0)
            {
                p[currentExecProcess]->remainingtime--;
                remaining_qt[currentExecProcess]--;
                total_waiting_time[currentExecProcess][currentTime] = 2;
            }
            else if (remaining_qt[currentExecProcess] == 0 && p[currentExecProcess]->remainingtime > 0)
            {
                p[currentExecProcess]->isWaiting = 1;
                remaining_qt[currentExecProcess] = tq;
                q.push(currentExecProcess);
                currentExecProcess = q.front();
                q.pop();
                p[currentExecProcess]->remainingtime--;
                remaining_qt[currentExecProcess]--;
                if (p[currentExecProcess]->remainingtime == 0)
                {
                    p[currentExecProcess]->isWaiting = 0;
                }
                if (remaining_qt[currentExecProcess] == 0 && p[currentExecProcess]->remainingtime > 0)
                {
                    p[currentExecProcess]->isWaiting = 1;
                }
                total_waiting_time[currentExecProcess][currentTime] = 2;
            }
            else if (remaining_qt[currentExecProcess] > 0 && p[currentExecProcess]->remainingtime == 0)
            {
                p[currentExecProcess]->isWaiting = 0;
                remaining_qt[currentExecProcess] = 0;
                currentExecProcess = q.front();
                q.pop();
                p[currentExecProcess]->remainingtime--;
                remaining_qt[currentExecProcess]--;
                total_waiting_time[currentExecProcess][currentTime] = 2;
            }
            else if (remaining_qt[currentExecProcess] == 0 && p[currentExecProcess]->remainingtime == 0)
            {
                p[currentExecProcess]->isWaiting = 0;
                remaining_qt[currentExecProcess] = 0;
                currentExecProcess = q.front();
                q.pop();
                p[currentExecProcess]->remainingtime--;
                remaining_qt[currentExecProcess]--;
                if (p[currentExecProcess]->remainingtime == 0)
                {
                    p[currentExecProcess]->isWaiting = 0;
                }
                total_waiting_time[currentExecProcess][currentTime] = 2;
            }
        }
    }


    for (int y = 0; y < n; y++)
    {
        for (int z = 0; z < lastInstance; z++)
        {
            // cout << total_waiting_time[y][z] << "\n";
            if (total_waiting_time[y][z] == 1)
            {
                p[y]->waiting_time++;
            }
            if (total_waiting_time[y][z] == 0 || total_waiting_time[y][z] == 2)
            {
                if (z > p[y]->arrival && total_waiting_time[y][z - 1] > 0)
                {
                    total_waiting_time[y][z] == 0 ? p[y]->finish = z : p[y]->finish = z+1;
                }
            }
        }
    }
    // avg_turnaround = (float)total_turnaround / n;
    // // avg_waiting_time = (float)total_waiting_time / n;
    // avg_response_time = (float)total_response_time / n;
    // cpu_utilisation = ((p[n - 1].finish - total_idle_time) / (float)p[n - 1].finish) * 100;
    // throughput = float(n) / (p[n - 1].finish - p[0].arrival);


    sort(p, p + n, compare2);

    if (!printmode.compare("stats") )
    {
        printStats(n, total_turnaround, p);

    }
    else
    {
        printTrace(n, total_waiting_time, lastInstance);
    }
    
    
    // cout << "Average Turnaround Time = " << avg_turnaround << endl;
    // cout << "Average Waiting Time = " << avg_waiting_time << endl;
    // cout << "Average Response Time = " << avg_response_time << endl;
    // cout << "CPU Utilization = " << cpu_utilisation << "%" << endl;
    // cout << "Throughput = " << throughput << " process/unit time" << endl;
}

#endif