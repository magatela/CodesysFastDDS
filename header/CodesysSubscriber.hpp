#ifndef _HELLOWORLD_SUBSUBTYPES_H_
#define _HELLOWORLD_SUBSUBTYPES_H_

#include "shared_memory.hpp"
#include "codesysToDDS.h"       /*  here are defined the structures for data 
                                    exchange using shared memory library */
#include <semaphore.h>

#include "SubListener.hpp"

class CodesysSubscriber
{
    private:
        //semaphore
        sem_t semaphore;

        // shared memory variables
        DataExchange * ptrToShmBlock;   //structure of shared memory block
        SharedMemory * shm;             //pointer to shared memory block
        
        // checksums for data transfer verification
        std::atomic_int checkSum2;
        std::atomic_int checkSum3;
        std::atomic_int checkSum4;
        std::atomic_int Messages;

        //Fast DDS specific
        DomainParticipant* participant_;
        Subscriber* subscriber_;
        DataReader* reader_;
        Topic* topic_;
        TypeSupport type_;
        SubListener listener_;
        bool continueSubcription;

    public:
        CodesysSubscriber();
        virtual ~CodesysSubscriber();
        bool init();
        bool initShm();
        void subcriber_callback(DataExchange * msg);
        bool ok();
};
#endif