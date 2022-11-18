#ifndef fb_h
#define fb_h


#include <iostream>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <cstring>
#include <math.h>
#include "process.h"


#define ARRAYSIZE(array) (sizeof(array) / sizeof(array[0]))
using namespace std;


void printTrace(int n, int total_waiting_time[100][100], process * p[], int lastInstance, int addpar)
{
    cout << "FB-" << addpar
         << "\t"
         << " ";
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
        cout << p[i]->name << "\t";
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
    for (int i = 0; i < 48; i++)
    {
        cout << "-";
    }
    cout << "\n";
}


void printStats(int n, int total_turnaround, process * p[], int addpar)
{
    float totalTurnAround = 0;
    cout << "FB-" << addpar
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
    cout << "|\n";
}

int fb(int n, int lastInstance, process * p[], int addpar, string outmode)
{
    // int tq;
    float avg_turnaround;
    float avg_waiting_time;
    float avg_response_time;
    float cpu_utilisation;
    int total_turnaround = 0;
    int total_waiting_time[100][100];
    int total_response_time = 0;
    int total_idle_time = 0;
    float throughput;
    double remaining_qt[10];
    int idx;



    memset(total_waiting_time, 0, sizeof(total_waiting_time));


    for (int i = 0; i < n; i++)
    {
        p[i]->pid = i + 1;
    }


    for (int i = 0; i < 10; i++)
    {
        remaining_qt[i] = 1;
    }


    sort(p, p + n, compare1);


    queue<int> q[10];
    int currentTime;
    int currentExecProcess = -1;
    for (int i = 0; i < lastInstance; i++)
    {
        currentTime = i;
        for (int x = 0; x < n; x++)
        {
            if (p[x]->arrival == currentTime)
            {
                q[0].push(x);
                p[x]->isWaiting = 1;
            }
            if (p[x]->isWaiting)
            {
                total_waiting_time[x][currentTime] = 1;
            }
        }
        if (currentExecProcess < 0)
        {
            if (!q[0].empty())
            {
                currentExecProcess = q[0].front();
                q[0].pop();
                p[currentExecProcess]->currentQueue = 0;
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
        }
        else
        {
            if (remaining_qt[currentExecProcess] > 0 && p[currentExecProcess]->remainingtime > 0)
            {
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
            else if (remaining_qt[currentExecProcess] == 0 && p[currentExecProcess]->remainingtime > 0)
            {
                p[currentExecProcess]->isWaiting = 1;
                p[currentExecProcess]->currentQueue++;
                
                if (addpar == 1)
                {
                    remaining_qt[currentExecProcess] = 1;
                }
                else
                {
                    remaining_qt[currentExecProcess] = pow(2, p[currentExecProcess]->currentQueue);
                }
                q[p[currentExecProcess]->currentQueue].push(currentExecProcess);
                for (int x = 0; x < ARRAYSIZE(q); x++)
                {
                    if (!q[x].empty())
                    {


                        currentExecProcess = q[x].front();
                        q[x].pop();
                        p[currentExecProcess]->currentQueue = x;
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
                        break;
                    }
                }
            }
            else if (remaining_qt[currentExecProcess] > 0 && p[currentExecProcess]->remainingtime == 0)
            {
                p[currentExecProcess]->isWaiting = 0;
                remaining_qt[currentExecProcess] = 0;


                for (int x = 0; x < ARRAYSIZE(q); x++)
                {
                    if (!q[x].empty())
                    {


                        currentExecProcess = q[x].front();
                        q[x].pop();
                        p[currentExecProcess]->currentQueue = x;
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
                        break;
                    }
                }
            }
            else if (remaining_qt[currentExecProcess] == 0 && p[currentExecProcess]->remainingtime == 0)
            {
                p[currentExecProcess]->isWaiting = 0;
                remaining_qt[currentExecProcess] = 0;
                for (int x = 0; x < ARRAYSIZE(q); x++)
                {
                    if (!q[x].empty())
                    {


                        currentExecProcess = q[x].front();
                        q[x].pop();
                        p[currentExecProcess]->currentQueue = x;
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
                        break;
                    }
                }
            }
        }


        cout << currentExecProcess << "\n";
    }


    for (int y = 0; y < n; y++)
    {
        for (int z = 0; z < lastInstance; z++)
        {
            if (total_waiting_time[y][z] == 1)
            {
                p[y]->waiting_time++;
            }
            if (total_waiting_time[y][z] == 0 || total_waiting_time[y][z] == 2)
            {
                if (z > p[y]->arrival && total_waiting_time[y][z - 1] > 0)
                {
                    total_waiting_time[y][z] == 0 ? p[y]->finish = z : p[y]->finish = z + 1;
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

    if (!outmode.compare("stats") )
    {
    printStats(n, total_turnaround, p, addpar);
    }
    else
    {
    printTrace(n, total_waiting_time, p, lastInstance, addpar);
    }
    // for (int i = 0; i < n; i++)
    // {
    //     cout << "|  " << static_cast<char>('A' + i) << "  \n";
    //     cout << "|  " << p[i]->arrival << "  \n";
    //     cout << "|  " << p[i]->service << "  \n";
    //     cout << "|  " << p[i]->finish << "  \n";
    //     cout << "|  " << p[i]->finish - p[i]->arrival<< "  \n";
    //     cout << "|  " << (p[i]->finish - p[i]->arrival)/p[i]->service << "  \n";


    // }


    // cout << "Average Turnaround Time = " << avg_turnaround << endl;
    // cout << "Average Waiting Time = " << avg_waiting_time << endl;
    // cout << "Average Response Time = " << avg_response_time << endl;
    // cout << "CPU Utilization = " << cpu_utilisation << "%" << endl;
    // cout << "Throughput = " << throughput << " process/unit time" << endl;
}
#endif