#include <iostream>
#include "SequentialFile.h"
#include "SeekableFile.h"
#include "Process.h"

using namespace std;

int main() {

    SequentialFile sequentialFile;
    SeekableFile seekableFile;
    Process process;

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
            case '1': {
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
                            cout << "2: Open file" << endl;
                            cout << "3: Close file" << endl;
                            cout << "4: Read from file" << endl;
                            cout << "5: Write to file" << endl;
                            if (seekable) {
                                cout << "6: Seek file" << endl;
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
                                case '3': {
                                    fileClassPtr->closeFile();
                                    break;
                                }
                                case '4': {
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
                                case '5': {
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
                                case '6': {
                                    if (seekable) {
                                        cout << endl;
                                        cout << "Please enter reference point in the file to move the offset pointer" <<
                                        endl;
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
                                        cout << "Please enter the offset you want to move from the reference point" <<
                                        endl;
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
                    cout << "1: Attach to process" << endl;
                    cout << "2: Send signal to process" << endl;
                    cout << "3: Change process priority" << endl;
                    cout << "4: Wait for process" << endl;
                    cout << "b: Go back to previous menu" << endl;
                    cout << "q: Quit" << endl;
                    cin >> input;
                    switch (input) {
                        case '1': {
                            pid_t pid;
                            cout << "Please enter PID of the process you want to attach to:" << endl;
                            cin >> pid;
                            process.attach(pid);
                        }
                    }
                }
            }
            case 'q': {
                exit(0);
            }
        }
    }
}