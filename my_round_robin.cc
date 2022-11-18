#include <iostream>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <cstring>
using namespace std;

struct process
{
    int pid;
    int arrival_time;
    int burst_time;
    int start_time;
    int completion_time;
    int turnaround_time;
    int waiting_time = 0;
    int isWaiting = 0;
    int response_time;
};
void printTrace(int n, int total_waiting_time[100][100], process p[], int lastInstance,int tq)
{
    cout << endl;
    cout << "RR-"<<tq
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
void printStats(int n, int total_turnaround_time, process p[],int tq)
{
    float totalTurnAround = 0;
    cout << endl;
    cout << "RR-"<<tq
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
        cout << "|  " << p[i].arrival_time << "  ";
    }
    cout << "|";
    cout << "\n";
    cout << "Service    ";
    for (int i = 0; i < n; i++)
    {
        cout << "|  " << p[i].burst_time << "  ";
    }
    cout << "| ";
    cout << "Mean|";
    cout << "\n";
    cout << "Finish     ";
    for (int i = 0; i < n; i++)
    {
        cout << "| ";
        (float)p[i].completion_time / 10 >= 1 ? cout << "" : cout << " ";
        cout << p[i].completion_time << "  ";
    }
    cout << "|";
    cout << "-----|";
    cout << "\n";
    cout << "Turnaround ";
    for (int i = 0; i < n; i++)
    {
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        cout << "| ";
        (float)p[i].turnaround_time / 10 >= 1 ? cout << "" : cout << " ";
        cout << p[i].turnaround_time << "  ";
        total_turnaround_time += p[i].turnaround_time;
    }
    cout << "|";
    cout << fixed << setprecision(2) << (float)total_turnaround_time / n;
    cout << "|";
    cout << "\n";
    cout << "NormTurn   ";
    for (int i = 0; i < n; i++)
    {
        cout << "| " << fixed << setprecision(2) << (float)p[i].turnaround_time / p[i].burst_time;
        totalTurnAround += (float)p[i].turnaround_time / p[i].burst_time;
    }
    cout << "| ";
    cout << fixed << setprecision(2) << (float)totalTurnAround / n;
    cout << "|";
}
bool compare1(process p1, process p2)
{
    return p1.arrival_time < p2.arrival_time;
}

bool compare2(process p1, process p2)
{
    return p1.pid < p2.pid;
}

