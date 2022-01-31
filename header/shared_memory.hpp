/*
 * SharedMemory.h
 *
 *  Created on: Dec 13, 2021
 *      Author: imig
 */

#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <iostream>     //for cout()
#include <cstring>      //for strerror()
#include <string>       //for sstd::string

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

class SharedMemory {
private:
        std::string sharedBlockName;
        size_t sharedBlockSize;
    public:
        void * ptrToSharedBlock;
        SharedMemory(std::string sharedBlockName, size_t sharedBlockSize);
        ~SharedMemory();
        void * open();
        bool isOpen();
        void close();
};

#endif /* SHAREDMEMORY_H_ */
