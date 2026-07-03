#include "tracker.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Features:\n";
        std::cout << "  Learn_CP add\n";
        std::cout << "  Learn_CP list\n";
        std::cout << "  Learn_CP remove <target>\n"; // target can be either name or url

        return 1;
    }

    std::string command = argv[1];

    if (command == "add" && argc == 2) add_problem();

    else if (command == "list" && argc == 2) problem_list();
    
    else if (command == "remove" && argc >= 3){
        std::string target = argv[2];
        for(int i = 3; i < argc; i++){
            target += ' ';
            target += argv[i];
        }
        remove(target);
    }

    else std::cout << "Invalid command.\n";

    return 0;
}
