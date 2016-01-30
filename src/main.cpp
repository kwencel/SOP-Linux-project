#include <iostream>
#include "SequentialFile.h"
#include "SeekableFile.h"

using namespace std;

int main() {

//    SeekableFile plik;
//    plik.openFile("TextFile.txt", 01);
//    plik.writeBytes("d");
//    plik.closeFile();

    SequentialFile sequentialFile;
    SeekableFile seekableFile;

    char input;

    while (true) {
        cout << endl;
        cout << "1: File operations" << endl;
        cout << "2: Process operations" << endl;
        cout << "3: IPC operations" << endl;
        cout << "q: Quit" << endl;
        cin >> input;
        switch (input) {
            case '1': {
                bool goback = false;
                while (!goback) {
                    cout << endl;
                    cout << "1: Sequential access" << endl;
                    cout << "2: Direct access" << endl;
                    cout << "b: Go back to previous menu" << endl;
                    cin >> input;
                    switch (input) {
                        case '1': {
                            bool goback = false;
                            while (!goback) {
                                cout << endl;
                                cout << "1: Create file" << endl;
                                cout << "2: Open file" << endl;
                                cout << "3: Close file" << endl;
                                cout << "4: Read from file" << endl;
                                cout << "5: Write to file" << endl;
                                cout << "b: Go back to previous menu" << endl;
                                cin >> input;
                                switch (input) {
                                    case '1': {
                                        cout << endl;
                                        cout << "Please enter the path to new file: " << endl;
                                        string filepath;
                                        cin >> filepath;
                                        sequentialFile.createFile(filepath.c_str());
                                        break;
                                    }
                                    case '2': {
                                        cout << endl;
                                        cout << "Please enter the path to file: " << endl;
                                        string filepath;
                                        cin >> filepath;
                                        cout << endl;
                                        cout << "File Access mode: " << endl;
                                        cout << "1: Read-only " << endl;
                                        cout << "2: Write-only: " << endl;
                                        cout << "3: Read and write: " << endl;
                                        cin >> input;
                                        int mode;
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
                                        sequentialFile.openFile(filepath.c_str(), mode);
                                        break;
                                    }
                                    case '3': {
                                        sequentialFile.closeFile();
                                        break;
                                    }
                                    case '4': {
                                        if (!sequentialFile.hasValidDescriptor()) {
                                            cerr << "Open the file first!" << endl;
                                            break;
                                        }
                                        cout << endl;
                                        cout << "Please enter the number of bytes you want to read from file:" << endl;
                                        size_t bytes;
                                        cin >> bytes;
                                        sequentialFile.readBytes(bytes);
                                        break;
                                    }
                                    case '5': {
                                        if (!sequentialFile.hasValidDescriptor()) {
                                            cerr << "Open the file first!" << endl;
                                            break;
                                        }
                                        cout << endl;
                                        cout << "Please enter the text you want to write to file:" << endl;
                                        string text;
                                        cin.ignore();
                                        getline(cin, text);
                                        sequentialFile.writeText(text);
                                        break;
                                    }
                                    case 'b': {
                                        goback = true;
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                        case 'b': {
                            goback = true;
                            break;
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