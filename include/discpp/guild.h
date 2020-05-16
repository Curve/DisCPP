#ifndef DISCPP_GUILD_H
#define DISCPP_GUILD_H

#define RAPIDJSON_HAS_STDSTRING 1

#include "discord_object.h"
#include "emoji.h"
#include "channel.h"
#include "member.h"
#include "utils.h"
#include "role.h"
#include "image.h"
#include "audit_log.h"



#include <optional>
#include <utility>

namespace discpp {
	class Guild;
    class VoiceState;
	struct GuildBan {
		GuildBan(std::string reason, discpp::User user) : reason(reason), user(user) {
            /**
             * @brief Constructs a discpp::GuildBan object.
             *
             * ```cpp
             *      discpp::GuildBan guild_invite("Spamming", user);
             * ```
             *
             * @param[in] reason The reason for the ban
             * @param[in] user The banned user
             *
             * @return discpp::GuildBan, this is a constructor.
             */
		}

        std::string reason; /**< The reason for the ban. */
        discpp::User user; /**< The banned user. */
	};

	class GuildInvite {
	public:
	    enum class TargetUserType : int {
	        STREAM = 1
	    };

		GuildInvite() = default;
		GuildInvite(rapidjson::Document& json) {
            /**
             * @brief Constructs a discpp::GuildInvite object from json.
             *
             * ```cpp
             *      discpp::GuildInvite guild_invite(json);
             * ```
             *
             * @param[in] json The json data for the guild invite.
             *
             * @return discpp::GuildInvite, this is a constructor.
             */

            code = json["code"].GetString();
            rapidjson::Value::ConstMemberIterator itr = json.FindMember("guild");
            if (itr != json.MemberEnd()) {
                guild_id = SnowflakeFromString(json["guild"]["id"].GetString());
            }
            itr = json.FindMember("channel");
            if (itr != json.MemberEnd()) {
                rapidjson::Document channel_json;
                channel_json.CopyFrom(json["channel"], channel_json.GetAllocator());
                channel = discpp::GuildChannel(channel_json);
            }
            itr = json.FindMember("inviter");
            if (itr != json.MemberEnd()) {
                rapidjson::Document inviter_json;
                inviter_json.CopyFrom(json["inviter"], inviter_json.GetAllocator());
                inviter = discpp::User(inviter_json);
            }
            else {
                inviter = discpp::User();
            }
            itr = json.FindMember("target_user");
            if (itr != json.MemberEnd()) {
                rapidjson::Document target_json;
                target_json.CopyFrom(json["target_user"], target_json.GetAllocator());
                target_user = discpp::User(target_json);
            }
            target_user_type = static_cast<TargetUserType>(json["target_user_type"].GetInt());
            approximate_presence_count = json["approximate_presence_count"].GetInt();
            approximate_member_count = json["approximate_member_count"].GetInt();
        }
        std::string code; /**< The invite code (unique ID). */
        snowflake guild_id; /**< GThe guild this invite is for. */
        discpp::GuildChannel channel; /**< The channel this invite is for. */
        discpp::User inviter; /**< he user who created the invite. */
        discpp::User target_user; /**< The target user for this invite. */
        TargetUserType target_user_type; /**< The type of user target for this invite. */
        int approximate_presence_count; /**< Approximate count of online members (only present when target_user is set). */
        int approximate_member_count; /**< Approximate count of total members. */
	};

	class IntegrationAccount : public DiscordObject {
	public:
        IntegrationAccount() = default;
        IntegrationAccount(rapidjson::Document& json) {

            /**
             * @brief Constructs a discpp::IntegrationAccount object from json.
             *
             * ```cpp
             *      discpp::IntegrationAccount guild_integration_account(json);
             * ```
             *
             * @param[in] json The json data for the integration account.
             *
             * @return discpp::IntegrationAccount, this is a constructor.
             */
			id = SnowflakeFromString(json["id"].GetString());
			name = json["name"].GetString();
		}

        std::string name; /**< Name of the account. */
    };

