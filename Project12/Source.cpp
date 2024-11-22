#include <iostream>
#include <json.hpp>
#include <cfloat> 
#include<string>
#include <fstream>
using namespace std;
using json = nlohmann::json;

//Data Structurs used (struct (PCB) , struct (node) , class (linked list) , class (queue) , class (circular queue).
struct PCB
{
    string id;
    int priority;
    float cpuBurstTime;
    float originalCpuBurstTime;
    float arrivalTime;
    int timeSlice;
    float remainingBurstTime;
    float startTime;
    float completionTime;
    float turnaroundTime;  
    float waitingTime;      

};
struct node
{
    PCB data;
    node* next = nullptr;
};
class linkedList
{
private:
    node* head;
    node* tail;
    int length;

public:
    linkedList()
    {
        head = nullptr;
        tail = nullptr;
        length = 0;
    }

    void addNode(PCB n)
    {
        node* newNode = new node;
        newNode->data = n;
        newNode->next = nullptr;

        if (length == 0)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            if (n.arrivalTime < head->data.arrivalTime)
            {
                newNode->next = head;
                head = newNode;
            }
            else
            {
                node* temp = head;
                while (temp->next != nullptr && temp->next->data.arrivalTime < n.arrivalTime)
                {
                    temp = temp->next;
                }

                newNode->next = temp->next;
                temp->next = newNode;

                if (newNode->next == nullptr)
                {
                    tail = newNode;
                }
            }
        }

        length++;
    }
    void addNodeWithoutOrder(PCB n)
    {
        node* newNode = new node;
        newNode->data = n;
        newNode->next = nullptr;

        if (length == 0)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }

        length++;
    }


    void deleteNode()
    {
        if (length == 0)
        {
            std::cout << "Already empty!" << endl;
            return;
        }
        else if (length == 1)
        {
            delete head;
            head = nullptr;
            tail = nullptr;
        }
        else
        {
            node* temp1 = head;
            while (temp1->next != tail)
            {
                temp1 = temp1->next;
            }
            node* temp2 = tail;
            tail = temp1;
            tail->next = nullptr;
            delete temp2;
        }
        length--;
    }
    node* deleteFromBegin() {
        if (length == 0) {
            std::cout << "List is already empty!" << std::endl;
            return nullptr;
        }

        node* temp = head;
        head = head->next;


        if (head == nullptr) {
            tail = nullptr;
        }

        length--;
        return temp;
    }

    int getLength()
    {
        return length;
    }

    void printLinkedList()
    {
        node* a = head;
        while (a != nullptr)
        {
            std::cout << "ID: " << a->data.id
                << " Priority: " << a->data.priority
                << ", CPU Burst Time: " << a->data.cpuBurstTime
                << ", Arrival Time: " << a->data.arrivalTime
                << ", Time Slice: " << a->data.timeSlice << endl;
            a = a->next;
        }


    }

};

class queue
{
private:
    linkedList readyQueue;

public:
    void push(PCB process)
    {
        readyQueue.addNode(process);

    }
    void pushWithoutOrder(PCB process)
    {
        readyQueue.addNodeWithoutOrder(process);
    }

    PCB pull()
    {
        if (readyQueue.getLength() == 0)
        {
            std::cout << "it's empty";
        }
        else
        {
            return readyQueue.deleteFromBegin()->data;
        }
    }
    int numberOfProcesses()
    {
        return readyQueue.getLength();
    }

    void printQueue()
    {
        readyQueue.printLinkedList();
    }
    bool isEmpty()
    {
        int x = readyQueue.getLength();
        if (x == 0)
            return true;
        else return false;
    }

};
class circularQueue {

private:
    int length = 0;
    PCB myArray[10];
    int head = 0;
    int tail = 0;
public:
    bool isEmpty()
    {
        if (length == 0)
        {
            return true;
        }
        else return false;
    }
    bool isFull()
    {
        if (length == 10)
        {
            return true;
        }
        else return false;
    }
    void push(PCB process)
    {
        if (length == 0)
        {
            myArray[tail] = process;
            tail = 0;
            length++;
        }
        else if (!isFull())
        {
            tail = (tail + 1) % 10;
            myArray[tail] = process;
            length++;
        }

        else return;
    }
    PCB pull()
    {
        PCB pulledProcess;
        if (isEmpty())
        {
            cout << "it's empty \n";

        }
        if (length == 1)
        {
            pulledProcess = myArray[head];

            head = tail = 0;
            length = 0;


        }
        else
        {
            pulledProcess = myArray[head];

            head = (head + 1) % 10;
            length--;
        }
        return pulledProcess;
    }


    int getLength()
    {
        return length;
    }
};
//to read from json file 
json readJsonFile(const string& filePath)
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        cerr << "Could not open the file!" << endl;
        return json();
    }

    json jsonData;
    file >> jsonData;

    return jsonData;
};

//CPU scheduling algorithm.

