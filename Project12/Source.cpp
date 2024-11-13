#include <iostream>
#include <json.hpp>
#include <fstream>
using namespace std;
using json = nlohmann::json;

struct PCB
{
    int priority;
    float cpuBurstTime;
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

    int getLength()
    {
        return length;
    }

    void printLinkedList()
    {
        node* a = head;
        while (a != nullptr)
        {
            cout << "Priority: " << a->data.priority
                << ", CPU Burst Time: " << a->data.cpuBurstTime
                << ", Arrival Time: " << a->data.arrivalTime
                << ", Time Slice: " << a->data.timeSlice << endl;
            a = a->next;
        }
        cout << endl;
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
        cout << "Process added successfully\n";
    }

    void pull()
    {
        readyQueue.deleteNode();
    }

    int numberOfProcesses()
    {
        return readyQueue.getLength();
    }

    void printQueue()
    {
        readyQueue.printLinkedList();
    }
};

void FCFS(const json& processesData, queue& readyQueue)
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

void SRT(const json& processesData, queue& readyQueue)
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

    cout << "Data loaded from JSON:" << endl;

    queue readyQueue;
    FCFS(data, readyQueue);

    cout << "Processes in the queue:" << endl;
    readyQueue.printQueue();
    return 0;
}