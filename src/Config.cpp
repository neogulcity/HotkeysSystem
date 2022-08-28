#include <Windows.h>

#include "Config.h"

spdlog::level::level_enum Config::GetLogLevel()
{
    const char* path = ".\\Data\\SKSE\\Plugins\\HotkeysSystem.ini";

    int value = GetPrivateProfileIntA("Logging", "Level", 0, path);

    switch (value) {
        case 0:
            return spdlog::level::level_enum::debug;

        case 1:
            return spdlog::level::level_enum::info;

        case 2:
            return spdlog::level::level_enum::warn;

        case 3:
            return spdlog::level::level_enum::err;

        default:
            return spdlog::level::level_enum::debug;
    }
}