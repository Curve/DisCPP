#ifndef DISCPP_CHANNEL_DELETE_EVENT_H
#define DISCPP_CHANNEL_DELETE_EVENT_H

#include "../event.h"
#include "../channel.h"

#include <nlohmann/json.hpp>

namespace discord {
	class ChannelDeleteEvent : public Event {
	public:
		inline ChannelDeleteEvent(discord::Channel channel) : channel(channel) {}

		discord::Channel channel;
	};
}

#endif