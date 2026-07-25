#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <cctype>
#include <filesystem>

inline std::filesystem::path get_app_dir() {
    std::filesystem::path base_path;

#if defined(_WIN32)
    const char* appdata = std::getenv("LOCALAPPDATA");
    if (appdata != nullptr) {
        base_path = std::filesystem::path(appdata);
    } else {
        const char* user_profile = std::getenv("USERPROFILE");
        base_path = std::filesystem::path(user_profile != nullptr ? user_profile : ".") / "AppData" / "Local";
    }
#elif defined(__APPLE__)
    const char* home = std::getenv("HOME");
    base_path = std::filesystem::path(home != nullptr ? home : ".") / "Library" / "Application Support";
#else
    const char* xdg_data = std::getenv("XDG_DATA_HOME");
    if (xdg_data != nullptr) {
        base_path = std::filesystem::path(xdg_data);
    } else {
        const char* home = std::getenv("HOME");
        base_path = std::filesystem::path(home != nullptr ? home : ".") / ".local" / "share";
    }
#endif

    std::filesystem::path app_dir = base_path / "cptrack";
    std::filesystem::create_directories(app_dir);
    return app_dir;
}

inline long long get_current_time() {
    return std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

inline bool is_equal_ignore_case(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) {
        return false;
    }
    
    return std::equal(a.begin(), a.end(), b.begin(),
                      [](char c1, char c2) {
                          return std::tolower(static_cast<unsigned char>(c1)) ==
                                 std::tolower(static_cast<unsigned char>(c2));
                      });
}

#endif // COMMON_H
