#include <bits/stdc++.h> // Thư viện thường được sử dụng trong Competitive Programming

using namespace std;

// Khai báo cấu trúc Process để mô tả thông tin của mỗi tiến trình
struct Process
{
    int pid;                // ID của tiến trình
    int arrivalTime;        // Thời gian đến
    int burstTime;          // Thời gian thực hiện
    int burstTimeRemaining; // Thời gian thực hiện còn lại sau mỗi lần thực hiện
    int completionTime;     // Thời gian hoàn thành
    int turnaroundTime;     // Thời gian quay vòng
    int waitingTime;        // Thời gian chờ
    bool isComplete;        // Đánh dấu xem tiến trình đã hoàn thành hay chưa
    bool inQueue;           // Đánh dấu xem tiến trình đã được thêm vào hàng đợi hay chưa
};

// Hàm kiểm tra và thêm các tiến trình mới vào hàng đợi
void checkForNewArrivals(Process processes[], const int n, const int currentTime, queue<int> &readyQueue)
{
    for (int i = 0; i < n; i++)
    {
        Process p = processes[i];
        // Kiểm tra nếu có tiến trình mới đến
        // Nếu có, thêm vào hàng đợi
        if (p.arrivalTime <= currentTime && !p.inQueue && !p.isComplete)
        {
            processes[i].inQueue = true;
            readyQueue.push(i);
        }
    }
}

// Hàm cập nhật hàng đợi sau mỗi lần thực hiện tiến trình
void updateQueue(Process processes[], const int n, const int quantum, queue<int> &readyQueue, int &currentTime, int &programsExecuted)
{
    int i = readyQueue.front();
    readyQueue.pop();
    // Nếu tiến trình sắp hoàn thành thực hiện
    // Cập nhật thông tin và kiểm tra tiến trình mới đến
    if (processes[i].burstTimeRemaining <= quantum)
    {
        processes[i].isComplete = true;
        currentTime += processes[i].burstTimeRemaining;
        processes[i].completionTime = currentTime;
        processes[i].waitingTime = processes[i].completionTime - processes[i].arrivalTime - processes[i].burstTime;
        processes[i].turnaroundTime = processes[i].waitingTime + processes[i].burstTime;
        if (processes[i].waitingTime < 0)
            processes[i].waitingTime = 0;
        processes[i].burstTimeRemaining = 0;
        // Nếu vẫn còn tiến trình chưa được thêm vào hàng đợi
        // Kiểm tra và thêm tiến trình mới đến vào hàng đợi
        if (programsExecuted != n)
        {
            checkForNewArrivals(processes, n, currentTime, readyQueue);
        }
    }
    // Nếu tiến trình chưa hoàn thành nhưng đã sử dụng hết thời gian quantum
    // Cập nhật thông tin và kiểm tra tiến trình mới đến
    else
    {
        processes[i].burstTimeRemaining -= quantum;
        currentTime += quantum;
        if (programsExecuted != n)
        {
            checkForNewArrivals(processes, n, currentTime, readyQueue);
        }
        // Thêm tiến trình chưa hoàn thành vào hàng đợi để thực hiện tiếp
        readyQueue.push(i);
    }
}

// Hàm xuất kết quả của các tiến trình đã được thực hiện
void output(Process processes[], const int n)
{
    double avgWaitingTime = 0;
    double avgTurntaroundTime = 0;
    // Sắp xếp các tiến trình theo ID
    sort(processes, processes + n, [](const Process &p1, const Process &p2)
         { return p1.pid < p2.pid; });
    // Xuất kết quả của từng tiến trình
    for (int i = 0; i < n; i++)
    {
        // cout << "Process " << processes[i].pid << ": Waiting Time: " << processes[i].waitingTime << " Turnaround Time: "
        //  << processes[i].turnaroundTime << endl;

        cout << left << setw(10) << "PName"
             << left << setw(10) << "Arrtime"
             << left << setw(10) << "Bursttime"
             << left << setw(10) << "Waitting Time"
             << left << setw(10) << "TAT" << endl;

        cout << setw(10) << left << processes[i].pid
             << setw(10) << left << processes[i].arrivalTime
             << setw(10) << left << processes[i].burstTime
             << setw(10) << left << processes[i].waitingTime
             << setw(10) << left << processes[i].turnaroundTime
             << endl;

        avgWaitingTime += processes[i].waitingTime;
        avgTurntaroundTime += processes[i].turnaroundTime;
    }
    cout << "-----------------------\n";

    // Xuất thời gian chờ trung bình và thời gian quay vòng trung bình
    cout << fixed << setprecision(2) << "Thoi gian doi trung binh: " << avgWaitingTime / n << endl;
    cout << fixed << setprecision(2) << "Thoi gian hoan thanh trung binh: " << avgTurntaroundTime / n << endl;
}

// Hàm lập lịch Round Robin cho các tiến trình
void roundRobin(Process processes[], int n, int quantum)
{
    queue<int> readyQueue;
    readyQueue.push(0); // Thêm tiến trình đầu tiên vào hàng đợi
    processes[0].inQueue = true;
    int currentTime = 0;      // Thời gian hiện tại sau mỗi lần thực hiện
    int programsExecuted = 0; // Số lượng tiến trình đã thực hiện
    // Lặp cho đến khi hàng đợi trống
    while (!readyQueue.empty())
    {
        // Cập nhật hàng đợi và thời gian hiện tại sau mỗi lần thực hiện
        updateQueue(processes, n, quantum, readyQueue, currentTime, programsExecuted);
    }
}

int main()
{
    int n, quantum;
    cout << "Nhap so luong process: ";
    cin >> n;
    cout << "Nhap Quantum Time: ";
    cin >> quantum;
    Process processes[n + 1]; // Khai báo mảng các tiến trình
    // Nhập thông tin cho từng tiến trình
    for (int i = 0; i < n; i++)
    {
        cout << "-----------------------\n";
        cout << "ID process: " << i + 1 << endl;
        cout << "Arrival Time: ";
        cin >> processes[i].arrivalTime;
        cout << "Burst Time: ";
        cin >> processes[i].burstTime;
        processes[i].burstTimeRemaining = processes[i].burstTime;
        processes[i].pid = i + 1;
        cout << endl;
    }
    // Sắp xếp các tiến trình theo thời gian đến
    sort(processes, processes + n, [](const Process &p1, const Process &p2)
         { return p1.arrivalTime < p2.arrivalTime; });
    // Thực hiện lập lịch Round Robin
    roundRobin(processes, n, quantum);
    // Xuất kết quả của các tiến trình
    output(processes, n);
    return 0;
}