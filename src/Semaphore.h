#ifndef SOP_LINUX_PROJECT_SEMAPHORE_H
#define SOP_LINUX_PROJECT_SEMAPHORE_H

#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>

class Semaphore {
private:
    int id = -1;
    int nsems = -1;
    struct sembuf buf;

public:
    int create(int amount, int flags = 0600 | IPC_CREAT, key_t key = IPC_PRIVATE);

    int attach(int id, int amount);

    int remove();

    int P(unsigned short which, short amount);

    int V(unsigned short which, short amount);

    int set(unsigned short which, short amount);

    int getID();

    int getAmount();
};


#endif //SOP_LINUX_PROJECT_SEMAPHORE_H
