#include "CodesysPublisher.hpp"

CodesysPublisher::CodesysPublisher()
    : type_(new dataExchangeDDSPubSubType())
{
    participant_ = nullptr;
    publisher_ = nullptr;
    topic_ = nullptr;
    writer_ = nullptr;

    this->checkSum2 = 0;
    this->checkSum3 = 0;
    this->checkSum4 = 0;

    this->sentMessages = 0;
}

CodesysPublisher::~CodesysPublisher()
{
    if (writer_ != nullptr)
    {
        publisher_->delete_datawriter(writer_);
    }
    if (publisher_ != nullptr)
    {
        participant_->delete_publisher(publisher_);
    }
    if (topic_ != nullptr)
    {
        participant_->delete_topic(topic_);
    }
    DomainParticipantFactory::get_instance()->delete_participant(participant_);
}

//! Initialize the publisher
bool CodesysPublisher::initDDS()
{
    // initialize the message
    dataExchange_.endLoopSignal(1);
    dataExchange_.synchronizer(0);
    dataExchange_.diValue2(0);
    dataExchange_.diValue3(0);
    dataExchange_.diValue4(0);

    DomainParticipantQos participantQos;
    participantQos.name("Participant_publisher");
    participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);

    if (participant_ == nullptr)
    {
        return false;
    }

    // Register the Type
    type_.register_type(participant_);

    // Create the publications Topic
    topic_ = participant_->create_topic("CodesysTopic", "dataExchangeDDS", TOPIC_QOS_DEFAULT);

    if (topic_ == nullptr)
    {
        return false;
    }

    // Create the Publisher
    publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);

    if (publisher_ == nullptr)
    {
        return false;
    }

    // Create the DataWriter
    writer_ = publisher_->create_datawriter(topic_, DATAWRITER_QOS_DEFAULT, &listener_);

    if (writer_ == nullptr)
    {
        return false;
    }
        
    return true;
}

bool CodesysPublisher::initShm()
{
    // initiaize semaphore
    sem_init(&(this->semaphore), 1, 1);

    // initialize the shared memory block
    shm = new SharedMemory("memory_test", sizeof(*(ptrToShmBlock)));
    ptrToShmBlock = (DataExchange *)shm->open();
    if (ptrToShmBlock == nullptr)
    {
        return false;
    }
    //Wait for memory to be allocated * maybe not needed
    sleep(1);

    //initialize start values 
    ptrToShmBlock->endLoopSignal = 1;
    ptrToShmBlock->synchronizer = 0;
    ptrToShmBlock->diValue2 = 0;
    ptrToShmBlock->diValue3 = 0;
    ptrToShmBlock->diValue4 = 0;
    return true;
}

// Send a publication
bool CodesysPublisher::publish()
{
    if (listener_.matched_ > 0)
    {
        writer_->write(&dataExchange_);
        return true;
    }
    return false;
}

//! Run the Publisher
void CodesysPublisher::run()
{
    int endloopSignal = 1;
    
    while (endloopSignal != 0)
    {
        //lock the memory block before reading
        sem_wait(&(this->semaphore));
        
        //if the values has changed publish the nuw values
        if (dataExchange_.diValue2() != this->ptrToShmBlock->diValue2)
        {
            dataExchange_.endLoopSignal(this->ptrToShmBlock->endLoopSignal);
            dataExchange_.synchronizer(this->ptrToShmBlock->synchronizer);
            dataExchange_.diValue2(this->ptrToShmBlock->diValue2);
            dataExchange_.diValue3(this->ptrToShmBlock->diValue3);
            dataExchange_.diValue4(this->ptrToShmBlock->diValue4);
            endloopSignal = this->ptrToShmBlock->endLoopSignal;
            sem_post(&(this->semaphore));
            
            //  only when a subscriber is available
            if (listener_.matched_ > 0)
            {
                //publish the message
                writer_->write(&dataExchange_);
                //calculate the checksum
                this->checkSum2 += dataExchange_.diValue2();
                this->checkSum3 += dataExchange_.diValue3();
                this->checkSum4 += dataExchange_.diValue4();
                this->sentMessages++;
            }
        } else {
            dataExchange_.endLoopSignal(this->ptrToShmBlock->endLoopSignal);
            sem_post(&(this->semaphore));
            
            if(dataExchange_.endLoopSignal() == 0)
            {
                writer_->write(&dataExchange_);
                endloopSignal = 0;
            }
        }
    }
    //this will not happend because the listener is waitung for a subscriber
    printTransmissionResults();
}


void CodesysPublisher::printTransmissionResults()
{
    // DEBUG //
    std::cout << "REPORT: Sent messages:" << this->sentMessages << "\n" 
                << "endLoopSignal: " << this->dataExchange_.endLoopSignal() << "\n"
                << "synchronizer:  " << this->dataExchange_.synchronizer() << "\n"
                << "diValue2:      " << this->dataExchange_.diValue2() << "\n"
                << "diValue3:      " << this->dataExchange_.diValue3() << "\n"
                << "diValue4:      " << this->dataExchange_.diValue4() << "\n\n"
                << "--------------------------------\n"
                << "checkSum2      " << this->checkSum2 - this->dataExchange_.diValue2() << "\n"
                << "checkSum3      " << this->checkSum3 - this->dataExchange_.diValue3() << "\n"
                << "checkSum4      " << this->checkSum4 - this->dataExchange_.diValue4() << "\n";
    // END_DEBUG /*/
}

int main(int argc, char **argv)
{
    std::cout << "Starting publisher." << std::endl;
    
    CodesysPublisher *codesysPub = new CodesysPublisher();
    codesysPub->initShm();
    if (codesysPub->initDDS())
    {
        codesysPub->run();
    }
    delete codesysPub;
    return 0;
}
