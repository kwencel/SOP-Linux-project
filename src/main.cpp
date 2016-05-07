#include <iostream>
#include "SequentialFile.h"
#include "SeekableFile.h"
#include "Process.h"
#include "SharedMemory.h"
#include "Semaphore.h"
#include "MessageQueue.h"

using namespace std;

int main() {

    SequentialFile sequentialFile;
    SeekableFile seekableFile;
    Process process;
    SharedMemory sharedMemory;
    Semaphore semaphore;
    MessageQueue messageQueue;

    char input;
    bool goback = false;

    while (true) {
        cout << endl;
        cout << "1: File operations" << endl;
        cout << "2: Process operations" << endl;
        cout << "3: IPC operations" << endl;
        cout << "q: Quit" << endl;
        cin >> input;
        switch (input) {
            case '1': { // FILE OPERATIONS
                goback = false;
                while (!goback) {
                    cout << endl;
                    cout << "1: Sequential access" << endl;
                    cout << "2: Direct access" << endl;
                    cout << "b: Go back to previous menu" << endl;
                    cout << "q: Quit" << endl;
                    cin >> input;
                    if (input == '1' or input == '2') {
                        bool goback = false;
                        bool seekable = (bool) (input - '0' - 1);
                        SequentialFile* fileClassPtr = &sequentialFile;
                        while (!goback) {
                            cout << endl;
                            cout << "1: Create file" << endl;
                            cout << "2: Delete file" << endl;
                            cout << "3: Open file" << endl;
                            cout << "4: Close file" << endl;
                            cout << "5: Read from file" << endl;
                            cout << "6: Write to file" << endl;
                            if (seekable) {
                                cout << "7: Seek file" << endl;
                                fileClassPtr = &seekableFile;
                            }
                            cout << "b: Go back to previous menu" << endl;
                            cout << "q: Quit" << endl;
                            cin >> input;
                            switch (input) {
                                case '1': {
                                    cout << endl;
                                    cout << "Please enter the path to new file: " << endl;
                                    string filepath;
                                    cin >> filepath;
                                    fileClassPtr->createFile(filepath.c_str());
                                    break;
                                }
                                case '2': {
                                    cout << endl;
                                    cout << "Please enter the path to file you want to delete: " << endl;
                                    string filepath;
                                    cin >> filepath;
                                    fileClassPtr->deleteFile(filepath.c_str());
                                    break;
                                }
                                case '3': {
                                    cout << endl;
                                    cout << "Please enter the path to file: " << endl;
                                    string filepath;
                                    cin >> filepath;
                                    int mode = -1;
                                    while (mode == -1) {
                                        cout << endl;
                                        cout << "File Access mode: " << endl;
                                        cout << "1: Read-only " << endl;
                                        cout << "2: Write-only: " << endl;
                                        cout << "3: Read and write: " << endl;
                                        cin >> input;
                                        switch (input) {
                                            case '1':
                                                mode = 00;
                                                break;
                                            case '2':
                                                mode = 01;
                                                break;
                                            case '3':
                                                mode = 02;
                                                break;
                                        }
                                    }
                                    fileClassPtr->openFile(filepath.c_str(), mode);
                                    break;
                                }
                                case '4': {
                                    fileClassPtr->closeFile();
                                    break;
                                }
                                case '5': {
                                    if (!fileClassPtr->hasValidDescriptor()) {
                                        cerr << "Open the file first!" << endl;
                                        break;
                                    }
                                    cout << endl;
                                    cout << "Please enter the number of bytes you want to read from file:" << endl;
                                    size_t bytes;
                                    cin >> bytes;
                                    fileClassPtr->readBytes(bytes);
                                    break;
                                }
                                case '6': {
                                    if (!fileClassPtr->hasValidDescriptor()) {
                                        cerr << "Open the file first!" << endl;
                                        break;
                                    }
                                    cout << endl;
                                    cout << "Please enter the text you want to write to file:" << endl;
                                    string text;
                                    cin.ignore();
                                    getline(cin, text);
                                    fileClassPtr->writeText(text);
                                    break;
                                }
                                case '7': {
                                    if (seekable) {
                                        cout << endl;
                                        cout << "Please enter reference point in the file to move the offset pointer" << endl;
                                        cout << "1: Beginning of file (SEEK_SET)" << endl;
                                        cout << "2: Current position (SEEK_CUR)" << endl;
                                        cout << "3: End of file (SEEK_END)" << endl;
                                        int whence;
                                        cin >> input;
                                        switch (input) {
                                            case '1': {
                                                whence = SEEK_SET;
                                                break;
                                            }
                                            case '2': {
                                                whence = SEEK_CUR;
                                                break;
                                            }
                                            case '3': {
                                                whence = SEEK_END;
                                                break;
                                            }
                                        }
                                        cout << "Please enter the offset you want to move from the reference point" << endl;
                                        off_t offset;
                                        cin >> offset;
                                        seekableFile.seek(offset, whence);
                                    }
                                    break;
                                }
                                case 'b': {
                                    goback = true;
                                    break;
                                }
                                case 'q': {
                                    exit(0);
                                }
                            }
                        }
                    }
                    else if (input == 'b') {
                        goback = true;
                    }
                    else if (input == 'q') {
                        exit(0);
                    }
                }
                break;
            }
            case '2': { // PROCESS OPERATIONS
                goback = false;
                while (!goback) {
                    cout << endl;
                    cout << "1: Attach to process" << endl;
                    cout << "2: Create a child process" << endl;
                    cout << "3: Send signal to process" << endl;
                    cout << "4: Change process priority" << endl;
                    cout << "5: Wait for process" << endl;
                    cout << "6: List all your running processes" << endl;
                    cout << "7: Show PID of currently assigned process" << endl;
                    cout << "b: Go back to previous menu" << endl;
                    cout << "q: Quit" << endl;
                    cin >> input;
                    switch (input) {
                        case '1': {
                            if (process.isChild() && process.verifyPid() && !process.isZombie()) {
                                RETRY1:
                                cout << endl;
                                cout << "Your previous child process needs to end before proceeding." << endl;
                                cout << "Do you want to end it now?" << endl;
                                cout << "1: No action" << endl;
                                cout << "2: Send SIGTERM" << endl;
                                cout << "3: Send SIGKILL" << endl;
                                cin >> input;
                                if (input == '1') {
                                    break;
                                } else if (input == '2') {
                                    process.sendSignal(15);
                                } else if (input == '3') {
                                    process.sendSignal(9);
                                } else {
                                    goto RETRY1;
                                }
                            }
                            cout << endl;
                            cout << "Please enter PID of the process you want to attach to:" << endl;
                            pid_t pid;
                            cin >> pid;
                            process.attach(pid);
                            break;
                        }
                        case '2': {
                            if (process.isChild() && process.verifyPid() && !process.isZombie()) {
                                RETRY2:
                                cout << endl;
                                cout << "Your previous child process needs to end before proceeding." << endl;
                                cout << "Do you want to end it now?" << endl;
                                cout << "1: No action" << endl;
                                cout << "2: Send SIGTERM" << endl;
                                cout << "3: Send SIGKILL" << endl;
                                cin >> input;
                                cin.clear();
                                if (input == '1') {
                                    break;
                                } else if (input == '2') {
                                    process.sendSignal(15);
                                } else if (input == '3') {
                                    process.sendSignal(9);
                                } else {
                                    goto RETRY2;
                                }
                            }
                            cout << endl;
                            cout << "Please enter the name or a full path to executable to run:" << endl;
                            string command;
                            cin.ignore();
                            getline(cin, command);
                            process.runExecutable(command);
                            break;
                        }
                        case '3': {
                            if (!process.verifyPid()) {
                                cerr << "Please attach a process first!" << endl;
                                break;
                            }
                            cout << endl;
                            cout << " 1: SIGHUP\t 2: SIGINT\t 3: SIGQUIT\t 4: SIGILL" << endl;
                            cout << " 6: SIGABRT\t 8: SIGFPE\t 9: SIGKILL\t11: SIGSEGV" << endl;
                            cout << "13: SIGPIPE\t14: SIGALRM\t15: SIGTERM\t16: SIGUSR1" << endl;
                            cout << "17: SIGUSR2\t18: SIGCHLD\t25: SIGCONT\t23: SIGSTOP" << endl;
                            cout << "24: SIGTSTP\t26: SIGTTIN\t27: SIGTTOU" << endl;
                            int signal = -1;
                            cin >> signal;
                            switch (signal) {
                                case 1:
                                case 2:
                                case 3:
                                case 4:
                                case 6:
                                case 8:
                                case 9:
                                case 11:
                                case 13:
                                case 14:
                                case 15:
                                case 16:
                                case 17:
                                case 18:
                                case 25:
                                case 23:
                                case 24:
                                case 26:
                                case 27: {
                                    process.sendSignal(signal);
                                    break;
                                }
                                default: {
                                    cerr << "Please enter the valid signal number!" << endl;
                                    break;
                                }
                            }
                            break;
                        }
                        case '4': {
                            if (!process.verifyPid()) {
                                cerr << "Please attach a process first!" << endl;
                                break;
                            }
                            cout << endl;
                            cout << "Please enter the priority (integer number between [-20,19])" << endl;
                            cout << "You can only lower priority of your processes unless you are superuser" << endl;
                            int priority;
                            cin >> priority;
                            process.changePriority(priority);
                            break;
                        }
                        case '5': {
                            if (!process.verifyPid()) {
                                cerr << "Please attach a process first!" << endl;
                                break;
                            }
                            int retval = process.waitFor();
                            cout << "Value returned by the process was: " << retval << endl;
                            break;
                        }
                        case '6': {
                            cout << endl;
                            process.listProcesses();
                            cout << endl;
                            break;
                        }
                        case '7': {
                            pid_t pid = process.getPid();
                            if (pid == -1) {
                                cout << "No process assigned." << endl;
                            } else {
                                cout << "Currently assigned to process with PID " << pid;
                                if (!process.verifyPid()) {
                                    cout << " (dead)";
                                } else if (process.isZombie()) {
                                    cout << " (zombie)";
                                }
                                cout << endl;
                            }
                            break;
                        }
                        case 'b': {
                            goback = true;
                            break;
                        }
                        case 'q': {
                            exit(0);
                        }
                    }
                }
                break;
            }
            case '3': {
                goback = false;
                while (!goback) {
                    cout << endl;
                    cout << "1: Shared memory" << endl;
                    cout << "2: Semaphores" << endl;
                    cout << "3: Message queues" << endl;
                    cout << "b: Go back to previous menu" << endl;
                    cout << "q: Quit" << endl;
                    cin >> input;
                    switch (input) {
                        case '1': { // SHARED MEMORY
                            bool goback = false;
                            while (!goback) {
                                cout << endl;
                                cout << "1: Create shared memory segment" << endl;
                                cout << "2: Delete shared memory segment" << endl;
                                cout << "3: Attach shared memory segment" << endl;
                                cout << "4: Detach shared memory segment" << endl;
                                cout << "5: Read from shared memory segment" << endl;
                                cout << "6: Write to shared memory segment" << endl;
                                cout << "7: List all created and attached segments" << endl;
                                cout << "b: Go back to previous menu" << endl;
                                cout << "q: Quit" << endl;
                                cin >> input;
                                switch (input) {
                                    case '1': {
                                        cout << endl;
                                        cout << "Please enter how many chars (bytes) should the segment store:" << endl;
                                        size_t size;
                                        cin >> size;
                                        sharedMemory.create(size);
                                        break;
                                    }
                                    case '2': {
                                        cout << endl;
                                        sharedMemory.remove();
                                        break;
                                    }
                                    case '3': {
                                        cout << endl;
                                        sharedMemory.attach();
                                        break;
                                    }
                                    case '4': {
                                        cout << endl;
                                        sharedMemory.detach();
                                        break;
                                    }
                                    case '5': {
                                        cout << endl;
                                        sharedMemory.read();
                                        break;
                                    }
                                    case '6': {
                                        cout << endl;
                                        cout << "Please enter the text to put into the shared memory segment" << endl;
                                        string text;
                                        cin >> text;
                                        sharedMemory.write(text);
                                        break;
                                    }
                                    case '7': {
                                        cout << endl;
                                        sharedMemory.viewSegmentList();
                                        break;
                                    }
                                    case 'b': {
                                        goback = true;
                                        break;
                                    }
                                    case 'q': {
                                        exit(0);
                                    }
                                }
                            }
                            break;
                        }
                        case '2': {
                            bool goback = false;
                            while (!goback) {
                                cout << endl;
                                cout << "1: Create a semaphore set" << endl;
                                cout << "2: Attach a previously created semaphore set" << endl;
                                cout << "3: Remove a semaphore set" << endl;
                                cout << "4: Perform P() (lock) operation" << endl;
                                cout << "5: Perform V() (unlock) operation" << endl;
                                cout << "6: Set value of a semaphore" << endl;
                                cout << "7: Show ID of the semaphore set" << endl;
                                cout << "8. Show number of semaphores in set" << endl;
                                cout << "b: Go back to previous menu" << endl;
                                cout << "q: Quit" << endl;
                                cin >> input;
                                switch (input) {
                                    case '1': {
                                        cout << endl;
                                        cout << "Please enter the number of semaphores to create:" << endl;
                                        int amount;
                                        cin >> amount;
                                        semaphore.create(amount);
                                        break;
                                    }
                                    case '2': {
                                        cout << endl;
                                        cout << "Please enter the ID of the semaphore set you want to attach:" << endl;
                                        int id;
                                        cin >> id;
                                        cout << "Please enter the number of semaphores in the set" << endl;
                                        int amount;
                                        cin >> amount;
                                        semaphore.attach(id, amount);
                                        break;
                                    }
                                    case '3': {
                                        cout << endl;
                                        semaphore.remove();
                                        break;
                                    }
                                    case '4': {
                                        cout << endl;
                                        cout << "Please enter which semaphore do you want to operate on (0-" <<
                                        semaphore.getAmount() - 1 << "):" << endl;
                                        unsigned short which;
                                        cin >> which;
                                        cout << "Please enter how much do you want to decrement semaphore's value:" << endl;
                                        short amount;
                                        cin >> amount;
                                        semaphore.P(which, amount);
                                        break;
                                    }
                                    case '5': {
                                        cout << endl;
                                        cout << "Please enter which semaphore do you want to operate on (0-" <<
                                        semaphore.getAmount() - 1 << "):" << endl;
                                        unsigned short which;
                                        cin >> which;
                                        cout << "Please enter how much do you want to increment semaphore's value:" << endl;
                                        short amount;
                                        cin >> amount;
                                        semaphore.V(which, amount);
                                        break;
                                    }
                                    case '6': {
                                        cout << endl;
                                        cout << "Please enter which semaphore do you want to operate on (0-" <<
                                        semaphore.getAmount() - 1 << "):" << endl;
                                        unsigned short which;
                                        cin >> which;
                                        cout << "Please enter the new semaphore's value:" << endl;
                                        short amount;
                                        cin >> amount;
                                        semaphore.set(which, amount);
                                        break;
                                    }
                                    case '7': {
                                        cout << endl;
                                        cout << "Semaphore set ID: " << semaphore.getID() << endl;
                                        break;
                                    }
                                    case '8': {
                                        cout << endl;
                                        cout << "Semaphores amount: " << semaphore.getAmount() << endl;
                                        break;
                                    }
                                    case 'b': {
                                        goback = true;
                                        break;
                                    }
                                    case 'q': {
                                        exit(0);
                                    }
                                }
                            }
                            break;
                        }
                        case '3': {
                            bool goback = false;
                            while (!goback) {
                                cout << endl;
                                cout << "1: Create a message queue" << endl;
                                cout << "2: Attach a previously created message queue" << endl;
                                cout << "3: Remove a message queue" << endl;
                                cout << "4: Receive a message" << endl;
                                cout << "5: Send a message" << endl;
                                cout << "6: Get ID of the message queue" << endl;
                                cout << "b: Go back to previous menu" << endl;
                                cout << "q: Quit" << endl;
                                cin >> input;
                                switch (input) {
                                    case '1': {
                                        cout << endl;
                                        messageQueue.create();
                                        break;
                                    }
                                    case '2': {
                                        cout << endl;
                                        cout << "Please enter the ID of the message queue you want to attach:" << endl;
                                        int id;
                                        cin >> id;
                                        messageQueue.attach(id);
                                        break;
                                    }
                                    case '3': {
                                        cout << endl;
                                        messageQueue.remove();
                                        break;
                                    }
                                    case '4': {
                                        cout << endl;
                                        if (messageQueue.getID() == -1) {
                                            cerr << "Create or attach a message queue first!" << endl;
                                            break;
                                        }
                                        cout << "Please enter the message type (integer value >0):" << endl;
                                        long type;
                                        cin >> type;
                                        messageQueue.receive(type);
                                        break;
                                    }
                                    case '5': {
                                        cout << endl;
                                        if (messageQueue.getID() == -1) {
                                            cerr << "Create or attach a message queue first!" << endl;
                                            break;
                                        }
                                        cout << "Please enter the message type (integer value >0):" << endl;
                                        long type;
                                        cin >> type;
                                        cout << "Please enter the message to send via the message queue:" << endl;
                                        string text;
                                        cin.ignore();
                                        getline(cin, text);
                                        messageQueue.send(text, type);
                                        break;
                                    }
                                    case '6': {
                                        int id = messageQueue.getID();
                                        if (id == -1) {
                                            cerr << "Create or attach a message queue first!" << endl;
                                            break;
                                        }
                                        cout << "Message queue ID: " << id << endl;
                                        break;
                                    }
                                    case 'b': {
                                        goback = true;
                                        break;
                                    }
                                    case 'q': {
                                        exit(0);
                                    }
                                }
                            }
                            break;
                        }
                        case 'b': {
                            goback = true;
                            break;
                        }
                        case 'q': {
                            exit(0);
                        }
                    }
                }
                break;
            }
            case 'q': {
                exit(0);
            }
        }
    }
}