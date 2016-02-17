#ifndef SOP_LINUX_PROJECT_MESSAGEQUEUE_H
#define SOP_LINUX_PROJECT_MESSAGEQUEUE_H

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <iostream>

using namespace std;

struct msgbuf_mod {
    long mtype;
    char mtext[128];
};

class MessageQueue {
private:
    msgbuf_mod buf;
    int id = -1;

public:
    int create(int flags = 0600 | IPC_CREAT, key_t key = IPC_PRIVATE);

    int attach(int id);

    int remove();

    int send(string text, long type);

    int receive(long type);

    int getID();
};


#endif //SOP_LINUX_PROJECT_MESSAGEQUEUE_H
