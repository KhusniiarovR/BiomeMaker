#pragma once
#include <string>
#include <cstdlib>
#include <filesystem>
#include "Constants/Path.h"

#if defined(_WIN32)
    #include "Utilities/Windows/WindowsPaths.h"
#elif defined(__linux__)
    #include <unistd.h>
    #include <pwd.h>
#endif

inline void initPaths(const std::string& appName)
{
    std::string base;

#if defined(_WIN32)
    base = GetAppDataPath();

#elif defined(__linux__)
    const char* home = std::getenv("HOME");
    if (!home) home = getpwuid(getuid())->pw_dir;
    base = std::string(home) + "/.config";
#endif

    path::root     = base + "/" + appName;
    path::settings = path::root + "/settings.cfg";
    path::saves    = path::root + "/saves";
    path::worlds   = path::saves + "/worlds/";

    std::filesystem::create_directories(path::worlds);
}