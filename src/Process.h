#ifndef SOP_LINUX_PROJECT_PROCESS_H
#define SOP_LINUX_PROJECT_PROCESS_H

#include <sys/types.h>

class Process {
private:
    pid_t pid = -1;

public:
    int attach(pid_t pid);

    int sendSignal(int signal);

    int changePriority();

    pid_t waitFor();

    bool hasValidPid();
};


#endif //SOP_LINUX_PROJECT_PROCESS_H
