#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <iomanip>

#include "queue.hpp"
#include "library.hpp"
#include "log.hpp"


// -----------------------------------------------------------------------------
// Helper: Case-Insensitive Lowercase Conversion
// -----------------------------------------------------------------------------

std::string to_lowercase(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return str;
}

// -----------------------------------------------------------------------------
// Helper: REPL Input Tokenizer (Splits "remove Aggressive Cows" -> cmd & param)
// -----------------------------------------------------------------------------

void parse_input(const std::string& raw_input, std::string& cmd, std::string& param) {
    size_t start = raw_input.find_first_not_of(" \t");
    if (start == std::string::npos) {
        cmd = "";
        param = "";
        return;
    }
    std::string input = raw_input.substr(start);
    size_t first_space = input.find_first_of(" \t");
    if (first_space == std::string::npos) {
        cmd = to_lowercase(input);
        param = "";
    } else {
        cmd = to_lowercase(input.substr(0, first_space));
        size_t param_start = input.find_first_not_of(" \t", first_space);
        if (param_start != std::string::npos) {
            param = input.substr(param_start); // Preserves exact capitalization for targets!
        } else {
            param = "";
        }
    }
}

// -----------------------------------------------------------------------------
// UI Banners & On-Demand Command Tree
// -----------------------------------------------------------------------------

void print_welcome_banner() {
    std::cout<<"\n============================================================================\n";
    std::cout << "Direct CLI Usage:  Learn_CP <workspace> <command> [args]  (Case-Insensitive)\n";
    std::cout << "============================================================================\n\n";

}

void print_command_tree() {
    std::cout << "\n=================================================================================\n";
    std::cout << "                        Learn_CP - Complete Command Tree                         \n";
    std::cout << "=================================================================================\n";
    std::cout << "Learn_CP\n";
    std::cout << "├── queue                   (Active Training Backlog)\n";
    std::cout << "│   ├── add                 Add a new problem to the active queue\n";
    std::cout << "│   ├── list                Display all queued problems and elapsed times\n";
    std::cout << "│   └── remove <target>     Remove a problem by name or URL\n";
    std::cout << "│\n";
    std::cout << "├── library                 (Categorized Problem Catalog)\n";
    std::cout << "│   ├── add-tag             Create a new category tag\n";
    std::cout << "│   ├── add                 Save a problem under a category\n";
    std::cout << "│   ├── list [tag]          Display catalog as an ASCII tree (or filter by tag)\n";
    std::cout << "│   └── remove <target>     Delete a problem by name or URL\n";
    std::cout << "│\n";
    std::cout << "└── log                     (Algorithmic Notes & Templates)\n";
    std::cout << "    ├── list                List all saved Markdown concept notes\n";
    std::cout << "    ├── add <name>          Create a new note and launch text editor\n";
    std::cout << "    ├── open <name>         Open an existing note in your text editor\n";
    std::cout << "    ├── rename <old> <new>  Rename a concept file on disk\n";
    std::cout << "    └── remove <name>       Delete a concept note permanently\n";
    std::cout << "=================================================================================\n";
}

void print_queue_help() {
    std::cout << "\n── [Queue Commands] ─────────────────────────────────────────────────────────────\n";
    std::cout << "  Learn_CP queue             Open interactive Queue REPL\n";
    std::cout << "  Learn_CP queue add         Add a new problem to the active queue\n";
    std::cout << "  Learn_CP queue list        Display all queued problems and elapsed times\n";
    std::cout << "  Learn_CP queue remove <id> Remove a problem by name or URL\n\n";
}

void print_library_help() {
    std::cout << "\n── [Library Commands] ───────────────────────────────────────────────────────────\n";
    std::cout << "  Learn_CP library               Open interactive Library REPL\n";
    std::cout << "  Learn_CP library add-tag       Create a new category tag\n";
    std::cout << "  Learn_CP library add           Save a problem under a category\n";
    std::cout << "  Learn_CP library list [tag]    Display catalog as an ASCII tree (or filter by tag)\n";
    std::cout << "  Learn_CP library remove <id>   Delete a problem by name or URL\n\n";
}

void print_log_help() {
    std::cout << "\n── [Log Commands] ───────────────────────────────────────────────────────────────\n";
    std::cout << "  Learn_CP log                   Open interactive Concept Log REPL\n";
    std::cout << "  Learn_CP log list              List all saved Markdown concept notes\n";
    std::cout << "  Learn_CP log add <name>        Create a new note and launch text editor\n";
    std::cout << "  Learn_CP log open <name>       Open an existing note in your text editor\n";
    std::cout << "  Learn_CP log rename <old> <new> Rename a concept file on disk\n";
    std::cout << "  Learn_CP log remove <name>     Delete a concept note permanently\n\n";
}