	class Integration : public DiscordObject {
	public:
	    enum class IntegrationExpireBehavior : int {
	        REMOVE_ROLE = 0,
	        KICK = 1
	    };
		    Integration() = default;
        Integration(rapidjson::Document& json) {
            /**
             * @brief Constructs a discpp::Integration object from json.
             *
             * ```cpp
             *      discpp::Integration integration(json);
             * ```
             *
             * @param[in] json The json data for the integration.
             *
             * @return discpp::Integration, this is a constructor.
             */

            id = SnowflakeFromString(json["id"].GetString());
            name = json["name"].GetString();
            type = json["type"].GetString();
            enabled = json["enabled"].GetBool();
            syncing = json["syncing"].GetBool();
            role_id = SnowflakeFromString(json["role_id"].GetString());
            enable_emoticons = GetDataSafely<bool>(json, "enable_emoticons");
            expire_behavior = static_cast<IntegrationExpireBehavior>(json["expire_behavior"].GetInt());
            expire_grace_period = json["expire_grace_period"].GetInt();
            user = ConstructDiscppObjectFromJson(json, "user", discpp::User());
            account = ConstructDiscppObjectFromJson(json, "account", discpp::IntegrationAccount());
            synced_at = json["synced_at"].GetString();
        }

        std::string name; /**< Integration name. */
        std::string type; /**< Integration type (twitch, youtube, etc). */
        bool enabled; /**< Is this integration enabled? */
        bool syncing; /**< Is this integration syncing? */
        discpp::snowflake role_id; /**< ID that this integration uses for "subscribers". */
        bool enable_emoticons; /**< Whether emoticons should be synced for this integration (twitch only currently). */
        IntegrationExpireBehavior expire_behavior; /**< The behavior of expiring subscribers. */
        int expire_grace_period; /**< The grace period (in days) before expiring subscribers. */
        discpp::User user; /**< User for this integration. */
        discpp::IntegrationAccount account; /**< Integration account information. */
        // @TODO: Convert to iso8601Time
        std::string synced_at; /**< When this integration was last synced. */
	};

	class GuildEmbed : public DiscordObject {
	public:
        GuildEmbed() = default;
		GuildEmbed(rapidjson::Document& json) {
            /**
             * @brief Constructs a discpp::GuildEmbed object from json.
             *
             * ```cpp
             *      discpp::GuildEmbed guild_embed(json);
             * ```
             *
             * @param[in] json The json data for the embed.
             *
             * @return discpp::GuildEmbed, this is a constructor.
             */

            enabled = json["enabled"].GetBool();
			//channel_id = ContainsNotNull(json, "channel_id") ? json["channel_id"].GetString() : "";
			channel_id = GetIDSafely(json, "channel_id");
		}

        bool enabled; /**< Whether the embed is enabled. */
        snowflake channel_id; /**< The embed channel id. */
	};

	enum WidgetStyle : int {
		SHIELD,
		BANNER1,
		BANNER2,
		BANNER3,
		BANNER4
	};

    enum class GuildProperty : int {
        NAME,
        REGION,
        VERIFICATION_LEVEL,
        DEFAULT_MESSAGE_NOTIFICATIONS,
        EXPLICIT_CONTENT_FILTER,
        AFK_CHANNEL_ID,
        AFK_TIMEOUT,
        ICON,
        OWNER_ID,
        SPLASH,
        BANNER,
        SYSTEM_CHANNEL_ID,
        RULES_CHANNEL_ID,
        PUBLIC_UPDATES_CHANNEL_ID,
        PREFERRED_LOCALE
    };

    struct GuildModifyRequests {
        std::unordered_map<GuildProperty, std::variant<std::string, int, Image>> guild_requests;

        GuildModifyRequests(GuildProperty key, const std::variant<std::string, int, Image>& value ) : guild_requests({{ key, value }}) {};
        GuildModifyRequests(std::unordered_map<GuildProperty, std::variant<std::string, int, Image>> guild_requests) : guild_requests(std::move(guild_requests)) {};

        void Add(GuildProperty key, const std::variant<std::string, int, Image>& value) {
            guild_requests.insert({key, value});
        };

        void Remove(GuildProperty key) {
            guild_requests.erase(guild_requests.find(key));
        }
    };

	class Guild : public DiscordObject {
    private:
	    char flags;
	public:
		Guild() = default;
		Guild(snowflake id);
		Guild(rapidjson::Document& json);

