#include "MessageQueue.h"
#include <string.h>

int MessageQueue::create(int flags, key_t key) {
    int id = msgget(key, flags);
    if (id == -1) {
        perror("Can't create shared memory segment");
        return -1;
    }
    this->id = id;
    cout << "Successfully created a message queue with ID " << id << endl;
    return id;
}

int MessageQueue::attach(int id) {
    this->id = id;
    cout << "Message queue attached." << endl;
    return id;
}

int MessageQueue::remove() {
    if (id == -1) {
        cerr << "Nothing to remove!" << endl;
        return 1;
    }
    if (msgctl(id, IPC_RMID, NULL) != 0) {
        perror("Can't remove the message queue");
        return -1;
    }
    cout << "Successfully removed a semaphore set with ID " << id << endl;
    id = -1;
    return 0;
}

int MessageQueue::send(string text, long type) {
    buf.mtype = type;
    strcpy(buf.mtext, text.c_str());
    if (msgsnd(id, &buf, sizeof(buf) - sizeof(long), 0) == -1) {
        perror("Can't send the message");
        return -1;
    }
    cout << "Message sent:" << endl;
    cout << text << endl;
    return 0;
}

int MessageQueue::receive(long type) {
    if (msgrcv(id, &buf, sizeof(buf) - sizeof(long), type, 0) == -1) {
        perror("Can't receive the message");
        return -1;
    }
    cout << "Message received:" << endl;
    cout << buf.mtext;
    return 0;
}

int MessageQueue::getID() {
    return id;
}