//First come first serve algorithm 
void FCFS(const json& processesData, queue& readyQueue)
{
    for (const auto& process : processesData)
    {
        PCB pcb;
        pcb.id = process.value("id", "\t");
        pcb.priority = process.value("priority", 0);
        pcb.cpuBurstTime = process.value("cpuBurstTime", 0.0f);
        pcb.arrivalTime = process.value("arrivalTime", 0.0f);
        pcb.timeSlice = process.value("timeSlice", 0);
        pcb.turnaroundTime = 0.0f; 
        pcb.waitingTime = 0.0f;    
        readyQueue.push(pcb);
    }

    cout << "Gantt chart:\n";

    float terminationTime = 0.0f;
    float totalTurnaroundTime = 0.0f;
    float totalWaitingTime = 0.0f;
    float cpuBusyTime = 0.0f; 
    float startTime = FLT_MAX;  
    float endTime = 0.0f;     
    int numberOfProcesses = readyQueue.numberOfProcesses();

    if (numberOfProcesses == 0)
    {
        cout << "The ready queue is empty!\n";
        return;
    }

    for (const auto& process : processesData)
    {
        float arrivalTime = process.value("arrivalTime", 0.0f);
        if (arrivalTime < startTime)
        {
            startTime = arrivalTime;
        }
    }

    // Combleted processes
    vector<PCB> completedProcessList;

    while (!readyQueue.isEmpty())
    {
        PCB temp = readyQueue.pull();
        if (temp.cpuBurstTime != 0)
        {
            if (terminationTime < temp.arrivalTime)
                terminationTime = temp.arrivalTime; 

            float waitingTime = terminationTime - temp.arrivalTime;
            float turnaroundTime = waitingTime + temp.cpuBurstTime;

            totalWaitingTime += waitingTime;
            totalTurnaroundTime += turnaroundTime;
   cout << temp.id << " from: " << terminationTime
                << " to: " << terminationTime + temp.cpuBurstTime << std::endl;
cout << "   Waiting Time: " << waitingTime
                << ", Turnaround Time: " << turnaroundTime << std::endl;

            cpuBusyTime += temp.cpuBurstTime; 
            terminationTime += temp.cpuBurstTime;

            temp.waitingTime = waitingTime;
            temp.turnaroundTime = turnaroundTime;
            completedProcessList.push_back(temp);
        }
    }

    endTime = terminationTime;
    float totalTime = endTime - startTime;
  

   cout << "\nProcess\tWaiting Time\tTurnaround Time\n";
    for (const auto& process : completedProcessList)
    {
  cout << process.id << "\t" << process.waitingTime << "\t\t" << process.turnaroundTime << std::endl;
    }
 cout << "\nAverage Waiting Time: " << totalWaitingTime / numberOfProcesses << std::endl;
cout << "Average Turnaround Time: " << totalTurnaroundTime / numberOfProcesses << std::endl;

    float cpuUtilization = (cpuBusyTime / totalTime) * 100;
   cout << "CPU Utilization: " << cpuUtilization << "%\n";
}




//Shortest Remaing algorithm
void SRT(const json& processesData, queue& readyQueue)
{
    for (const auto& process : processesData)
    {
        PCB pcb;
        pcb.id = process.value("id", "\t");
        pcb.priority = process.value("priority", 0);
        pcb.cpuBurstTime = process.value("cpuBurstTime", 0.0f);
        pcb.originalCpuBurstTime = pcb.cpuBurstTime; 
        pcb.arrivalTime = process.value("arrivalTime", 0.0f);
        readyQueue.pushWithoutOrder(pcb);
    }

    cout << "Gantt Chart:\n";

    float currentTime = 0.0;
    float totalWaitingTime = 0.0;
    float totalTurnaroundTime = 0.0;
    float cpuBusyTime = 0.0;
    int completedProcesses = 0;
    int numberOfProcesses = readyQueue.numberOfProcesses();

    // Cromleted proceses
    vector<PCB> completedProcessList;

    while (completedProcesses < numberOfProcesses)
    {
        PCB shortestProcess;
        bool found = false;

        int queueLength = readyQueue.numberOfProcesses();
        for (int i = 0; i < queueLength; ++i)
        {
            PCB temp = readyQueue.pull();

            if (temp.arrivalTime <= currentTime)
            {
                if (!found || temp.cpuBurstTime < shortestProcess.cpuBurstTime)
                {
                    if (found)
                        readyQueue.pushWithoutOrder(shortestProcess);

                    shortestProcess = temp;
                    found = true;
                }
                else
                {
                    readyQueue.pushWithoutOrder(temp);
                }
            }
            else
            {
                readyQueue.pushWithoutOrder(temp);
            }
        }

        if (!found)
        {
            currentTime++;
            continue;
        }

        cout << shortestProcess.id << " runs from " << currentTime << " to " << (currentTime + 1) << endl;
        shortestProcess.cpuBurstTime -= 1;
        currentTime += 1;
        cpuBusyTime += 1;

        if (shortestProcess.cpuBurstTime == 0)
        {
            completedProcesses++;
            float turnaroundTime = currentTime - shortestProcess.arrivalTime;
            float waitingTime = turnaroundTime - shortestProcess.originalCpuBurstTime;

            shortestProcess.turnaroundTime = turnaroundTime;
            shortestProcess.waitingTime = waitingTime;
            completedProcessList.push_back(shortestProcess);

            totalTurnaroundTime += turnaroundTime;
            totalWaitingTime += waitingTime;
        }
        else
        {
            readyQueue.pushWithoutOrder(shortestProcess);
        }
    }

    float totalTime = currentTime;
    float cpuUtilization = (cpuBusyTime / totalTime) * 100;

   

    cout << "\nProcess\tWaiting Time\tTurnaround Time\n";
    for (const auto& process : completedProcessList)
    {
        cout << process.id << "\t" << process.waitingTime << "\t\t" << process.turnaroundTime << endl;
    }
    cout << "\nCPU Utilization: " << cpuUtilization << "%";
    cout << "\nAverage Turnaround Time: " << totalTurnaroundTime / numberOfProcesses << endl;
    cout << "Average Waiting Time: " << totalWaitingTime / numberOfProcesses << endl;
}


