#include "library.h"
#include "common.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>

namespace Library {

    struct Problem {
        std::string tag;
        std::string name;
        std::string url;
        int score;
    };

    std::string get_library_file_path() {
        return (get_app_dir() / "library.tsv").string();
    }

    std::string get_tags_file_path() {
        return (get_app_dir() / "tags.tsv").string();
    }

    // Helper: Loads all unique tags from disk
    std::vector<std::string> load_tags() {
        std::vector<std::string> tags;
        std::ifstream file(get_tags_file_path());
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                tags.push_back(line);
            }
        }
        return tags;
    }

    // Helper: Checks if a problem name or URL already exists in library.tsv
    bool is_duplicate(const std::string& target_name, const std::string& target_url) {
        std::ifstream file(get_library_file_path());
        if (!file.is_open()) {
            return false;
        }

        std::string line, tag, name, url, score_str;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::getline(ss, tag, '|');
            std::getline(ss, name, '|');
            std::getline(ss, url, '|');
            std::getline(ss, score_str);

            if (url == target_url || is_equal_ignore_case(name, target_name)) {
                return true;
            }
        }
        return false;
    }

    void add_tag() {
        std::string tag_name;
        std::cout << "Enter new Tag Name (e.g., Binary_Search, Dynamic_Programming): ";
        std::getline(std::cin, tag_name);

        if (tag_name.empty()) {
            std::cout << "Error: Tag name cannot be empty.\n";
            return;
        }

        std::replace(tag_name.begin(), tag_name.end(), ' ', '_');

        std::vector<std::string> existing_tags = load_tags();
        for (const std::string& t : existing_tags) {
            if (is_equal_ignore_case(t, tag_name)) {
                std::cout << "[-] Tag '" << tag_name << "' already exists.\n";
                return;
            }
        }

        std::ofstream file(get_tags_file_path(), std::ios::app);
        file << tag_name << '\n';
        std::cout << "[+] Tag '" << tag_name << "' created successfully.\n";
    }

    void add_problem() {
        std::string name, url, score_str;
        std::cout << "Enter Problem Name: "; std::getline(std::cin, name);
        std::cout << "Enter Problem Link: "; std::getline(std::cin, url);

        if (name.empty() || url.empty()) {
            std::cout << "Error: Name and Link are required.\n";
            return;
        }

        if (is_duplicate(name, url)) {
            std::cout << "Error: A problem with this Name or URL already exists in your Library!\n";
            return;
        }

        int score = 0;
        while (true) {
            std::cout << "Enter Score (1 to 10): ";
            std::getline(std::cin, score_str);
            try {
                score = std::stoi(score_str);
                if (score >= 1 && score <= 10) break;
            } catch (...) {}
            std::cout << "Invalid input. Please enter an integer between 1 and 10.\n";
        }

        std::vector<std::string> tags = load_tags();
        if (tags.empty()) {
            std::cout << "\nNo tags found in database. Creating default 'General' tag...\n";
            std::ofstream tag_out(get_tags_file_path(), std::ios::app);
            tag_out << "General\n";
            tags.push_back("General");
        }

        std::cout << "\nAvailable Tags:\n";
        for (size_t i = 0; i < tags.size(); ++i) {
            std::cout << "  [" << (i + 1) << "] " << tags[i] << '\n';
        }

        std::string tag_input;
        std::string selected_tag = "";
        while (selected_tag.empty()) {
            std::cout << "Choose Tag by Index (1-" << tags.size() << ") or Name: ";
            std::getline(std::cin, tag_input);

            try {
                size_t idx = std::stoul(tag_input);
                if (idx >= 1 && idx <= tags.size()) {
                    selected_tag = tags[idx - 1];
                }
            } catch (...) {
                for (const std::string& t : tags) {
                    if (is_equal_ignore_case(t, tag_input)) {
                        selected_tag = t;
                        break;
                    }
                }
            }

            if (selected_tag.empty()) {
                std::cout << "Invalid selection. Please choose a valid index number or exact tag name.\n";
            }
        }

        std::ofstream file(get_library_file_path(), std::ios::app);
        file << selected_tag << '|' << name << '|' << url << '|' << score << '\n';
        std::cout << "[+] Problem '" << name << "' added under tag [" << selected_tag << "] with score " << score << "/10.\n";
    }

    void list(const std::string& filter_input) {
        std::ifstream file(get_library_file_path());
        if (!file.is_open()) {
            std::cout << "Library database is currently empty.\n";
            return;
        }

        std::vector<Problem> problems;
        std::string line, tag, name, url, score_str;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::getline(ss, tag, '|');
            std::getline(ss, name, '|');
            std::getline(ss, url, '|');
            std::getline(ss, score_str);
            int score = 0;
            try { score = std::stoi(score_str); } catch (...) {}
            problems.push_back({tag, name, url, score});
        }

        if (problems.empty()) {
            std::cout << "No problems tracked in Library.\n";
            return;
        }

        std::vector<std::string> tags = load_tags();
        // Include tags that might exist in problems even if deleted from tags.tsv
        for (const Problem& p : problems) {
            if (std::find(tags.begin(), tags.end(), p.tag) == tags.end()) {
                tags.push_back(p.tag);
            }
        }

        std::string target_filter = filter_input;

        // Interactive menu if no filter was passed via CLI
        if (target_filter.empty()) {
            std::cout << "\nFilter Options:\n";
            std::cout << "  [0] All Tags\n";
            for (size_t i = 0; i < tags.size(); ++i) {
                std::cout << "  [" << (i + 1) << "] " << tags[i] << '\n';
            }
            std::cout << "Enter option number or Tag Name to display: ";
            std::string choice;
            std::getline(std::cin, choice);

            if (choice == "0" || is_equal_ignore_case(choice, "All") || choice.empty()) {
                target_filter = ""; // Show all
            } else {
                try {
                    size_t idx = std::stoul(choice);
                    if (idx >= 1 && idx <= tags.size()) {
                        target_filter = tags[idx - 1];
                    } else {
                        target_filter = choice;
                    }
                } catch (...) {
                    target_filter = choice;
                }
            }
        }

        std::cout << "\n=================================================================================\n";
        std::cout << "                                LIBRARY CATALOG                                  \n";
        std::cout << "=================================================================================\n";

        bool any_printed = false;
        for (size_t t_idx = 0; t_idx < tags.size(); ++t_idx) {
            const std::string& current_tag = tags[t_idx];

            if (!target_filter.empty() && !is_equal_ignore_case(current_tag, target_filter)) {
                continue;
            }

            std::vector<Problem> tag_problems;
            for (const Problem& p : problems) {
                if (is_equal_ignore_case(p.tag, current_tag)) {
                    tag_problems.push_back(p);
                }
            }

            if (tag_problems.empty() && !target_filter.empty()) {
                continue;
            }

            any_printed = true;
            bool is_last_tag = (t_idx == tags.size() - 1) || (!target_filter.empty());
            std::cout << (is_last_tag ? "└── " : "├── ") << "[" << current_tag << "]\n";

            for (size_t p_idx = 0; p_idx < tag_problems.size(); ++p_idx) {
                const Problem& p = tag_problems[p_idx];
                bool is_last_prob = (p_idx == tag_problems.size() - 1);
                
                std::string indent = is_last_tag ? "    " : "│   ";
                std::string branch = is_last_prob ? "└── " : "├── ";

                std::cout << indent << branch 
                          << "(" << p.score << "/10) " 
                          << p.name << "  -->  " << p.url << '\n';
            }
        }

        if (!any_printed) {
            std::cout << "No problems found matching tag filter: [" << target_filter << "]\n";
        }
        std::cout << "=================================================================================\n";
    }

    void remove(const std::string& target) {
        std::string file_path = get_library_file_path();
        std::ifstream file(file_path);
        if (!file.is_open()) {
            std::cout << "Error: Library database not found.\n";
            return;
        }

        std::vector<std::string> remaining_lines;
        std::string line, tag, name, url, score_str;
        bool found = false;

        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::getline(ss, tag, '|');
            std::getline(ss, name, '|');
            std::getline(ss, url, '|');
            std::getline(ss, score_str);

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
            std::cout << "[-] Problem '" << target << "' successfully removed from Library.\n";
        } else {
            std::cout << "Error: No matching problem found in Library.\n";
        }
    }
}
