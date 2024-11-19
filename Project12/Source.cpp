#include <iostream>
#include <json.hpp>
#include <cfloat> 
#include<string>
#include <fstream>
using namespace std;
using json = nlohmann::json;

struct PCB
{
    string id;
    int priority;
    float cpuBurstTime;
    float originalCpuBurstTime;
    float arrivalTime;
    int timeSlice;
};

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
            cout << "Already empty!" << endl;
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
            cout << "ID: " << a->data.id
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
            cout << "it's empty";  
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
    PCB arrar[10];








};
void FCFS(const json& processesData, queue& readyQueue)
{
    for (const auto& process : processesData)
    {
        PCB pcb;
        pcb.id = process.value("id","\t");
        pcb.priority = process.value("priority", 0);
        pcb.cpuBurstTime = process.value("cpuBurstTime", 0.0f);
        pcb.arrivalTime = process.value("arrivalTime", 0.0f);
        pcb.timeSlice = process.value("timeSlice", 0);
        readyQueue.push(pcb);
    }
    cout << "Gantt chart : \n";
    float termiation = 0.0f;
    float turnArround = 0.0f;
    float waitingTime = 0.0f;
    int numberOfProcesses = readyQueue.numberOfProcesses();
    if (readyQueue.numberOfProcesses() == 0)
    {
        cout << "empty!";
    }
    else {
        PCB temp = readyQueue.pull();
        termiation = temp.arrivalTime + temp.cpuBurstTime;

        cout << temp.id << " " << "from : " << temp.arrivalTime << " to : " << temp.cpuBurstTime<<endl;
        waitingTime += 0;
        turnArround += temp.cpuBurstTime;
        if (readyQueue.numberOfProcesses()!=0)
        while (!readyQueue.isEmpty())
        {
            PCB temp = readyQueue.pull();
            
            cout << temp.id << " " << "from : " << termiation;
            waitingTime += termiation - temp.arrivalTime;
            termiation += temp.cpuBurstTime;
            turnArround += termiation - temp.arrivalTime;

            cout << " to : " << termiation << endl;

        }
        cout << "The Avarrage waiting time " << waitingTime / numberOfProcesses << endl;
        cout << "The Avarage turnarround time " << turnArround / numberOfProcesses << endl;
    }
}

void SRT(const json& processesData, queue& readyQueue)
{
    for (const auto& process : processesData)
    {
        PCB pcb;
        pcb.id = process.value("id", "");
        pcb.priority = process.value("priority", 0);
        pcb.cpuBurstTime = process.value("cpuBurstTime", 0.0f);
        pcb.originalCpuBurstTime = pcb.cpuBurstTime; 
        pcb.arrivalTime = process.value("arrivalTime", 0.0f);
        readyQueue.push(pcb);
    }

    cout << "Gantt Chart: " << endl;

    float currentTime = 0.0;
    float totalWaitingTime = 0.0;
    float totalTurnaroundTime = 0.0;
    int completedProcesses = 0;
    int numberOfProcesses = readyQueue.numberOfProcesses();

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

        if (shortestProcess.cpuBurstTime > 0)
        {
            readyQueue.pushWithoutOrder(shortestProcess);
        }
        else
        {
            completedProcesses++;
            float turnaroundTime = currentTime - shortestProcess.arrivalTime;
            float waitingTime = turnaroundTime - shortestProcess.originalCpuBurstTime;

            totalTurnaroundTime += turnaroundTime;
            totalWaitingTime += waitingTime;
        }
    }

    cout << "Average Turnaround Time: " << totalTurnaroundTime / numberOfProcesses << endl;
    cout << "Average Waiting Time: " << totalWaitingTime / numberOfProcesses << endl;
}



void RR(const json& processesData, queue& readyQueue)
{
    
    for (const auto& process : processesData)
    {
        PCB pcb;
        pcb.priority = process.value("priority", 0);
        pcb.cpuBurstTime = process.value("cpuBurstTime", 0.0f);
        pcb.arrivalTime = process.value("arrivalTime", 0.0f);
        pcb.timeSlice = process.value("timeSlice", 0);
        readyQueue.push(pcb);
    } 



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

    SRT(data, readyQueue);

    return 0;
}

