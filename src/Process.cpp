#include "Process.h"
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <fcntl.h>
#include <sstream>
#include <string.h>

int Process::attach(pid_t pid) {
    if (isZombie()) {
        waitpid(this->pid, NULL, 0);
    }
    stringstream psCommand;
    psCommand << "ps -p " << to_string(pid);
    int retval = getCommandReturnCode(psCommand.str(), true);
    verifyPid(pid);
    if (retval == 0) {
        string whoami = getCommandOutput("whoami");
        psCommand.str("");
        psCommand << "ps -p " << pid << " -o user=";
        string pidOwner = getCommandOutput(psCommand.str());
        if (!((whoami.compare("root") == 0) || (whoami.compare(pidOwner) == 0))) {
            cerr << "You can't manipulate processes that belong to " << pidOwner << "!" << endl;
            return 1;
        }
        this->pid = pid;
        hasChild = false;
        return 0;
    } else {
        cerr << "There is no such process running! Try again." << endl;
        return -1;
    }
}

int Process::sendSignal(int signal) {
    return kill(pid, signal);
}

int Process::waitFor() {
    if (!hasChild) {
        cerr << "You can't wait for a process that is not a child of yours." << endl;
        cerr << "Please use the option 'Create a child process' first." << endl;
        return -1;
    }
    int retval;
    waitpid(pid, &retval, 0);
    return retval;
}

int Process::changePriority(int priority) {
    return setpriority(PRIO_PROCESS, (id_t) pid, priority);
}

void Process::runExecutable(string command) {
    if (isZombie()) {
        waitpid(pid, NULL, 0);
    }
    freeMemoryForArguments();
    argv = filterArguments(command, ' ');
    hasChild = true;
    pid = fork();
    if (pid == 0) {
        execvp(argv[0], argv.data());
        perror("Can't run the executable");
        exit(-1);
    }
}

int Process::getCommandReturnCode(string command, bool hideOutput) {
    int retval;
    int pid = fork();
    if (pid == 0) {
        if (hideOutput) {
            int devnull = open("/dev/null", O_WRONLY);
            dup2(devnull, 1);
            dup2(devnull, 2);
            close(devnull);
        }
        vector<char*> argv = filterArguments(command, ' ');
        execvp(argv[0], argv.data());
        perror("Can't run the process and capture the output");
        exit(-1);
    } else {
        waitpid(pid, &retval, 0);
    }
    return WEXITSTATUS(retval);
}

string Process::getCommandOutput(string command) {
    FILE* fp = popen(command.c_str(), "r");
    if (fp == NULL) {
        cerr << "Can't run the command (and capture its output)" << endl;
    }
    char buffer[1024];
    fscanf(fp, "%s\n", &buffer);
    pclose(fp);
    return string(buffer);
}

string Process::getMultilineCommandOutput(string command) {
    FILE* fp = popen(command.c_str(), "r");
    if (fp == NULL) {
        cerr << "Can't run the command (and capture its output)" << endl;
    }
    char buffer[1024];
    stringstream ss;
    while (fgets(buffer, sizeof(buffer) - 1, fp) != NULL) {
        ss << buffer;
    }
    fgets(buffer, sizeof(buffer) - 1, fp) != NULL;
    ss << buffer;
    return ss.str();
}

vector<char*> Process::filterArguments(string command, char delim) {
    stringstream ss(command);
    string argument;
    vector<char*> vecOfArgs;
    while (getline(ss, argument, delim)) {
        char* arg = new char[argument.length() + 1];
        strcpy(arg, argument.c_str());
        vecOfArgs.push_back(arg);
    }
    vecOfArgs.push_back(NULL);
    return vecOfArgs;
}

void Process::freeMemoryForArguments() {
    for (int i = 0; i < argv.size(); ++i) {
        delete[] argv[i];
    }
    argv.clear();
}

bool Process::verifyPid() {
    return verifyPid(pid);
}

bool Process::verifyPid(pid_t pid) {
    stringstream psCommand;
    psCommand << "ps -p " << to_string(pid);
    int retval = getCommandReturnCode(psCommand.str(), true);
    if (retval == 0) {
        return true;
    }
    return false;
}

void Process::listProcesses() {
    char* argv[4] = {(char*) "ps", (char*) "-fu", (char*) getCommandOutput("whoami").c_str(), NULL};
    int pid = fork();
    if (pid == 0) {
        execvp(argv[0], argv);
    } else {
        waitpid(pid, NULL, 0);
    }
}

bool Process::isChild() {
    return hasChild;
}

bool Process::isZombie(pid_t pid) {
    if (pid == -1) {
        return false;
    }
    stringstream ss;
    ss << "ps -p " << pid << " -o stat=";
    string state = getCommandOutput(ss.str());
    if (state[0] == 'Z') {
        return true;
    }
    return false;
}

bool Process::isZombie() {
    return isZombie(pid);
}

pid_t Process::getPid() {
    return pid;
}
