#ifndef CODESYSPUBLISHER_H
#define CODESYSPUBLISHER_H

#include "PubListener.hpp"

//for callback functions
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <functional>

#include "shared_memory.hpp"
#include "codesysToDDS.h"       /*  here are defined the structures for data 
                                    exchange using shared memory library */
#include <semaphore.h>

using namespace eprosima::fastdds::dds;

class CodesysPublisher
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
        std::atomic_int sentMessages;

        // Message type Fast DDS
        dataExchangeDDS dataExchange_;
        
        // Fast DDS specific
        DomainParticipant* participant_;
        Publisher* publisher_;
        Topic* topic_;
        DataWriter* writer_;
        TypeSupport type_;
        PubListener  listener_;

    public:
        CodesysPublisher();
        virtual ~CodesysPublisher();
        bool initDDS();
        bool initShm();
        bool publish();
        void run();
        void printTransmissionResults();
};

#endif

