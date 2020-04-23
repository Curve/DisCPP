﻿/*
	Basic bot showing off preconditions
*/

#include <discpp/bot.h>
#include <discpp/context.h>
#include <discpp/command_handler.h>

#include <discpp/event_handler.h>
#include <discpp/events/ready_event.h>

#include "CommandServerinfo.h"

int main(int argc, const char* argv[]) {
	std::ifstream token_file("token.txt", std::ios::out);
	std::string token;
	std::getline(token_file, token);

	discord::Bot bot { token, {"!"} }; // Token, command prefix.

	ServerinfoCommand();
	
	// ready event
	discord::EventHandler<discord::ReadyEvent>::RegisterListener([&bot](discord::ReadyEvent event) {
		std::cout << "Ready!" << std::endl; 
	});

	return bot.Run();
}