#include "PubListener.hpp"

PubListener::PubListener() : matched_(0)
{
}

PubListener::~PubListener()
{
}

void PubListener::on_publication_matched(
    DataWriter *,
    const PublicationMatchedStatus &info)
{
    if (info.current_count_change == 1)
    {
        matched_ = info.total_count;
        std::cout << "Publisher matched." << std::endl;
    }
    else if (info.current_count_change == -1)
    {
        matched_ = info.total_count;
        std::cout << "Publisher unmatched." << std::endl;
    }
    else
    {
        std::cout << info.current_count_change
                  << " is not a valid value for PublicationMatchedStatus current count change." << std::endl;
    }
}