// -----------------------------------------------------------------------------
// Minimalist Workspace REPL Menus
// -----------------------------------------------------------------------------

void run_interactive_queue_menu() {
    std::cout << "\n── [Queue] ── commands: add | list | remove | exit (type 'help' for info)\n";
    while (true) {
        std::cout << "Queue > ";
        
        std::string raw_input, cmd, param;
        std::getline(std::cin, raw_input);
        parse_input(raw_input, cmd, param);

        if (cmd == "0" || cmd == "exit" || cmd == "quit" || cmd == "back" || cmd == "cd ..") {
            break;
        } else if (cmd == "?" || cmd == "help") {
            std::cout << "  commands: add, list, remove <target>, exit\n";
        } else if (cmd == "1" || cmd == "add") {
            Queue::add();
        } else if (cmd == "2" || cmd == "list" || cmd == "ls") {
            Queue::list();
        } else if (cmd == "3" || cmd == "remove" || cmd == "rm" || cmd == "delete" || cmd == "del") {
            if (param.empty()) {
                std::cout << "Enter Problem Name or URL to remove: ";
                std::getline(std::cin, param);
            }
            if (!param.empty()) {
                Queue::remove(param);
            }
        } else if (!cmd.empty()) {
            std::cout << "Invalid command: '" << cmd << "'. Try 'add', 'list', 'remove <target>', or 'exit'.\n";
        }
    }
}

void run_interactive_library_menu() {
    std::cout << "\n── [Library] ── commands: add-tag | add | list | remove | exit (type 'help' for info)\n";
    while (true) {
        std::cout << "Library > ";
        
        std::string raw_input, cmd, param;
        std::getline(std::cin, raw_input);
        parse_input(raw_input, cmd, param);

        if (cmd == "0" || cmd == "exit" || cmd == "quit" || cmd == "back" || cmd == "cd ..") {
            break;
        } else if (cmd == "?" || cmd == "help") {
            std::cout << "  commands: add-tag, add, list [tag], remove <target>, exit\n";
        } else if (cmd == "1" || cmd == "add-tag" || cmd == "tag" || cmd == "new-tag") {
            Library::add_tag();
        } else if (cmd == "2" || cmd == "add") {
            Library::add_problem();
        } else if (cmd == "3" || cmd == "list" || cmd == "ls" || cmd == "tree") {
            Library::list(param);
        } else if (cmd == "4" || cmd == "remove" || cmd == "rm" || cmd == "delete" || cmd == "del") {
            if (param.empty()) {
                std::cout << "Enter Problem Name or URL to remove: ";
                std::getline(std::cin, param);
            }
            if (!param.empty()) {
                Library::remove(param);
            }
        } else if (!cmd.empty()) {
            std::cout << "Invalid command: '" << cmd << "'. Try 'add', 'list [tag]', 'remove <target>', or 'exit'.\n";
        }
    }
}

void run_interactive_log_menu() {
    std::cout << "\n── [Log] ── commands: list | add | open | rename | remove | exit (type 'help' for info)\n";
    while (true) {
        std::cout << "Log > ";
        
        std::string raw_input, cmd, param;
        std::getline(std::cin, raw_input);
        parse_input(raw_input, cmd, param);

        if (cmd == "0" || cmd == "exit" || cmd == "quit" || cmd == "back" || cmd == "cd ..") {
            break;
        } else if (cmd == "?" || cmd == "help") {
            std::cout << "  commands: list, add <name>, open <name>, rename <old> <new>, remove <name>, exit\n";
        } else if (cmd == "1" || cmd == "list" || cmd == "ls") {
            Log::list();
        } else if (cmd == "2" || cmd == "add" || cmd == "create" || cmd == "new") {
            if (param.empty()) {
                std::cout << "Enter Concept Name to create: ";
                std::getline(std::cin, param);
            }
            if (!param.empty()) {
                Log::add(param);
            }
        } else if (cmd == "3" || cmd == "open" || cmd == "edit" || cmd == "vim" || cmd == "nvim") {
            if (param.empty()) {
                std::cout << "Enter Concept Name to edit: ";
                std::getline(std::cin, param);
            }
            if (!param.empty()) {
                Log::edit_content(param);
            }
        } else if (cmd == "4" || cmd == "rename" || cmd == "mv") {
            std::string old_name, new_name;
            std::cout << "Enter Current Concept Name: ";
            std::getline(std::cin, old_name);
            std::cout << "Enter New Concept Name: ";
            std::getline(std::cin, new_name);
            if (!old_name.empty() && !new_name.empty()) {
                Log::rename(old_name, new_name);
            }
        } else if (cmd == "5" || cmd == "remove" || cmd == "rm" || cmd == "delete" || cmd == "del") {
            if (param.empty()) {
                std::cout << "Enter Concept Name to remove: ";
                std::getline(std::cin, param);
            }
            if (!param.empty()) {
                Log::remove(param);
            }
        } else if (!cmd.empty()) {
            std::cout << "Invalid command: '" << cmd << "'. Try 'list', 'add <name>', 'open <name>', or 'exit'.\n";
        }
    }
}

