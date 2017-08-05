#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include "TimeStamp.h"

class Util{
    public:
        Util();
        ~Util();
    public:
        template<typename T> static std::string transToString(const T &input) {
            std::stringstream ss;
            std::string output;
            ss << input;
            ss >> output;
            return output;
        }
        static void trim(std::string &str);
        static void split(std::string str, std::string pattern, std::map<std::string, std::string> &m);
        static void toUpper(std::string &str);
};
