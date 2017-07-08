#pragma once

#include <spdlog/spdlog.h>

namespace Logger
{
const std::string logger = "engine";

void init_logger();

void log(const std::string &message);
void warn(const std::string &message);

void logif(bool condition, const std::string &message);
void warnif(bool condition, const std::string &message);

}
