#include "tracker.h"

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Time constants in seconds
const long long DAY = 86400;
const long long P1_MIN = 1.5 * DAY;
const long long P1_MAX = 3 * DAY;
const long long P2_MIN = 6 * DAY;
const long long P2_MAX = 8 * DAY;

// Resolves the absolute path to your home directory
std::string getFilePath()
{
    const char* homeDir = std::getenv("HOME");

    if (homeDir == nullptr)
    {
        return "problems.csv";
    }

    return std::string(homeDir) + "/.cptrack.csv";
}

// Current UNIX timestamp
long long getCurrentTime()
{
    return std::chrono::duration_cast<std::chrono::seconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
}

void add_problem(const std::string& url)
{
    std::ofstream file(getFilePath(), std::ios::app);

    file << getCurrentTime() << "," << url << "\n";

    std::cout << "Problem tracked successfully.\n";
}

void remind(int phase)
{
    std::ifstream file(getFilePath());

    if (!file.is_open())
    {
        std::cout << "No tracked problems found.\n";
        return;
    }

    std::string line;
    std::string url;
    std::string timeStr;

    long long currentTime = getCurrentTime();

    bool foundAny = false;

    std::cout << "=== Phase " << phase << " Reminders ===\n";

    while (getline(file, line))
    {
        std::stringstream ss(line);

        getline(ss, timeStr, ',');
        getline(ss, url, ',');

        long long savedTime = std::stoll(timeStr);

        long long diff = currentTime - savedTime;

        if (phase == 1 && diff >= P1_MIN && diff <= P1_MAX)
        {
            std::cout << "[Phase 1 Ready] " << url << "\n";
            foundAny = true;
        }
        else if (phase == 2 && diff >= P2_MIN && diff <= P2_MAX)
        {
            std::cout << "[Phase 2 Ready] " << url << "\n";
            foundAny = true;
        }
    }

    if (!foundAny)
    {
        std::cout << "No problems due for this phase right now.\n";
    }
}

void mark_done(const std::string& targetUrl)
{
    std::string filePath = getFilePath();

    std::ifstream fileIn(filePath);

    std::vector<std::string> remainingLines;

    std::string line;
    std::string timeStr;
    std::string url;

    bool found = false;

    while (getline(fileIn, line))
    {
        std::stringstream ss(line);

        getline(ss, timeStr, ',');
        getline(ss, url, ',');

        if (url == targetUrl)
        {
            found = true;
        }
        else
        {
            remainingLines.push_back(line);
        }
    }

    fileIn.close();

    if (found)
    {
        std::ofstream fileOut(filePath, std::ios::trunc);

        for (const std::string& l : remainingLines)
        {
            fileOut << l << "\n";
        }

        std::cout << "Problem marked as done and removed from tracking.\n";
    }
    else
    {
        std::cout << "Error: URL not found in your tracking list.\n";
    }
}
