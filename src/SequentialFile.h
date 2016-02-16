#ifndef SOP_LINUX_PROJECT_FILE_H
#define SOP_LINUX_PROJECT_FILE_H

using namespace std;

class SequentialFile {
protected:
    int descriptor = -1;
    int access = -1; // 00 - RDONLY, 01 - WRONLY, 02 - RDWR
    string filename;
    string filepath;
    char buffer[500];

    string getFileName(string path);

public:
    int openFile(const char* path, int flag = 00);  // O_RDONLY

    int closeFile(int fd = -1);

    bool verifyFile(char* filename);

    bool verifyFile();

    virtual int createFile(const char* path, mode_t mode = 0600);

    int readBytes(size_t count, int fd = -1, char* buffer = nullptr);

    int writeText(string text, int fd = -1);

    bool hasValidDescriptor();

    int deleteFile(const char* path);
};


#endif //SOP_LINUX_PROJECT_FILE_H
