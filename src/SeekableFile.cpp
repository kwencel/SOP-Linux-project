#include <unistd.h>
#include <iostream>
#include "SeekableFile.h"

off_t SeekableFile::seek(off_t offset, int whence, int fd) {
    if (fd == -1) {
        fd = descriptor;
//        if (fd == -1) {
//            cerr << "Open the file first!" << endl;
//            return 1;
//        }
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
