#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>

namespace Library {
    void add_tag();
    void add_problem();
    void list(const std::string& filter_input = "");
    void remove(const std::string& target);
}

#endif // LIBRARY_H
