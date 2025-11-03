#include "logger.hpp"

#include <filesystem>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

bool init_logger() {
    try {
        std::string log_file = (std::filesystem::current_path() / "log" / "log.log").string();
        auto logger = spdlog::basic_logger_mt("logger", log_file, true);
        spdlog::set_default_logger(logger);
        spdlog::set_level(spdlog::level::trace);
        spdlog::flush_on(spdlog::level::trace);
    }
    catch (const spdlog::spdlog_ex& ex) {
        return false;    
    }

    return true;
}
