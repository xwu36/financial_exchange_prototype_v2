#ifndef SRC_PUBLISHER_FEED_PUBLISHER_H_
#define SRC_PUBLISHER_FEED_PUBLISHER_H_

#include <iostream>

#include "src/feed_event/feed_event.h"

namespace fep::src::publisher
{

    class EventsPublisher
    {
    public:
        EventsPublisher() {}
        EventsPublisher(const EventsPublisher &) = delete;
        EventsPublisher(EventsPublisher &&) = delete;
        ~EventsPublisher() = default;

        void Publish(const fep::src::feed_event::FeedEvents &events) const
        {
            std::cout << events.to_str() << std::endl;
        };
    };

} // namespace fep::src::publisher
#endif