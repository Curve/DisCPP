#ifndef DISCPP_GUILD_CREATE_EVENT_H
#define DISCPP_GUILD_CREATE_EVENT_H

#include "../event.h"
#include "../guild.h"

#include <nlohmann/json.hpp>

namespace discord {
	class GuildCreateEvent : public Event {
	public:
		inline GuildCreateEvent(discord::Guild guild) : guild(guild) {}

		discord::Guild guild;
	};
}

#endif