int main()
{

    int n;
    int tq;
    int lastInstance;
    struct process p[100];
    float avg_turnaround_time;
    float avg_waiting_time;
    float avg_response_time;
    float cpu_utilisation;
    int total_turnaround_time = 0;
    int total_waiting_time[100][100];
    int total_response_time = 0;
    int total_idle_time = 0;
    float throughput;
    int burst_remaining[10];
    int remaining_qt[10];
    int idx;

    cout << setprecision(2) << fixed;

    cout << "Enter the number of processes: ";
    cin >> n;
    cout << "Enter time quantum: ";
    cin >> tq;
    cout << "Enter last Instance: ";
    cin >> lastInstance;
    memset(total_waiting_time, 0, sizeof(total_waiting_time));

    for (int i = 0; i < n; i++)
    {
        cout << "Enter arrival time of process " << i + 1 << ": ";
        cin >> p[i].arrival_time;
        cout << "Enter burst time of process " << i + 1 << ": ";
        cin >> p[i].burst_time;
        burst_remaining[i] = p[i].burst_time;
        p[i].pid = i + 1;
        cout << endl;
    }

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
            if (p[x].arrival_time == currentTime)
            {
                q.push(x);
                p[x].isWaiting = 1;
            }
            if (p[x].isWaiting)
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
                burst_remaining[currentExecProcess]--;
                remaining_qt[currentExecProcess]--;
                total_waiting_time[currentExecProcess][currentTime] = 2;
            }
        }
        else
        {
            if (remaining_qt[currentExecProcess] > 0 && burst_remaining[currentExecProcess] > 0)
            {
                burst_remaining[currentExecProcess]--;
                remaining_qt[currentExecProcess]--;
                if (burst_remaining[currentExecProcess] == 0)
                {
                    p[currentExecProcess].isWaiting = 0;
                }
                if (remaining_qt[currentExecProcess] == 0 && burst_remaining[currentExecProcess] > 0)
                {
                    p[currentExecProcess].isWaiting = 1;
                }
                total_waiting_time[currentExecProcess][currentTime] = 2;
            }
            else if (remaining_qt[currentExecProcess] == 0 && burst_remaining[currentExecProcess] > 0)
            {
                p[currentExecProcess].isWaiting = 1;
                remaining_qt[currentExecProcess] = tq;
                q.push(currentExecProcess);
                currentExecProcess = q.front();
                cout << "Current Process: " << currentExecProcess << "\n";
                q.pop();
                burst_remaining[currentExecProcess]--;
                remaining_qt[currentExecProcess]--;
                if (burst_remaining[currentExecProcess] == 0)
                {
                    p[currentExecProcess].isWaiting = 0;
                }
                if (remaining_qt[currentExecProcess] == 0 && burst_remaining[currentExecProcess] > 0)
                {
                    p[currentExecProcess].isWaiting = 1;
                }
                cout << "Burst Remaining: " << burst_remaining[currentExecProcess] << "\n";
                cout << "isWaiting: " << p[currentExecProcess].isWaiting << "\n";
                total_waiting_time[currentExecProcess][currentTime] = 2;
            }
            else if (remaining_qt[currentExecProcess] > 0 && burst_remaining[currentExecProcess] == 0)
            {
                p[currentExecProcess].isWaiting = 0;
                remaining_qt[currentExecProcess] = 0;
                currentExecProcess = q.front();
                q.pop();
                burst_remaining[currentExecProcess]--;
                remaining_qt[currentExecProcess]--;
                total_waiting_time[currentExecProcess][currentTime] = 2;
            }
            else if (remaining_qt[currentExecProcess] == 0 && burst_remaining[currentExecProcess] == 0)
            {
                p[currentExecProcess].isWaiting = 0;
                remaining_qt[currentExecProcess] = 0;
                currentExecProcess = q.front();
                cout << "Current Process: " << currentExecProcess << "\n";
                q.pop();
                burst_remaining[currentExecProcess]--;
                remaining_qt[currentExecProcess]--;
                if (burst_remaining[currentExecProcess] == 0)
                {
                    p[currentExecProcess].isWaiting = 0;
                }
                cout << "Burst Remaining: " << burst_remaining[currentExecProcess] << "\n";
                cout << "isWaiting: " << p[currentExecProcess].isWaiting << "\n";
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
                p[y].waiting_time++;
            }
            if (total_waiting_time[y][z] == 0 || total_waiting_time[y][z] == 2)
            {
                if (z > p[y].arrival_time && total_waiting_time[y][z - 1] > 0)
                {
                    total_waiting_time[y][z] == 0 ? p[y].completion_time = z - 1 : p[y].completion_time = z + 1;
                }
            }
        }
    }
    // avg_turnaround_time = (float)total_turnaround_time / n;
    // // avg_waiting_time = (float)total_waiting_time / n;
    // avg_response_time = (float)total_response_time / n;
    // cpu_utilisation = ((p[n - 1].completion_time - total_idle_time) / (float)p[n - 1].completion_time) * 100;
    // throughput = float(n) / (p[n - 1].completion_time - p[0].arrival_time);

    sort(p, p + n, compare2);

    printStats(n, total_turnaround_time, p, tq);
    printTrace(n, total_waiting_time, p, lastInstance, tq);
    // cout << "Average Turnaround Time = " << avg_turnaround_time << endl;
    // cout << "Average Waiting Time = " << avg_waiting_time << endl;
    // cout << "Average Response Time = " << avg_response_time << endl;
    // cout << "CPU Utilization = " << cpu_utilisation << "%" << endl;
    // cout << "Throughput = " << throughput << " process/unit time" << endl;
}