#include "Util.h"

void Util::trim(std::string &str) {
    int index = 0;
    if(!str.empty()) {
        while( (index = str.find(' ')) != std::string::npos) {
            str.erase(index, 1);
        }
    }
}


void Util::split(std::string str, std::string pattern, std::map<std::string, std::string> &m) {
    std::string::size_type pos;
    pos = str.find(pattern);
    if(pos != std::string::npos) {
        std::string key = str.substr(0, pos);
        std::string value = str.substr(pos+pattern.size(), str.size()-pos-pattern.size());
        m[key] = value;
    }
}

void Util::toUpper(std::string &str) {
    if (str.empty()) return;
    std::string::iterator it;
    for (it = str.begin(); it != str.end(); ++it) {
        *it = toupper(*it);
    }
}
