#ifndef DISCPP_BOT_CONFIG_H
#define DISCPP_BOT_CONFIG_H

#include "log.h"
#include <string>
#include <vector>

namespace discpp {
	class BotConfig {
	public:
		std::vector<std::string> prefixes;
		int logger_flags;
		int messageCacheSize;
		std::string logger_path;
		BotConfig(std::vector<std::string> prefixes, int logger_flags = logger_flags::ERROR_SEVERITY | logger_flags::WARNING_SEVERITY, int messageCacheSize = 5000, std::string logger_path = "")
			: prefixes(prefixes), logger_flags(logger_flags), messageCacheSize(messageCacheSize), logger_path(logger_path) {}
	};
}
#endif