#include "CommandServerinfo.h"
#include "PreconditionRequireGuild.h"
#include <discpp/color.h>
#include <discpp/command.h>
#include <discpp/command_handler.h>

ServerinfoCommand::ServerinfoCommand() : discord::Command("serverinfo") {
	this->description = "displays helpful server information";
	this->hint_args = { "" };
}

bool ServerinfoCommand::CanRun(discord::Context ctx) {
	return Precondition::RequireGuild(ctx);
}

void ServerinfoCommand::CommandBody(discord::Context ctx) {
	discord::Guild guild = ctx.message.guild;
	discord::EmbedBuilder* embed = new discord::EmbedBuilder("", "", 0x3498DB);
	embed->SetThumbnail(guild.GetIconURL());
	std::string iconURL = guild.GetIconURL();
	embed->AddField("Server Name:", guild.name, true);
	embed->AddField("Server ID:", guild.id.c_str(), true);
	embed->AddField("Created At:", guild.created_at, true);
	std::string ownerId = guild.owner_id.c_str();
	embed->AddField("Owner:", "<@!" + ownerId + ">", true);
	embed->AddField("Members:", std::to_string(guild.member_count), true);
	embed->AddField("Server Region:", guild.region, true);
	embed->AddField("Verification Level:", std::to_string(guild.verification_level));
	ctx.channel.Send("", false, embed);
}