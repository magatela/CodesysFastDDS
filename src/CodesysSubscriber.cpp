#include "CodesysSubscriber.hpp"

CodesysSubscriber::CodesysSubscriber() : type_(new dataExchangeDDSPubSubType())
{
    // Specific fast DDS
    participant_ = nullptr;
    subscriber_ = nullptr;
    topic_ = nullptr;
    reader_ = nullptr;

    // innere variable
    this->continueSubcription = true;
    this->checkSum2 = 0;
    this->checkSum3 = 0;
    this->checkSum4 = 0;
    this->Messages = 0;
}

CodesysSubscriber::~CodesysSubscriber()
{
    if (reader_ != nullptr)
    {
        subscriber_->delete_datareader(reader_);
    }
    if (topic_ != nullptr)
    {
        participant_->delete_topic(topic_);
    }
    if (subscriber_ != nullptr)
    {
        participant_->delete_subscriber(subscriber_);
    }
    DomainParticipantFactory::get_instance()->delete_participant(participant_);
}

//! Initialize the subscriber
bool CodesysSubscriber::init()
{
    DomainParticipantQos participantQos;
    participantQos.name("Participant_subscriber");
    participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);

    if (participant_ == nullptr)
    {
        return false;
    }

    // Register the Type
    type_.register_type(participant_);

    // Create the subscriptions Topic param: "name of the topic". "data type of the topic"
    topic_ = participant_->create_topic("CodesysTopic", "dataExchangeDDS", TOPIC_QOS_DEFAULT);

    if (topic_ == nullptr)
    {
        return false;
    }

    // Create the Subscriber
    subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);

    if (subscriber_ == nullptr)
    {
        return false;
    }

    // Create the DataReader
    reader_ = subscriber_->create_datareader(topic_, DATAREADER_QOS_DEFAULT, &listener_);

    if (reader_ == nullptr)
    {
        return false;
    }

    // set the on_data_available callback function
    listener_.callback = boost::bind(&CodesysSubscriber::subcriber_callback, this, _1);
    return true;
}

bool CodesysSubscriber::initShm()
{
    // initiaize semaphore
    sem_init(&(this->semaphore), 1, 1);

    // initialize the shared memory block
    shm = new SharedMemory("memory_subscriber", sizeof(*(ptrToShmBlock)));
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

bool CodesysSubscriber::ok()
{
    return this->continueSubcription;
}

//! Run the Subscriber
void CodesysSubscriber::subcriber_callback(DataExchange * msg)
{
    sem_wait(&(this->semaphore));
    ptrToShmBlock->endLoopSignal = msg->endLoopSignal;
    ptrToShmBlock->synchronizer = msg->synchronizer;
    ptrToShmBlock->diValue2 = msg->diValue2;
    ptrToShmBlock->diValue3 = msg->diValue3;
    ptrToShmBlock->diValue4 = msg->diValue4;
    sem_post(&(this->semaphore));
        
    if(msg->endLoopSignal == 0)
    {
        this->continueSubcription = false;
        // DEBUG //
        std::cout << "REPORT: Received messages:" << this->Messages << "\n" 
            << "endLoopSignal: " << msg->endLoopSignal << "\n"
            << "synchronizer:  " << msg->synchronizer << "\n"
            << "diValue2:      " << msg->diValue2 << "\n"
            << "diValue3:      " << msg->diValue3 << "\n"
            << "diValue4:      " << msg->diValue4 << "\n"
            << "--------------------------------\n"
            << "checkSum2      " << this->checkSum2 << "\n"
            << "checkSum3      " << this->checkSum3 << "\n"
            << "checkSum4      " << this->checkSum4 << "\n"
            << "--------------------------------\n\n";
        // DEBUG //
    } else {
        this->checkSum2 += msg->diValue2;
        this->checkSum3 += msg->diValue3;
        this->checkSum4 += msg->diValue4;
        this->Messages++;
    }
    
}

int main(int argc, char **argv)
{
    std::cout << "Starting subscriber." << std::endl;
    int samples = 10;

    CodesysSubscriber *mysub = new CodesysSubscriber();
    if (!mysub->init())
    {
        return -1;
    }
    
    if (!mysub->initShm())
    {
        return -1;
    }
    while (mysub->ok())
    {
    
    }
    delete mysub;
    return 0;
}
