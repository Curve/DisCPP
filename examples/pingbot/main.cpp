/*
	Basic bot showing off commands
*/

#include <discpp/bot.h>
#include <discpp/context.h>
#include <discpp/command_handler.h>

// Events
#include <discpp/event_handler.h>
#include <discpp/events/ready_event.h>
#include <discpp/events/guild_member_add_event.h>
#include <discpp/events/channel_pins_update_event.h>

#include "ping_command.h"

int main(int argc, const char* argv[]) {
	std::ifstream token_file("token.txt", std::ios::out);
	std::string token;
	std::getline(token_file, token);

	discpp::BotConfig config{ {"!"} };
	discpp::Bot bot{ token, config }; // Token, config 

	PingCommand(); // This runs the constructor which will register the command.

	// I would recommend creating a class for the commands, you can check that in the examples folder
	// But, you can still register a command like you did before
	discpp::Command("test", "Quick example of a quick command", {}, [](discpp::Context ctx) {
		ctx.Send("Quick new command handler test");
	}, {});

	// New event system
	discpp::EventHandler<discpp::ReadyEvent>::RegisterListener([&bot](discpp::ReadyEvent event) {
		std::cout << "Ready!" << std::endl
			<< "Logged in as: " << bot.bot_user.username << "#" << bot.bot_user.discriminator << std::endl
			<< "ID: " << bot.bot_user.id << std::endl << "-----------------------------" << std::endl;

		// Will show "Playing With Crashes!"
		discpp::Activity activity = discpp::Activity("With Crashes!", discpp::presence::ActivityType::GAME, discpp::presence::Status::idle);
		bot.UpdatePresence(activity);
	});

	discpp::EventHandler<discpp::GuildMemberAddEvent>::RegisterListener([](discpp::GuildMemberAddEvent event) {
		discpp::Channel channel((discpp::snowflake) "638156895953223714");

		channel.Send("Welcome <@" + event.member.user.id + ">, hope you enjoy!");
	});

	discpp::EventHandler<discpp::ChannelPinsUpdateEvent>::RegisterListener([](discpp::ChannelPinsUpdateEvent event)->bool {
		event.channel.Send("Detected a pin update!");

		return false;
	});

	return bot.Run();
}