		discpp::Guild Modify(GuildModifyRequests modify_requests);
		void DeleteGuild();
		std::unordered_map<discpp::snowflake, discpp::GuildChannel> GetChannels();
		std::unordered_map<discpp::snowflake, discpp::CategoryChannel> GetCategories();
        discpp::GuildChannel GetChannel(snowflake id);
        discpp::GuildChannel CreateChannel(std::string name, std::string topic = "", ChannelType type = ChannelType::GUILD_TEXT, int bitrate = 0, int user_limit = 0, int rate_limit_per_user = 0, int position = 0, std::vector<discpp::Permissions> permission_overwrites = {}, discpp::Channel category = {}, bool nsfw = false);
		void ModifyChannelPositions(std::vector<discpp::Channel>& new_channel_positions);
        std::shared_ptr<discpp::Member> GetMember(snowflake id);
		void EnsureBotPermission(Permission req_perm);
        std::shared_ptr<discpp::Member> AddMember(snowflake id, std::string access_token, std::string nick, std::vector<discpp::Role>& roles, bool mute, bool deaf);
		void RemoveMember(discpp::Member& member);
		std::vector<discpp::GuildBan> GetBans();
		std::string GetMemberBanReason(discpp::Member& member);
		void BanMember(discpp::Member& member, std::string reason = "");
		void BanMemberById(snowflake& user_id, std::string reason = "");
		void UnbanMember(discpp::Member& member);
		void UnbanMemberById(snowflake& user_id);
		void KickMember(discpp::Member& member);
        void KickMemberById(snowflake& member_id);
        std::shared_ptr<discpp::Role> GetRole(snowflake id);
        std::shared_ptr<discpp::Role> CreateRole(std::string name, Permissions permissions = Permissions(), int color = 0, bool hoist = false, bool mentionable = false);
		void ModifyRolePositions(std::vector<discpp::Role>& new_role_positions);
        std::shared_ptr<discpp::Role> ModifyRole(discpp::Role role, std::string name, Permissions permissions = Permissions(), int color = 0, bool hoist = false, bool mentionable = false);
		void DeleteRole(discpp::Role& role);
		int GetPruneAmount(int days);
		void BeginPrune(int days);
		std::vector<discpp::GuildInvite> GetInvites();
		std::vector<discpp::Integration> GetIntegrations();
		void CreateIntegration(snowflake id, std::string type);
		void ModifyIntegration(discpp::Integration& guild_integration, int expire_behavior, int expire_grace_period, bool enable_emoticons);
		void DeleteIntegration(discpp::Integration& guild_integration);
		void SyncIntegration(discpp::Integration& guild_integration);
		GuildEmbed GetGuildEmbed();
		GuildEmbed ModifyGuildEmbed(snowflake channel_id, bool enabled);
		discpp::GuildInvite GetVanityURL();
		std::string GetWidgetImageURL(WidgetStyle widget_style = WidgetStyle::SHIELD);

		std::unordered_map<snowflake, discpp::Emoji> GetEmojis();
        discpp::Emoji GetEmoji(snowflake id);
        discpp::Emoji CreateEmoji(std::string name, discpp::Image image, std::vector<discpp::Role>& roles);
		discpp::Emoji ModifyEmoji(discpp::Emoji& emoji, std::string name, std::vector<discpp::Role>& roles);
		void DeleteEmoji(discpp::Emoji& emoji);
		std::string GetIconURL(discpp::ImageType imgType = discpp::ImageType::AUTO);
        inline std::shared_ptr<discpp::Member> GetOwnerMember();
		discpp::AuditLog GetAuditLog();

		bool IsBotOwner();
		bool IsEmbedEnabled();
		bool IsWidgetEnabled();
		bool IsLarge();
		bool IsUnavailable();

