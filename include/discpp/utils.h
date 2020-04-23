#ifndef DISCPP_UTILS_H
#define DISCPP_UTILS_H

#include "discord_object.h"

#include <nlohmann/json.hpp>

#include <cpr/cpr.h>

#include <boost/date_time.hpp>

#include <unordered_map>

namespace discord {
	class Bot;

	namespace globals {
		inline discord::Bot* bot_instance;
	}

	namespace specials {
		enum class NitroSubscription : int {
			NO_NITRO = 0,
			NITRO_CLASSIC = 1,
			NITRO = 2
		};

		enum class NitroTier : int {
			NO_TIER = 0,
			TIER_1 = 1,
			TIER_2 = 2,
			TIER_3 = 3
		};

		enum VerificationLevel : int {
			NO_VERIFICATION = 0,
			LOW = 1,
			MEDIUM = 2,
			HIGH = 3,
			VERY_HIGH = 4
		};

		enum DefaultMessageNotificationLevel : int {
			ALL_MESSAGES = 0,
			ONLY_MENTIONS = 1
		};

		enum class ExplicitContentFilterLevel : int {
			DISABLED = 0,
			MEMBERS_WITHOUT_ROLES = 1,
			ALL_MEMBERS = 2
		};

		enum class MFALevel : int {
			NO_MFA = 0,
			ELEVATED = 1
		};
	}

	std::string GetOsName();

	inline std::string Endpoint(std::string endpoint_format) {
		endpoint_format = endpoint_format[0] == '/' ? endpoint_format : '/' + endpoint_format;
		return "https://discordapp.com/api/v6" + endpoint_format;
	}

	template <typename type>
	inline type GetFromVector(std::vector<type> vector, type item) {
		auto new_item = std::find_if(vector.begin(), vector.end(), [](type a, type b) { return a == b; });

		if (new_item != vector.end()) return new_item;
		return nullptr;
	}

	template<typename T>
	inline T GetDataSafely(nlohmann::json json, std::string value_name) {
		return (json.contains(value_name) && json[value_name] != nullptr) ? json[value_name].get<T>() : T();
	}

	template<typename type, typename func>
	inline type GetIf(std::vector<type>& items, func const& predicate) {
		auto item = std::find_if(items.begin(), items.end(), predicate);

		if (item != items.end()) {
			return *item;
		}
		return type();
	}

	// Rate limits
	struct RateLimit {
		int limit = 500;
		int remaining_limit = 500;
		double ratelimit_reset = 0;
	};

	enum RateLimitBucketType : int {
		CHANNEL,
		GUILD,
		WEBHOOK,
		GLOBAL
	};

	inline std::unordered_map<snowflake, RateLimit> guild_ratelimit;
	inline std::unordered_map<snowflake, RateLimit> channel_ratelimit;
	inline std::unordered_map<snowflake, RateLimit> webhook_ratelimit;
	inline RateLimit global_ratelimit;

	inline int WaitForRateLimits(snowflake object, RateLimitBucketType ratelimit_bucket);
	inline void HandleRateLimits(cpr::Header header, snowflake object, RateLimitBucketType ratelimit_bucket);
	// End of rate limits

	extern nlohmann::json HandleResponse(cpr::Response response, snowflake object, RateLimitBucketType ratelimit_bucket);
	extern nlohmann::json SendGetRequest(std::string url, cpr::Header headers, snowflake object, RateLimitBucketType ratelimit_bucket, cpr::Body body = {});
	extern nlohmann::json SendPostRequest(std::string url, cpr::Header headers, snowflake object, RateLimitBucketType ratelimit_bucket, cpr::Body body = {});
	extern nlohmann::json SendPutRequest(std::string url, cpr::Header headers, snowflake object, RateLimitBucketType ratelimit_bucket, cpr::Body body = {});
	extern nlohmann::json SendPatchRequest(std::string url, cpr::Header headers, snowflake object, RateLimitBucketType ratelimit_bucket, cpr::Body body = {});
	extern nlohmann::json SendDeleteRequest(std::string url, cpr::Header headers, snowflake object, RateLimitBucketType ratelimit_bucket);
	cpr::Header DefaultHeaders(cpr::Header add = {});
	bool StartsWith(std::string string, std::string prefix);
	std::vector<std::string> SplitString(std::string str, std::string delimter);
	std::string CombineStringVector(std::vector<std::string> v, std::string delimiter = " ", int offset = 0);
	std::string ReadEntireFile(std::ifstream& file);
	std::string Base64Encode(std::string text);
	std::string ReplaceAll(std::string data, std::string toSearch, std::string replaceStr);
	std::string EscapeString(std::string string);
	time_t TimeFromSnowflake(snowflake snow);
	std::string FormatTimeFromSnowflake(snowflake snow);
}

#endif