# SOP-Linux-project
A C++ application providing an easy, object-oriented interface to Linux kernel functionalities like:
* File handling
* Process management
* Inter-process communication including message queues, semaphores and shared memory.

# Build prerequisites
    CMake 3.3 (it will probably compile using older version too, see the last paragraph)

# Build instructions
```
git clone https://github.com/daktyl/SOP-Linux-project.git
cd SOP-Linux-project/
cmake .
make
```

# How to use the program?
Use the numeric keys to navigate the menu. You need to choose an option and confirm your selection by pressing ENTER.
For a very detailed explanation of every function take a look at *Readme.txt* (written in Polish)

**WARNING** The program does not automatically clean up created shared memory segments, message queues and semaphores
(because you may have created them on purpose for future reference). Remember that if you don't need these resources anymore, you should free them using appropriate options provided by this application. Alternatively, you can call the Linux functions directly to delete these resources or just reboot your machine.

# Too old CMake version?
Try to change the minimum required version in CMakeLists.txt to match the version you have installed. There shouldn't be any issues.
