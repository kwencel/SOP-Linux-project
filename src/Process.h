#ifndef SOP_LINUX_PROJECT_PROCESS_H
#define SOP_LINUX_PROJECT_PROCESS_H

#include <sys/types.h>
#include <iostream>
#include <vector>

using namespace std;

class Process {
private:
    pid_t pid = -1;
    bool hasChild = false;
    vector<char*> argv; // If not empty, a child process is running.

    vector<char*> filterArguments(string command, char delim);

    int getCommandReturnCode(string command, bool hideOutput);

    string getCommandOutput(string command);

    string getMultilineCommandOutput(string command);

    void freeMemoryForArguments();

public:
    int attach(pid_t pid);

    int sendSignal(int signal);

    int changePriority(int priority);

    int waitFor();

    bool verifyPid();

    bool verifyPid(pid_t pid);

    bool isZombie();

    bool isZombie(pid_t pid);

    bool isChild();

    void runExecutable(string command);

    void listProcesses();

    pid_t getPid();
};


#endif //SOP_LINUX_PROJECT_PROCESS_H
