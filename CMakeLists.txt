cmake_minimum_required(VERSION 3.3)
project(SOP_Linux_project)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(SOURCE_FILES
        src/main.cpp
        src/SequentialFile.cpp
        src/SequentialFile.h
        src/SeekableFile.cpp
        src/SeekableFile.h
        src/Process.cpp
        src/Process.h
        src/SharedMemory.cpp
        src/SharedMemory.h
        src/MessageQueue.cpp
        src/MessageQueue.h
        src/Semaphore.cpp
        src/Semaphore.h)

add_executable(SOP_Linux_project ${SOURCE_FILES})