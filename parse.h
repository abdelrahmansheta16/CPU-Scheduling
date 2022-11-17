#ifndef parse_h
#define parse_h

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
#include "scheduler.h"

void parse()
{
    //definitions
    string outmode;
    string smode;
    int maxtime;
    int nump;

    //input
    cin >> outmode;
    cin >> smode;
    cin >> maxtime;
    cin >> nump;
    string pstr[nump];
    for (int i = 0; i < nump; i++)
    {
        cin >> pstr[i];
    }

    //initialize processes
    process * p[nump];
    for (int i = 0; i < nump; i++)
    {
        p[i] = new process;
    }

    //parse processes
    for (int i = 0; i < nump; i++)
    {
        p[i]->name = pstr[i].at(0);
        pstr[i].erase(0,2);

        p[i]->arrival = stoi(pstr[i].substr(0, pstr[i].find(',')));
        pstr[i].erase(0, pstr[i].find(',')+1);

        p[i]->service = stoi(pstr[i].substr(0, pstr[i].find(',')));
        pstr[i].erase(0, pstr[i].find(',')+1);

        p[i]->remainingtime = p[i]->service;
        p[i]->priority = 0;
    }
    
    vector<scheduler> sched;
    while (!smode.empty())
    {
        scheduler stemp;
        stemp.stype = stoi(smode.substr(0,1));
        smode.erase(0,1);
        if (!smode.empty())
        {
            if (smode.at(0) == '-')
            {
                smode.erase(0,1);
                stemp.addpar = stoi(smode.substr(0, smode.find(',')));
                smode.erase(0, smode.find(','));
            }
            smode.erase(0,1);
        }
        sched.push_back(stemp);
    }

    for (int i = 0; i <= sched.size(); i++)
    {
        process * ptemp[nump];
        for (int j = 0; j < nump; j++)
        {
            ptemp[j] = new process;
            ptemp[j]->name = p[j]->name;
            ptemp[j]->arrival = p[j]->arrival;
            ptemp[j]->service = p[j]->service;
            ptemp[j]->remainingtime = p[j]->remainingtime;
            ptemp[j]->priority = p[j]->priority;
        }

        //initialize timeline
        char * timeline[maxtime];
        for (int k = 0; k < maxtime; k++)
        {
            timeline[k] = new char;
        }
        
        //run
        switch (sched.front().stype)
        {
        case 1:
            fcfs(ptemp, maxtime, nump, timeline);
            break;
        case 2:
            /* code */
            break;
        case 3:
            spn(ptemp, maxtime, nump, timeline);
            break;
        case 4:
            /* code */
            break;
        case 5:
            hrrn(ptemp, maxtime, nump, timeline);
            break;
        case 6:
            /* code */
            break;
        case 7:
            /* code */
            break;
        case 8:
            aging(ptemp, maxtime, nump, sched.front().addpar, timeline);
            break;
        default:
            break;
        }

        cout << sched.front().stype << "\n";
        for (int k = 0; k < nump; k++)
        {
            cout << ptemp[k]->name << " " << ptemp[k]->finish << " " << ptemp[k]->turnaround << " " << ptemp[k]->normturn;
            cout << "\n";
        }
        
        for (int k = 0; k < maxtime; k++)
        {
            cout << *timeline[k];
        }

        cout << "\n";

        sched.erase(sched.begin());
    }
}

#endif