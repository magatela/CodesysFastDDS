#include "SubListener.hpp"

SubListener::SubListener()
{
    receivedMessages = 0;
    continueSubcription = true;
    // checksums for data transfer verification
    checkSum2 = 0;
    checkSum3 = 0;
    checkSum4 = 0;
}

SubListener::~SubListener()
{
}

void SubListener::on_subscription_matched(
    DataReader *,
    const SubscriptionMatchedStatus &info)
{
    if (info.current_count_change == 1)
    {
        std::cout << "Subscriber matched." << std::endl;
    }
    else if (info.current_count_change == -1)
    {
        std::cout << "Subscriber unmatched." << continueSubcription << std::endl;
    }
    else
    {
        std::cout << info.current_count_change
                  << " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
    }
}

// indicate that the subscription should finish
std::atomic_bool * SubListener::getContinueSubcriptionSignal()
{
    return &(this->continueSubcription);
}

void SubListener::on_data_available(
    DataReader *reader)
{
    SampleInfo info;
    if (reader->take_next_sample(&dataExchange_, &info) == ReturnCode_t::RETCODE_OK)
    {
        if (info.valid_data)
        {
            if (dataExchange_.endLoopSignal() == 0)
            {
                //  before finishing the program print the results 
                
                // DEBUG //
                std::cout << "REPORT: Received messages:" << this->receivedMessages << "\n" 
                    << "endLoopSignal: " << this->dataExchange_.endLoopSignal() << "\n"
                    << "synchronizer:  " << this->dataExchange_.synchronizer() << "\n"
                    << "diValue2:      " << this->dataExchange_.diValue2() << "\n"
                    << "diValue3:      " << this->dataExchange_.diValue3() << "\n"
                    << "diValue4:      " << this->dataExchange_.diValue4() << "\n"
                    << "--------------------------------\n"
                    << "checkSum2      " << this->checkSum2 - this->dataExchange_.diValue2() << "\n"
                    << "checkSum3      " << this->checkSum3 - this->dataExchange_.diValue3() << "\n"
                    << "checkSum4      " << this->checkSum4 - this->dataExchange_.diValue4() << "\n"
                    << "--------------------------------\n\n";
                // DEBUG /*/

                continueSubcription = false;
            } else {
                
                receivedMessages++;
                
                // checksums for data transfer verification
                checkSum2 += dataExchange_.diValue2();
                checkSum3 += dataExchange_.diValue3();
                checkSum4 += dataExchange_.diValue4();
            }
        }
    }
}
