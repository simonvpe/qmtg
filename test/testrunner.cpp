#include "spdlog/spdlog.h"

#define CATCH_CONFIG_MAIN
namespace MTG {
auto console = spdlog::stdout_logger_st("console");
}
#include "catch.hpp"
