#include <iostream>
#include <sstream>
#include <fcntl.h>
#include "File.h"

using namespace std;

int File::openFile(const char *path, int flag) {
    int tmp_handle = open(path, flag);
    if (tmp_handle > 0) {
        handle = tmp_handle;
        access = flag;
        filepath = path;
        filename = getFileName(path);
        cout << "File '" << path << "' opened successfully.";
        return 0;
    } else {
        stringstream errorstream;
        errorstream << "Can't open file '" << getFileName(path) << "'";
        perror(errorstream.str().c_str());
        return -1;
    }
}

string File::getFileName(string path) {
    size_t i = path.rfind('/', path.length());
    if (i != string::npos) {
        return (path.substr(i + 1, path.length() - i));
    }
    return path;
}