		std::string name; /**< Guild name. */
		std::string icon; /**< Hashed guild icon. */
		std::string splash; /**< Optional hashed guild splash. */
		std::string discovery_splash; /**< Optional hashed discovery splash. */
		//bool owner; /**< Whether or not the bot is the owner of the guild. */
		snowflake owner_id; /**< ID of the guild owner. */
		int permissions; /**< Total permissions for the bot in the guild (does not include channel overrides). */
		std::string region; /**< Voice region id for the guild. */
		snowflake afk_channel_id; /**< ID of afk channel. */
		int afk_timeout; /**< AFK timeout in seconds. */
		//bool embed_enabled;/**< Whether this guild is embeddable (e.g. widget). */
		snowflake embed_channel_id;/**< If not null, the channel id that the widget will generate an invite to. */
		discpp::specials::VerificationLevel verification_level; /**< Verification level required for the guild. */
		discpp::specials::DefaultMessageNotificationLevel default_message_notifications; /**< Default message notifications level. */
		discpp::specials::ExplicitContentFilterLevel explicit_content_filter; /**< Explicit content filter level. */
		std::unordered_map<snowflake, std::shared_ptr<Role>> roles; /**< Roles in the guild. */
		std::unordered_map<snowflake, Emoji> emojis; /**< Custom guild emojis. */
		std::vector<std::string> features; /**< Enabled guild features. */
		discpp::specials::MFALevel mfa_level; /**< Required MFA level for the guild. */
		snowflake application_id; /**< Application id of the guild creator if it is bot-created. */
		//bool widget_enabled; /**< Whether or not the server widget is enabled. */
		snowflake widget_channel_id; /**< The channel id for the server widget. */
		snowflake system_channel_id; /**< The id of the channel where guild notices such as welcome messages and boost events are posted. */
        int system_channel_flags; /**< System channel flags. */
        snowflake rules_channel_id; /**< The id of the channel where "PUBLIC" guilds display rules and/or guidelines. */
		// @TODO: Convert to iso8601Time
		std::string joined_at; /**< When this guild was joined at. */
		//bool large; /**< Whether this is considered a large guild. */
		//bool unavailable; /**< Whether this guild is unavailable. */
		int member_count; /**< Total number of members in this guild. */
		std::vector<discpp::VoiceState> voice_states; /**< Array of partial voice state objects. */
		std::unordered_map<snowflake, std::shared_ptr<Member>> members; /**< Users in the guild. */
		std::unordered_map<snowflake, GuildChannel> channels; /**< Channels in the guild. */
		int max_presences; /**< The maximum amount of presences for the guild (the default value, currently 25000, is in effect when null is returned). */
		int max_members; /**< The maximum amount of members for the guild. */
		std::string vanity_url_code; /**< The vanity url code for the guild. */
		std::string description; /**< The description for the guild. */
		std::string banner; /**< Banner hash. */
		discpp::specials::NitroTier premium_tier; /**< Premium tier (Server Boost level). */
		int premium_subscription_count; /**< The number of boosts this server currently has. */
		std::string preferred_locale; /**< The preferred locale of a "PUBLIC" guild used in server discovery and notices from Discord; defaults to "en-US". */
        std::shared_ptr<discpp::GuildChannel> public_updates_channel; /**< The channel where admins and moderators of "PUBLIC" guilds receive notices from Discord. */
		int approximate_member_count; /**< Approximate number of members in this guild, returned from the GET /guild/<id> endpoint when with_counts is true. */
		int approximate_presence_count; /**< Approximate number of online members in this guild, returned from the GET /guild/<id> endpoint when with_counts is true. */
		std::string created_at; /**< The id of the channel where admins and moderators of "PUBLIC" guilds receive notices from Discord. */
	};

    class VoiceState {
    public:
        VoiceState() = default;
        VoiceState(rapidjson::Document& json) {
            /**
             * @brief Constructs a discpp::VoiceState object from json.
             *
             * ```cpp
             *      discpp::VoiceState voice_state(json);
             * ```
             *
             * @param[in] json The json data for the voice state.
             *
             * @return discpp::VoiceState, this is a constructor.
             */
            guild_id = GetIDSafely(json, "guild_id");
            channel_id = GetIDSafely(json, "channel_id");
            user_id = SnowflakeFromString(json["user_id"].GetString());
            if (ContainsNotNull(json, "member")) {
                rapidjson::Document member_json; 
                member_json.CopyFrom(json["member"], member_json.GetAllocator());

                discpp::Guild guild(guild_id);
                member = discpp::Member(member_json, guild);
            }
            session_id = json["session_id"].GetString();
            deaf = json["deaf"].GetBool();
            mute = json["mute"].GetBool();
            self_deaf = json["self_deaf"].GetBool();
            self_mute = json["self_mute"].GetBool();
            self_stream = GetDataSafely<bool>(json, "self_stream");
            suppress = json["suppress"].GetBool();
        }

        snowflake guild_id; /**< The guild id this voice state is for. */
        snowflake channel_id; /**< The channel id this user is connected to. */
        snowflake user_id; /**< The user id this voice state is for. */
        discpp::Member member; /**< The guild member this voice state is for. */
        std::string session_id; /**< The session id for this voice state. */
        bool deaf; /**< Whether this user is deafened by the server. */
        bool mute; /**< Whether this user is muted by the server. */
        bool self_deaf; /**< Whether this user is locally deafened. */
        bool self_mute; /**< Whether this user is locally muted. */
        bool self_stream; /**< Whether this user is streaming using "Go Live". */
        bool suppress; /**< Whether this user is muted by the current user. */
    };
}

#endif