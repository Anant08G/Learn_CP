#ifndef LOG_H
#define LOG_H

#include <string>

namespace Log {
    void add(const std::string& concept_name);
    void list();
    void rename(const std::string& old_name, const std::string& new_name);
    void edit_content(const std::string& concept_name);
    void remove(const std::string& concept_name);
}

#endif // LOG_H
