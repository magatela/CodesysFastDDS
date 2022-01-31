#ifndef PUBLISTENER_HPP
#define PUBLISTENER_HPP

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>

#include "dataExchangeDDSPubSubTypes.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace eprosima::fastdds::dds;

class CodesysPublisher;

class PubListener : public DataWriterListener
{
    public:
    
    std::atomic_int matched_;
    PubListener();
    ~PubListener() override;
    void on_publication_matched(
        DataWriter *,
        const PublicationMatchedStatus &info) override;
};

#endif
