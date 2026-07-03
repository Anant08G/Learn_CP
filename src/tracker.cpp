#include "tracker.h"

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>

// Resolves the absolute path to your home directory
std::string get_file_path(){
    const char* home_dir = std::getenv("HOME");
    if(home_dir == nullptr) return "problems.csv";
    return std::string(home_dir) + "/.cptrack.csv";
}
// Current UNIX timestamp
long long get_current_time(){
    return std::chrono::duration_cast<std::chrono::seconds>(
               std::chrono::system_clock::now().time_since_epoch())
        .count();
}
// Checks wheather string "a" is same as "b" irrespective of the letter cases.
bool is_equal(const std::string& a, const std::string& b) {
    return std::equal(a.begin(), a.end(), b.begin(), b.end(),
                      [](char a, char b) {
                          return std::tolower(static_cast<unsigned char>(a)) ==
                                 std::tolower(static_cast<unsigned char>(b));
                      });
}
void add_problem(){
    std::string name, url;
    std::cout << "Enter the Problem Name: "; std::getline(std::cin, name);
    std::cout << "Enter the Problem link: "; std::getline(std::cin, url);
    std::ofstream file(get_file_path(), std::ios::app);
    file << get_current_time() <<','<< name <<','<< url <<'\n';
    std::cout << "Problem has been added to be tracked.\n";
}
void problem_list(){
    std::ifstream file(get_file_path());

    if (!file.is_open()){
        std::cout << "No tracked problems found.\n";
        return;
    }
    std::cout << "--------------------------------------------------------------------------------------------------------------------------\n";
    std::cout << std::left << std::setw(4) << "No." 
              << std::setw(18) << "Problem_Name" 
              << std::setw(9) << "Time" 
              << "URL\n";
    std::cout << "--------------------------------------------------------------------------------------------------------------------------\n";
    std::string line;
    std::string url;
    std::string time;
    std::string name;

    long long current_time = get_current_time();
    bool found = false;
    int index = 1;
    
    while(std::getline(file, line)){
        std::stringstream ss(line);
        std::getline(ss, time, ',');
        std::getline(ss, name, ',');
        std::getline(ss, url);
        long long time_elapsed = current_time - std::stoll(time);
        long long hours = time_elapsed / 3600;
        if(hours >= 48) time = std::to_string(hours/24) + " days";
        else time = std::to_string(hours) + " hours";
        std::cout << std::left 
                  << std::setw(4) << index++
                  << std::setw(18) << name
                  << std::setw(9) << time
                  << url <<'\n';
        found = true;
    }
    if(!found) std::cout << "There is no problem to track in the database\n";
    std::cout << "--------------------------------------------------------------------------------------------------------------------------\n";
}

void remove(const std::string& target) {
    std::string file_path = get_file_path();
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cout << "Error: Could not open database file.\n";
        return;
    }

    std::vector<std::string> remaining_lines;
    std::string line;
    bool found = false;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string time_str, name, url;

        std::getline(ss, time_str, ',');
        std::getline(ss, name, ',');
        std::getline(ss, url);

        if (url == target || is_equal(name, target)) {
            found = true;
        } else {
            remaining_lines.push_back(line);
        }
    }
    file.close();

    // Only rewrite the file if we actually found and removed something
    if (found) {
        std::ofstream file_out(file_path, std::ios::trunc);
        for (const std::string& l : remaining_lines) {
            file_out << l << '\n';
        }
        std::cout << "Problem '" << target << "' removed from tracking.\n";
    } else {
        std::cout << "Error: No problem matching match found.\n";
    }
}
