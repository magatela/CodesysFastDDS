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

//for callback functions
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <functional>

#include "codesysToDDS.h"       /*  here are defined the structures for data 
                                    exchange using shared memory library */

#include "dataExchangeDDSPubSubTypes.h"

using namespace eprosima::fastdds::dds;

class SubListener : public DataReaderListener
{
    private:
    DataExchange tmp;
    dataExchangeDDS dataExchange_;

public:
    
    SubListener();
    ~SubListener() override;
    void on_subscription_matched(
        DataReader *,
        const SubscriptionMatchedStatus &info) override;

    void on_data_available(DataReader *reader) override;
       
    //pointer to  calback funktion
    boost::function<void(DataExchange *)> callback;  
};
#endif
