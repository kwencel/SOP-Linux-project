#ifndef SOP_LINUX_PROJECT_SHAREDMEMORY_H
#define SOP_LINUX_PROJECT_SHAREDMEMORY_H

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <vector>
#include <iostream>

using namespace std;

class Segment {
public:
    int id = -1;
    size_t size = 0;
    char* ptr = nullptr;

    bool operator==(const Segment& other) { return this->id == other.id; }

    Segment(int id, size_t size, char* ptr) : id(id), size(size), ptr(ptr) { }
};

class SharedMemory {
private:
    vector<Segment> allSegments;
    vector<Segment> attachedSegments;

    int selectSegmentFromVector(vector<Segment>* vectorPtr, string activity = "", bool extraChoice = false);

public:
    int create(size_t size, int flags = 0600 | IPC_CREAT, key_t key = IPC_PRIVATE);

    char* attach();

    int detach();

    int remove();

    int read();

    int write(string text);

    void viewSegmentList();
};


#endif //SOP_LINUX_PROJECT_SHAREDMEMORY_H
