#include <stdio.h>
#include <iostream>
#include <queue>
#include <iomanip>

using namespace std;

struct Process
{
    int name;
    int burst_time;
    int arrival_time;
};

static double avg_turn_around_time = 0;
static double avg_waiting_time = 0;

void swap(Process &p1, Process &p2)
{
    Process tmp;
    tmp = p1;
    p1 = p2;
    p2 = tmp;
}

void sortByArrivalTime(Process *p, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (p[i].arrival_time < p[j].arrival_time)
                swap(p[i], p[j]);
        }
    }
}

void Input(Process *p, int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << "-----------------------\n";
        cout << "Nhap ID process: ";
        cin >> p[i].name;
        srand(time(NULL));
        cout << "Arrival Time: ";
        p[i].arrival_time = rand() % 21;
        cout << p[i].arrival_time << endl;
        cout << "Burst Time: ";
       p[i].burst_time = rand() % 11 + 2;
        cout << p[i].burst_time << endl;

    }
}

void sortByBurstTime(Process *p, int n, int time_current)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (p[i].burst_time < p[j].burst_time && p[i].arrival_time <= time_current)
            {
                swap(p[i], p[j]);
            }
        }
    }
}

void SelectionFunction(Process *p, int n)
{

    int time_current;

    // Sort theo arrival time
    sortByArrivalTime(p, n);

    // Ham lua chon quyet dinh xem process nao vao queue truoc;
    time_current = p[n - 1].arrival_time;

    for (int i = 0; i < n; n--)
    {
        time_current += p[n - 1].burst_time;

        avg_waiting_time += time_current - p[n - 1].arrival_time - p[n - 1].burst_time;

        avg_turn_around_time += (time_current - p[n - 1].arrival_time);

        cout << setw(10) << left << p[n - 1].name
             << setw(10) << left << p[n - 1].arrival_time
             << setw(10) << left << p[n - 1].burst_time
             << setw(10) << left << time_current - p[n - 1].burst_time
             << setw(10) << left << time_current - p[n - 1].arrival_time
             << setw(10) << left << time_current
             << endl;
        sortByBurstTime(p, n - 1, time_current);
    }
}

int main()
{
    Process *p = new Process[100];
    queue<Process> pQueue;
    int n;

    cout << "Nhap so luong process: ";
    cin >> n;

    Input(p, n);

    cout << "\n-----------------------\n";

    cout << left << setw(10) << "PName"
         << left << setw(10) << "Arrtime"
         << left << setw(10) << "Bursttime"
         << left << setw(10) << "Start"
         << left << setw(10) << "TAT"
         << left << setw(10) << "Finish" << endl;
    SelectionFunction(p, n);

    cout << "-----------------------\n";

    cout << "Thoi gian dap ung trung bình: " << avg_waiting_time / n << endl;

    cout << "Thoi gian doi trung bình: " << avg_waiting_time / n << endl;

    cout << "Thoi gian hoan thanh trung binh: " << avg_turn_around_time / n << endl;

    return 0;
}