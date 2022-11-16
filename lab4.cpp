#include<iostream>
#include<string>
#include <queue>
#include "process.h"
using namespace std;

#include "process.h"
#include "fcfs.h"
#include "spn.h"
#include "hrrn.h"
#include "aging.h"

int main()
{
    cout << "HI! \n";
    process * p[5];
    for (int i = 0; i < 5; i++)
    {
        p[i] = new process;
    }
    
    int maxtime = 20;
    int np = 5;
    p[0]->name = 'A';
    p[1]->name = 'B';
    p[2]->name = 'C';
    p[3]->name = 'D';
    p[4]->name = 'E';

    p[0]->arrival = 0;
    p[1]->arrival = 2;
    p[2]->arrival = 4;
    p[3]->arrival = 6;
    p[4]->arrival = 8;

    p[0]->service = 3;
    p[1]->service = 6;
    p[2]->service = 4;
    p[3]->service = 5;
    p[4]->service = 2;

    p[0]->remainingtime = 3;
    p[1]->remainingtime = 6;
    p[2]->remainingtime = 4;
    p[3]->remainingtime = 5;
    p[4]->remainingtime = 2;

    string str = hrrn(p, maxtime, np);

    for (int i = 0; i < 5; i++)
    {
        cout << p[i]->name << " " << p[i]->finish;
        cout << "\n";
    }
    

    cout << str << "\n";
    cout << "BYE! \n";

    return 0;
}