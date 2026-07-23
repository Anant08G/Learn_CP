#include "log.h"
#include "common.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iomanip>

namespace Log {

    std::filesystem::path get_logs_dir() {
        std::filesystem::path logs_dir = get_app_dir() / "logs";
        std::filesystem::create_directories(logs_dir);
        return logs_dir;
    }

    // Helper: Safely converts "Binary Search" to "Binary_Search"
    std::string normalize_name(const std::string& name) {
        std::string clean = name;
        std::replace(clean.begin(), clean.end(), ' ', '_');
        return clean;
    }

    // Helper: Checks if a concept file already exists (case-insensitive)
    bool concept_exists(const std::string& target_name) {
        std::string clean_target = normalize_name(target_name);
        for (const auto& entry : std::filesystem::directory_iterator(get_logs_dir())) {
            if (entry.is_regular_file() && entry.path().extension() == ".md") {
                std::string existing_stem = entry.path().stem().string();
                if (is_equal_ignore_case(existing_stem, clean_target)) {
                    return true;
                }
            }
        }
        return false;
    }

    std::string get_system_editor() {
        const char* visual = std::getenv("VISUAL");
        if (visual != nullptr && std::string(visual) != "") {
            return std::string(visual);
        }
        const char* editor = std::getenv("EDITOR");
        if (editor != nullptr && std::string(editor) != "") {
            return std::string(editor);
        }
#if defined(_WIN32)
        return "notepad";
#else
        return "vi";
#endif
    }

    // Requirement 2: Add Concept (with strict duplicate prevention)
    void add(const std::string& concept_name) {
        if (concept_name.empty()) {
            std::cout << "Error: Concept name cannot be empty.\n";
            return;
        }

        if (concept_exists(concept_name)) {
            std::cout << "Error: A concept log named '" << concept_name << "' already exists!\n";
            return;
        }

        std::string safe_name = normalize_name(concept_name);
        std::filesystem::path file_path = get_logs_dir() / (safe_name + ".md");

        // Create the scaffolded Markdown file
        std::ofstream seed_file(file_path);
        seed_file << "# Concept: " << concept_name << "\n\n";
        seed_file << "## Key Intuition\n\n\n";
        seed_file << "## C++ Template / Algorithm\n```cpp\n// Write template code here\n```\n";
        seed_file.close();

        std::cout << "[+] Created new concept log: " << safe_name << ".md\n";
        
        // Immediately launch editor so you can start typing
        edit_content(safe_name);
    }

    // Requirement 1: List all concepts
    void list() {
        std::filesystem::path logs_dir = get_logs_dir();

        std::cout << "\n=================================================================================\n";
        std::cout << std::left << std::setw(6) << "No." 
                  << std::setw(40) << "CP Concept Log" 
                  << "Last Modified\n";
        std::cout << "=================================================================================\n";

        int index = 1;
        bool found = false;

        for (const auto& entry : std::filesystem::directory_iterator(logs_dir)) {
            if (entry.is_regular_file() && entry.path().extension() == ".md") {
                found = true;

                auto ftime = std::filesystem::last_write_time(entry.path());
                auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                    ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
                std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

                std::string time_str = std::ctime(&cftime);
                time_str.pop_back(); // Remove trailing newline from ctime

                std::cout << std::left 
                          << std::setw(6) << index++
                          << std::setw(40) << entry.path().stem().string()
                          << time_str << "\n";
            }
        }

        if (!found) {
            std::cout << "No concept logs found. Use 'Learn_CP log add <concept>' to create one.\n";
        }
        std::cout << "=================================================================================\n";
    }

    // Requirement 3: Edit Concept Name (Rename)
    void rename(const std::string& old_name, const std::string& new_name) {
        if (old_name.empty() || new_name.empty()) {
            std::cout << "Error: Both current name and new name are required.\n";
            return;
        }

        if (!concept_exists(old_name)) {
            std::cout << "Error: Concept log '" << old_name << "' does not exist.\n";
            return;
        }

        if (concept_exists(new_name)) {
            std::cout << "Error: Cannot rename. A concept named '" << new_name << "' already exists!\n";
            return;
        }

        std::string safe_old = normalize_name(old_name);
        std::string safe_new = normalize_name(new_name);

        std::filesystem::path old_path = get_logs_dir() / (safe_old + ".md");
        std::filesystem::path new_path = get_logs_dir() / (safe_new + ".md");

        try {
            std::filesystem::rename(old_path, new_path);
            std::cout << "[+] Successfully renamed concept to '" << safe_new << ".md'.\n";
        } catch (const std::exception& e) {
            std::cout << "Error renaming file: " << e.what() << "\n";
        }
    }

    // Opens existing concept in UNIX text editor (vi/nano/nvim)
    void edit_content(const std::string& concept_name) {
        if (!concept_exists(concept_name)) {
            std::cout << "Error: Concept '" << concept_name << "' not found. Use 'add' first.\n";
            return;
        }

        std::string safe_name = normalize_name(concept_name);
        std::filesystem::path file_path = get_logs_dir() / (safe_name + ".md");

        std::string editor = get_system_editor();
        std::string command = editor + " \"" + file_path.string() + "\"";

        std::cout << "[*] Launching " << editor << "...\n";
        int result = std::system(command.c_str());

        if (result == 0) {
            std::cout << "[+] Saved changes to " << safe_name << ".md\n";
        } else {
            std::cout << "[!] Editor exited with an error status.\n";
        }
    }

    // Requirement 4: Remove
    void remove(const std::string& concept_name) {
        if (concept_name.empty()) {
            std::cout << "Error: You must specify the concept name to remove.\n";
            return;
        }

        std::string safe_name = normalize_name(concept_name);
        std::filesystem::path file_path = get_logs_dir() / (safe_name + ".md");

        if (std::filesystem::remove(file_path)) {
            std::cout << "[-] Concept log '" << safe_name << ".md' deleted successfully.\n";
        } else {
            std::cout << "Error: Concept log '" << safe_name << ".md' not found.\n";
        }
    }
}