//Round Robin Algorithm
void RR(const json& processesData, circularQueue& readyQueue)
{
    float cpuBusyTime = 0.0f;

   std:: cout << "Gantt Chart\n";

    vector<PCB> processStats;
    for (const auto& process : processesData)
    {
        PCB pcb;
        pcb.id = process.value("id", "");
        pcb.priority = process.value("priority", 0);
        pcb.cpuBurstTime = process.value("cpuBurstTime", 0.0f);
        pcb.arrivalTime = process.value("arrivalTime", 0.0f);
        pcb.timeSlice = process.value("timeSlice", 0);
        pcb.remainingBurstTime = pcb.cpuBurstTime;
        pcb.startTime = -1;
        pcb.completionTime = 0;
        readyQueue.push(pcb);
        processStats.push_back(pcb);
    }

    bool flag = true;
    int finishedProcesses = 0;
    int length = readyQueue.getLength();
    float currentTime = 0.0f;

    while (flag)
    {
        PCB temp = readyQueue.pull();

        if (currentTime < temp.arrivalTime)
        {
            cout << "CPU idle from " << currentTime << " to " << temp.arrivalTime << endl;
            currentTime = temp.arrivalTime;
        }

        if (temp.startTime == -1)
            temp.startTime = currentTime;

        if (temp.remainingBurstTime >= temp.timeSlice)
        {
           std:: cout << temp.id << " from " << currentTime << " to " << currentTime + temp.timeSlice << endl;
            temp.remainingBurstTime -= temp.timeSlice;
            currentTime += temp.timeSlice;
            cpuBusyTime += temp.timeSlice;
        }
        else if (temp.remainingBurstTime > 0)
        {
          std::  cout << temp.id << " from " << currentTime << " to " << currentTime + temp.remainingBurstTime << endl;
            currentTime += temp.remainingBurstTime;
            cpuBusyTime += temp.remainingBurstTime;
            temp.remainingBurstTime = 0;
        }

        if (temp.remainingBurstTime == 0)
        {
            finishedProcesses++;
            temp.completionTime = currentTime;
        }
        else
        {
            readyQueue.push(temp); 
        }

        if (finishedProcesses == length)
            flag = false;

        for (auto& stat : processStats)
        {
            if (stat.id == temp.id)
            {
                stat = temp; 
                break;
            }
        }
    }

    float totalTurnaroundTime = 0.0f;
    float totalWaitingTime = 0.0f;

   std:: cout << "\nProcess Stats:\n";
    cout << "ID\tTurnaround Time\tWaiting Time\n";
    for (const auto& process : processStats)
    {
        float turnaroundTime = process.completionTime - process.arrivalTime;
        float waitingTime = turnaroundTime - process.cpuBurstTime;
        totalTurnaroundTime += turnaroundTime;
        totalWaitingTime += waitingTime;

        cout << process.id << "\t" << turnaroundTime << "\t\t" << waitingTime << endl;
    }

    float totalTime = currentTime;

    float cpuUtilization = (cpuBusyTime / totalTime) * 100;
    cout << "\nCPU Utilization: " << cpuUtilization << "%";
    cout << "\nAverage Turnaround Time: " << totalTurnaroundTime / length << endl;
    cout << "Average Waiting Time: " << totalWaitingTime / length << endl;
}



int main()
{
    json data = readJsonFile("process.json");
    if (data.is_null())
    {
        cerr << "Failed to load JSON data." << endl;
        return 1;
    }

    queue readyQueue;
    circularQueue readyQueue1;

   cout << "\nFirst-Come-First-Serve\n";
   FCFS(data,readyQueue);
   cout<< "\nShortest-Job-First\n";
   SRT(data, readyQueue);
   cout << "\nRound-Robin\n";
   RR(data, readyQueue1);

    return 0;
}