// -----------------------------------------------------------------------------
// Interactive Root Hub Menu (Clean Startup without Clutter)
// -----------------------------------------------------------------------------

void run_root_interactive_hub() {
    print_welcome_banner();
    std::cout << "── [Hub] ── workspaces: queue | library | log | view command tree | exit\n";

    while (true) {
        std::cout << "Learn_CP > ";
        
        std::string raw_input, cmd, param;
        std::getline(std::cin, raw_input);
        parse_input(raw_input, cmd, param);

        if (cmd == "0" || cmd == "exit" || cmd == "quit" || cmd == "q") {
            break;
        } else if (cmd == "?" || cmd == "help" || cmd == "tree" || cmd == "cmds") {
            print_command_tree();
        } else if (cmd == "1" || cmd == "queue" || cmd == "q_workspace") {
            run_interactive_queue_menu();
            std::cout << "\n── [Hub] ── workspaces: queue | library | log | tree (view commands) | exit\n";
        } else if (cmd == "2" || cmd == "library" || cmd == "lib" || cmd == "l") {
            run_interactive_library_menu();
            std::cout << "\n── [Hub] ── workspaces: queue | library | log | tree (view commands) | exit\n";
        } else if (cmd == "3" || cmd == "log" || cmd == "logs" || cmd == "notes") {
            run_interactive_log_menu();
            std::cout << "\n── [Hub] ── workspaces: queue | library | log | tree (view commands) | exit\n";
        } else if (!cmd.empty()) {
            std::cout << "Invalid workspace: '" << cmd << "'. Type 'queue', 'library', 'log', 'tree', or 'exit'.\n";
        }
    }
}

// -----------------------------------------------------------------------------
// Main Application Routing
// -----------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    if (argc < 2) {
        run_root_interactive_hub();
        return 0;
    }

    std::string workspace = to_lowercase(argv[1]);

    // Check for global help/tree flags from Bash
    if (workspace == "--help" || workspace == "-h" || workspace == "help" || workspace == "tree") {
        print_command_tree();
        return 0;
    }

    if (argc == 2) {
        if (workspace == "queue") { run_interactive_queue_menu(); return 0; }
        if (workspace == "library") { run_interactive_library_menu(); return 0; }
        if (workspace == "log") { run_interactive_log_menu(); return 0; }
        
        std::cout << "Unknown workspace: '" << argv[1] << "'\n";
        print_welcome_banner();
        return 1;
    }

    std::string command = to_lowercase(argv[2]);
    
    if (command == "--help" || command == "-h") {
        if (workspace == "queue") { print_queue_help(); return 0; }
        if (workspace == "library") { print_library_help(); return 0; }
        if (workspace == "log") { print_log_help(); return 0; }
    }

    std::string param = "";
    if (argc >= 4) {
        param = argv[3];
        for (int i = 4; i < argc; i++) {
            param += " ";
            param += argv[i];
        }
    }

    if (workspace == "queue") {
        if (command == "add") { Queue::add(); }
        else if (command == "list" || command == "ls") { Queue::list(); }
        else if ((command == "remove" || command == "rm" || command == "del") && !param.empty()) { Queue::remove(param); }
        else { std::cout << "Invalid command for Queue workspace.\n"; print_queue_help(); return 1; }
    } 
    else if (workspace == "library") {
        if (command == "add-tag" || command == "tag" || command == "new-tag") { Library::add_tag(); }
        else if (command == "add") { Library::add_problem(); }
        else if (command == "list" || command == "ls" || command == "tree") { Library::list(param); }
        else if ((command == "remove" || command == "rm" || command == "del") && !param.empty()) { Library::remove(param); }
        else { std::cout << "Invalid command for Library workspace.\n"; print_library_help(); return 1; }
    } 
    else if (workspace == "log") {
        if ((command == "add" || command == "create" || command == "new") && !param.empty()) { Log::add(param); }
        else if (command == "list" || command == "ls") { Log::list(); }
        else if ((command == "open" || command == "edit" || command == "vim" || command == "nvim") && !param.empty()) { Log::edit_content(param); }
        else if ((command == "rename" || command == "mv") && argc >= 5) { Log::rename(argv[3], argv[4]); }
        else if ((command == "remove" || command == "rm" || command == "del") && !param.empty()) { Log::remove(param); }
        else { std::cout << "Invalid command for Log workspace.\n"; print_log_help(); return 1; }
    } 
    else {
        std::cout << "Unknown workspace: '" << argv[1] << "'\n";
        print_welcome_banner();
        return 1;
    }

    return 0;
}
