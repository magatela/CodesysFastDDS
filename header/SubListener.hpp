#ifndef SUBLISTENER_HPP
#define SUBLISTENER_HPP

#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>

#include "dataExchangeDDSPubSubTypes.h"

using namespace eprosima::fastdds::dds;

class SubListener : public DataReaderListener
{
    private:
    //  signal to close the subscriber
    std::atomic_bool continueSubcription;
    
    // checksums for data transfer verification
    std::atomic_int checkSum2;
    std::atomic_int checkSum3;
    std::atomic_int checkSum4;
    std::atomic_int receivedMessages;
    
public:
    dataExchangeDDS dataExchange_;
    SubListener();
    ~SubListener() override;
    void on_subscription_matched(
        DataReader *,
        const SubscriptionMatchedStatus &info) override;

    void on_data_available(DataReader *reader) override;
    std::atomic_bool * getContinueSubcriptionSignal();
};
#endif
