#include "SubListener.hpp"

SubListener::SubListener()
{
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
        std::cout << "Subscriber unmatched." << std::endl;
        callback(&(this->tmp));
    }
    else
    {
        std::cout << info.current_count_change
                  << " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
    }
}

void SubListener::on_data_available(
    DataReader *reader)
{
    SampleInfo info;
    if (reader->take_next_sample(&dataExchange_, &info) == ReturnCode_t::RETCODE_OK)
    {
        if (info.valid_data)
        {
            this->tmp.endLoopSignal = this->dataExchange_.endLoopSignal();
            this->tmp.synchronizer =  this->dataExchange_.synchronizer();
            this->tmp.diValue2 = this->dataExchange_.diValue2();
            this->tmp.diValue3 = this->dataExchange_.diValue3();
            this->tmp.diValue4 = this->dataExchange_.diValue4();
            callback(&(this->tmp));
        }
    }
}

