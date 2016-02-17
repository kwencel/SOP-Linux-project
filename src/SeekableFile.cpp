#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include "SeekableFile.h"

off_t SeekableFile::seek(off_t offset, int whence, int fd) {
    if (fd == -1) {
        fd = descriptor;
    }
    off_t tmp_position = lseek(fd, offset, whence);
    if (tmp_position == -1) {
        perror("Can't move the offset");
        return -1;
    }
    if (fd == descriptor) {
        position = tmp_position;
    }
    cout << "Current offset: " << position << endl;
    return tmp_position;
}

int SeekableFile::createFile(const char* path, mode_t mode) {
    return creat(path, mode);
}
