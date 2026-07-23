#include "queue.h"
#include "common.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

namespace Queue {

    struct QueuedProblem {
        long long timestamp;
        std::string name;
        std::string url;
    };

    std::string get_queue_file_path() {
        return (get_app_dir() / "queue.tsv").string();
    }

    bool is_duplicate_in_queue(const std::string& target_name, const std::string& target_url) {
        std::ifstream file(get_queue_file_path());
        if (!file.is_open()) {
            return false;
        }

        std::string line, time_str, name, url;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::getline(ss, time_str, '|');
            std::getline(ss, name, '|');
            std::getline(ss, url);

            if (url == target_url || is_equal_ignore_case(name, target_name)) {
                return true;
            }
        }
        return false;
    }

    std::string format_elapsed_time(long long added_timestamp) {
        long long current_time = get_current_time();
        long long elapsed_seconds = current_time - added_timestamp;

        if (elapsed_seconds < 60) {
            return "< 1 min";
        }

        long long minutes = (elapsed_seconds / 60) % 60;
        long long hours = (elapsed_seconds / 3600) % 24;
        long long days = elapsed_seconds / 86400;

        std::stringstream formatted;
        if (days > 0) {
            formatted << days << "d " << hours << "h";
        } else if (hours > 0) {
            formatted << hours << "h " << minutes << "m";
        } else {
            formatted << minutes << "m";
        }
        
        return formatted.str();
    }

    void add() {
        std::string name, url;
        std::cout << "Enter Problem Name: "; std::getline(std::cin, name);
        std::cout << "Enter Problem Link: "; std::getline(std::cin, url);
        
        if (name.empty() || url.empty()) {
            std::cout << "Error: Both Problem Name and Link are required.\n";
            return;
        }

        if (is_duplicate_in_queue(name, url)) {
            std::cout << "Error: This problem is already active in your Queue!\n";
            return;
        }

        std::ofstream file(get_queue_file_path(), std::ios::app);
        file << get_current_time() << '|' << name << '|' << url << '\n';
        std::cout << "[+] Problem successfully queued for training.\n";
    }

    void list() {
        std::ifstream file(get_queue_file_path());
        if (!file.is_open()) {
            std::cout << "Active Queue is currently empty.\n";
            return;
        }

        std::vector<QueuedProblem> problems;
        std::string line, time_str, name, url;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::getline(ss, time_str, '|');
            std::getline(ss, name, '|');
            std::getline(ss, url);
            
            long long timestamp = 0;
            try { timestamp = std::stoll(time_str); } catch (...) {}
            problems.push_back({timestamp, name, url});
        }

        if (problems.empty()) {
            std::cout << "No active problems found in Queue.\n";
            return;
        }

        std::cout << "\n================================================================================================\n";
        std::cout << std::left << std::setw(6) << "No." 
                  << std::setw(32) << "Problem Name" 
                  << std::setw(14) << "Time in Queue" 
                  << "Problem URL\n";
        std::cout << "================================================================================================\n";

        for (size_t i = 0; i < problems.size(); ++i) {
            const QueuedProblem& p = problems[i];
            std::string elapsed = format_elapsed_time(p.timestamp);

            std::cout << std::left 
                      << std::setw(6) << (i + 1)
                      << std::setw(32) << p.name
                      << std::setw(14) << elapsed
                      << p.url << '\n';
        }
        std::cout << "================================================================================================\n";
    }

    void remove(const std::string& target) {
        std::string file_path = get_queue_file_path();
        std::ifstream file(file_path);
        if (!file.is_open()) {
            std::cout << "Error: Queue database not found.\n";
            return;
        }

        std::vector<std::string> remaining_lines;
        std::string line, time_str, name, url;
        bool found = false;

        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::getline(ss, time_str, '|');
            std::getline(ss, name, '|');
            std::getline(ss, url);

            if (url == target || is_equal_ignore_case(name, target)) {
                found = true;
            } else {
                remaining_lines.push_back(line);
            }
        }
        file.close();

        if (found) {
            std::ofstream file_out(file_path, std::ios::trunc);
            for (const std::string& l : remaining_lines) file_out << l << '\n';
            std::cout << "[-] Problem '" << target << "' removed from active Queue.\n";
        } else {
            std::cout << "Error: No matching problem found in Queue.\n";
        }
    }
}
