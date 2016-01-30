#ifndef SOP_LINUX_PROJECT_FILE_H
#define SOP_LINUX_PROJECT_FILE_H

#include <string>

using namespace std;

class File {
private:
    int handle = -1;
    int access = -1; // 00 - RDONLY, 01 - WRONLY, 02 - RDWR
    string filename;
    string filepath;

    string getFileName(string path);

public:
    int openFile(const char *path, int flag);
};


#endif //SOP_LINUX_PROJECT_FILE_H
