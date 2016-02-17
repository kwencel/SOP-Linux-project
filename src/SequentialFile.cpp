#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "SequentialFile.h"

string SequentialFile::getFileName(string path) {
    size_t i = path.rfind('/', path.length());
    if (i != string::npos) {
        return (path.substr(i + 1, path.length() - i));
    }
    return path;
}

int SequentialFile::openFile(const char* path, int flag) {
    int fd = open(path, flag);
    if (fd != -1) {
        if (descriptor != -1) {
            close(descriptor);
        }
        descriptor = fd;
        access = flag;
        filepath = path;
        filename = getFileName(path);
        cout << "File '" << path << "' opened successfully." << endl;
        return 0;
    } else {
        stringstream errorstream;
        errorstream << "Can't open file '" << getFileName(path) << "'";
        perror(errorstream.str().c_str());
        return -1;
    }
}

int SequentialFile::closeFile(int fd) {
    if (fd == -1) {
        fd = descriptor;
    }
    if (close(fd) == 0) {
        cout << "File closed successfully." << endl;
        if (fd == descriptor) {
            descriptor = -1;
        }
        return 0;
    } else {
        perror("Can't close file");
        return -1;
    }
}

int SequentialFile::createFile(const char* path, mode_t mode) {
    return mkfifo(path, mode);
}

int SequentialFile::readBytes(size_t count, int fd, char* buffer) {
    if (fd == -1) {
        fd = descriptor;
    }
    if (buffer == nullptr) {
        buffer = this->buffer;
    }

    ssize_t read_bytes = read(fd, buffer, count);
    if (read_bytes == -1) {
        perror("Can't read file");
        return -1;
    }

    // Write read bytes to standard output
    cout << "Bytes read: " << read_bytes << endl;
    cout << "Content: ";
    for (int i = 0; i < read_bytes; ++i) {
        cout << buffer[i];
    }
    cout << endl;
    return 0;
}

int SequentialFile::writeText(string text, int fd) {
    if (fd == -1) {
        fd = descriptor;
    }
    ssize_t written_bytes = write(fd, text.c_str(), text.length());
    if (written_bytes == -1) {
        perror("Can't write file");
        return -1;
    }
    cout << "Bytes written: " << written_bytes << endl;
    return 0;
}

bool SequentialFile::hasValidDescriptor() {
    return (descriptor != -1 && verifyFile());
}

int SequentialFile::deleteFile(const char* path) {
    if (unlink(path) == -1) {
        perror("Can't delete file");
        return -1;
    }
    return 0;
}

bool SequentialFile::verifyFile(char* filename) {
    struct stat buffer;
    if (stat(filename, &buffer) != -1) {
        return true;
    }
    return false;
}

bool SequentialFile::verifyFile() {
    return verifyFile((char*) filepath.c_str());
}