#ifndef DISCPP_BOT_H
#define DISCPP_BOT_H

#include <string>
#include <future>
#include <string_view>
#include <optional>
#include <vector>

#include <nlohmann/json.hpp>

#include <cpprest/ws_client.h>

#include "channel.h"
#include "message.h"
#include "member.h"
#include "guild.h"
#include "log.h"

namespace discord {
	class Role;
	class User;
	class Activity;

	using namespace web::websockets::client;

	class Bot {
	public:
		std::string token; /**< Token for the current bot */
		std::vector<std::string> prefixes; /**< Command prefixes for the current bot */

		discord::User bot_user; /**< discord::User object representing current bot */
		discord::Logger logger; /**< discord::Logger object representing current logger */

		std::unordered_map<snowflake, Channel> channels; /**< List of channels the current bot can access */
		std::unordered_map<snowflake, Member> members; /**< List of members the current bot can access */
		std::unordered_map<snowflake, Guild> guilds; /**< List of guilds the current bot can access */ 
		std::unordered_map<snowflake, Message> messages; /**< List of messages the current bot can access */

		std::vector<std::future<void>> futures; /**< List of events */

		enum packet_opcode : int {
			dispatch = 0,				// Receive
			heartbeat = 1,				// Send/Receive
			identify = 2,				// Send
			status_update = 3,			// Send
			voice_state_update = 4,		// Send
			resume = 6,					// Send
			reconnect = 7,				// Receive
			request_guild_members = 8,	// Send
			invalid_session = 9,		// Receive
			hello = 10,					// Receive
			heartbeat_ack = 11			// Receive
		};

		Bot(std::string token, std::vector<std::string> prefixes, int logger_flags = logger_flags::ERROR_SEVERITY | logger_flags::WARNING_SEVERITY, std::string logger_path = "");
		int Run();
		discord::Guild GetGuild(snowflake guild_id);
		discord::User ModifyCurrentUser(std::string username);
		void LeaveGuild(discord::Guild guild);
		void UpdatePresence(discord::Activity activity);
		void CreateWebsocketRequest(nlohmann::json json);
		void SetCommandHandler(std::function<void(discord::Bot*, discord::Message)> command_handler);
		void DisconnectWebsocket();
		void ReconnectToWebsocket();

		template <typename FType, typename... T>
		void DoFunctionLater(FType&& func, T&&... args) {
			/**
			 * @brief Do a function async so it wont hold the bot up.
			 *
			 * ```cpp
			 *      bot.DoFunctionLater(method, this, message);
			 * ```
			 *
			 * @param[in] func The method that will run when the event is triggered.
			 * @param[in] args The arguments for the method.
			 *
			 * @return void
			 */

			futures.push_back(std::async(std::launch::async, func, std::forward<T>(args)...));
		}
	private:
		bool ready = false;
		bool disconnected = true;
		bool reconnecting = false;

		std::string session_id;
		std::string gateway_endpoint;

		nlohmann::json hello_packet;

		std::thread heartbeat_thread;

		std::mutex websocket_client_mutex;
		websocket_callback_client websocket_client;

		bool heartbeat_acked;
		int last_sequence_number;
		long long packet_counter;

		int message_cache_count = 10000;

		std::unordered_map<std::string, std::function<void(nlohmann::json)>> internal_event_map;

		// Websocket Methods
		void WebSocketStart();
		void OnWebSocketPacket(websocket_incoming_message msg);
		void HandleDiscordEvent(nlohmann::json const j, std::string event_name);
		void HandleDiscordDisconnect(websocket_close_status close_status, utility::string_t reason, std::error_code error_code);
		void HandleHeartbeat();
		nlohmann::json GetIdentifyPacket();

		void BindEvents();

		// Commands
		std::function<void(discord::Bot*, discord::Message)> fire_command_method;

		// Events
		void ReadyEvent(nlohmann::json result);
		void ResumedEvent(nlohmann::json result);
		void ReconnectEvent(nlohmann::json result);
		void InvalidSessionEvent(nlohmann::json result);
		void ChannelCreateEvent(nlohmann::json result);
		void ChannelUpdateEvent(nlohmann::json result);
		void ChannelDeleteEvent(nlohmann::json result);
		void ChannelPinsUpdateEvent(nlohmann::json result);
		void GuildCreateEvent(nlohmann::json result);
		void GuildUpdateEvent(nlohmann::json result);
		void GuildDeleteEvent(nlohmann::json result);
		void GuildBanAddEvent(nlohmann::json result);
		void GuildBanRemoveEvent(nlohmann::json result);
		void GuildEmojisUpdateEvent(nlohmann::json result);
		void GuildIntegrationsUpdateEvent(nlohmann::json result);
		void GuildMemberAddEvent(nlohmann::json result);
		void GuildMemberRemoveEvent(nlohmann::json result);
		void GuildMemberUpdateEvent(nlohmann::json result);
		void GuildMembersChunkEvent(nlohmann::json result);
		void GuildRoleCreateEvent(nlohmann::json result);
		void GuildRoleUpdateEvent(nlohmann::json result);
		void GuildRoleDeleteEvent(nlohmann::json result);
		void MessageCreateEvent(nlohmann::json result);
		void MessageUpdateEvent(nlohmann::json result);
		void MessageDeleteEvent(nlohmann::json result);
		void MessageDeleteBulkEvent(nlohmann::json result);
		void MessageReactionAddEvent(nlohmann::json result);
		void MessageReactionRemoveEvent(nlohmann::json result);
		void MessageReactionRemoveAllEvent(nlohmann::json result);
		void PresenceUpdateEvent(nlohmann::json result);
		void TypingStartEvent(nlohmann::json result);
		void UserUpdateEvent(nlohmann::json result);
		void VoiceStateUpdateEvent(nlohmann::json result);
		void VoiceServerUpdateEvent(nlohmann::json result);
		void WebhooksUpdateEvent(nlohmann::json result);
	};
}

#endif