#include "CodesysSubscriber.hpp"

CodesysSubscriber::CodesysSubscriber():
        type_(new dataExchangeDDSPubSubType())
{
    participant_ = nullptr;
    subscriber_ = nullptr;
    topic_ = nullptr;
    reader_ = nullptr;
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

    //set end of subcrioption signal
    continueSubcription = listener_.getContinueSubcriptionSignal();
    return true;
}

//! Run the Subscriber
void CodesysSubscriber::run()
{
    while ((*continueSubcription)){

    }
}

int main(int argc, char **argv)
{
    std::cout << "Starting subscriber." << std::endl;
    int samples = 10;

    CodesysSubscriber * mysub = new CodesysSubscriber();
    if (mysub->init())
    {
        mysub->run();
    }

    delete mysub;
    return 0;
}
