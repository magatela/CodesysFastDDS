/*
 * SharedMemory.cpp
 *
 *  Created on: Dec 13, 2021
 *      Author: Miguel Avendano
 */

#include "shared_memory.hpp"

SharedMemory::SharedMemory(std::string sharedBlockName, size_t sharedBlockSize)
{
    this->ptrToSharedBlock = NULL;
    this->sharedBlockName = sharedBlockName;
    this->sharedBlockSize = sharedBlockSize;
}

SharedMemory::~SharedMemory()
{
    if(isOpen()){
        close();
    }
}

// opens or creates a memory block and returns a pointer to the memory block being shared
void * SharedMemory::open()
{
    // check that the memory block is not open
    if( isOpen()){
        std::cout << "01. The shared Block " << this->sharedBlockName << " is already open\n";
        throw 1;
    }

    // open the file descriptor
    errno = 0;
    //int fileDescriptor = shm_open(this->sharedBlockName.c_str(), O_CREAT | O_RDWR, S_IRWXU | S_IRWXG);
    int fileDescriptor = shm_open(this->sharedBlockName.c_str(), O_CREAT | O_RDWR, 00700); 
    if(fileDescriptor == -1)
    {
        std::cout << "02. Failed to open the file descriptor: " << this->sharedBlockName
                  << "\nError number: " << errno <<  " -> " << strerror(errno) << "\n";
        throw 1;
    }

    // truncate - Set the memory to the desired size
    errno = 0;
    if(ftruncate(fileDescriptor, this->sharedBlockSize))
    {
        std::cout << "03. Failed to truncate the shared memory: " << this->sharedBlockName
                  << "\nError number: " << errno <<  " -> " << strerror(errno) << "\n";
        throw 1;
    }

    // mmap the memory block
    this->ptrToSharedBlock = mmap(0, this->sharedBlockSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);

    if(this->ptrToSharedBlock == MAP_FAILED)
    {
        std::cout << "04. Failed to map the shared memory: " << this->sharedBlockName
                  << "\nError number: " << errno <<  " -> " << strerror(errno) << "\n";
        throw 1;
    }

    errno = 0;
    if(::close(fileDescriptor) == -1){
        std::cout << "05. Failed to close File descriptor: " << this->sharedBlockName
                  << "\nError number: " << errno <<  " -> " << strerror(errno) << "\n";
        throw 1;
    }

    return this->ptrToSharedBlock;

}

// check that the memory block is open 
bool SharedMemory::isOpen()
{
    return !(ptrToSharedBlock == NULL || ptrToSharedBlock == MAP_FAILED);
}

// close the shared memory block 
void SharedMemory::close()
{
    // check that the memory block is not open
    if( isOpen() == false){
        std::cout << "06. The shared Block " << this->sharedBlockName << "is not open\n";
        throw 2;

    }
    //unmap the memory block
    errno = 0;
    if(munmap(this->ptrToSharedBlock, this->sharedBlockSize) == -1)
    {
        std::cout << "07. Failed to unmap the memory block: " << this->sharedBlockName
                  << "\nError number: " << errno <<  " -> " << strerror(errno) << "\n";
        throw 1;
    }

    //unlink the memory block
    errno = 0;
    if(shm_unlink(this->sharedBlockName.c_str()) == -1 && errno != 2)
    {
        std::cout << "08. Failed to unlink the memory block: " << this->sharedBlockName
                  << "\nError number: " << errno <<  " -> " << strerror(errno) << "\n";
        throw 1;
    }

    this->ptrToSharedBlock = NULL;
}
