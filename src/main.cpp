#include "tracker.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Features:\n";
        std::cout << "  Learn_CP add <url>\n";
        std::cout << "  Learn_CP remind <1|2>\n";
        std::cout << "  Learn_CP done <url>\n";

        return 1;
    }

    std::string command = argv[1];

    if (command == "add" && argc == 3)
    {
        add_problem(argv[2]);
    }
    else if (command == "remind" && argc == 3)
    {
        remind(std::stoi(argv[2]));
    }
    else if (command == "done" && argc == 3)
    {
        mark_done(argv[2]);
    }
    else
    {
        std::cout << "Invalid command.\n";
    }

    return 0;
}
