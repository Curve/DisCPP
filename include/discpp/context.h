#ifndef DISCPP_CONTEXT_H
#define DISCPP_CONTEXT_H

#include <vector>
#include <string>

#include "bot.h"
#include "guild.h"
#include "channel.h"
#include "user.h"
#include "member.h"
#include "message.h"

namespace discord {

	class Context {
	private:

	public:
		discord::Bot* bot;
		discord::Guild guild;
		discord::Channel channel;
		discord::User user;
		discord::Member author;
		discord::Message message;
		std::string remainder;
		std::vector<std::string> arguments;

		Context(discord::Bot* bot, discord::Channel channel, discord::Member author, discord::Message message, std::string remainder, std::vector<std::string> arguments);

		discord::Message Send(std::string text, bool tts = false, discord::EmbedBuilder* embed = nullptr, std::vector<discord::File> files = {});
	};
}

#endif