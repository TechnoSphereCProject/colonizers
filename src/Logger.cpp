#include "Logger.h"

namespace spd = spdlog;

void Logger::init_logger() {
    spd::stdout_color_st(logger);
}

void Logger::log(const std::string &message) {
    spd::get(logger)->info(message);
}

void Logger::warn(const std::string &message) {
    spd::get(logger)->warn(message);
}

void Logger::logif(bool condition, const std::string &message) {
    spd::get(logger)->info_if(condition, message);
}

void Logger::warnif(bool condition, const std::string &message) {
    spd::get(logger)->warn_if(condition, message);
}
