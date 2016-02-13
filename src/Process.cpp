#include "Process.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

using namespace std;

int Process::attach(pid_t pid) {
    int retval;
    if (fork() == 0) {
        char* argv[3] = {(char*) "-p", (char*) to_string(pid).c_str(), (char*) NULL};
        int devnull = open("/dev/null", O_WRONLY);
        dup2(devnull, 1);
        close(devnull);
        execvp("ps", argv);
        perror("Can't check PID corectness");
    } else {
        cout << "waiting" << endl;
        wait(&retval);
        cout << "ended wait" << endl;
    }
    retval = WEXITSTATUS(retval);
    if (retval == 0) {
        this->pid = pid;
        return 0;
    } else {
        cerr << "There is no such process! Try again." << endl;
        return -1;
    }
}

int Process::sendSignal(int signal) {
    return kill(pid, signal);
}

pid_t Process::waitFor() {
    int retval;
    return waitpid(pid, &retval, NULL);
}

bool Process::hasValidPid() {
    return (pid != -1);
}