#include "Semaphore.h"
#include <iostream>

using namespace std;

int Semaphore::create(int amount, int flags, key_t key) {
    int id = semget(key, amount, flags);
    if (id == -1) {
        perror("Can't create shared memory segment");
        return -1;
    }
    this->id = id;
    this->nsems = amount;
    cout << "Successfully created " << amount << " semaphore(s) with ID " << id << endl;
    return id;
}

int Semaphore::attach(int id, int amount) {
    this->id = id;
    this->nsems = amount;
    cout << "Semaphore set attached." << endl;
    return id;
}

int Semaphore::remove() {
    if (id == -1) {
        cerr << "Nothing to remove!" << endl;
        return 1;
    }
    if (semctl(id, nsems, IPC_RMID) != 0) {
        perror("Can't remove semaphore set");
        return -1;
    }
    cout << "Successfully removed a semaphore set with ID " << id << endl;
    id = -1;
    nsems = -1;
    return 0;
}

int Semaphore::P(unsigned short which, short amount) {
    buf.sem_num = which;
    buf.sem_op = -amount;
    buf.sem_flg = 0;
    if (semop(id, &buf, 1) != 0) {
        perror("Can't perform lock operation");
        return -1;
    }
    return 0;
}

int Semaphore::V(unsigned short which, short amount) {
    buf.sem_num = which;
    buf.sem_op = +amount;
    buf.sem_flg = 0;
    if (semop(id, &buf, 1) != 0) {
        perror("Can't perform unlock operation");
        return -1;
    }
    return 0;
}

int Semaphore::set(unsigned short which, short amount) {
    if (semctl(id, which, SETVAL, amount) == -1) {
        perror("Can't set semaphore value");
        return -1;
    }
    return 0;
}

int Semaphore::getID() {
    return id;
}

int Semaphore::getAmount() {
    return nsems;
}
