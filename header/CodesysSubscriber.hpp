#ifndef _HELLOWORLD_SUBSUBTYPES_H_
#define _HELLOWORLD_SUBSUBTYPES_H_

#include "SubListener.hpp"

class CodesysSubscriber
{
    private:
        //Fast DDS specific
        DomainParticipant* participant_;
        Subscriber* subscriber_;
        DataReader* reader_;
        Topic* topic_;
        TypeSupport type_;
        SubListener listener_;
        std::atomic_bool * continueSubcription;

    public:
        CodesysSubscriber();
        virtual ~CodesysSubscriber();
        bool init();
        void run();
};
#endif