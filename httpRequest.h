#include <map>
#include "TimeStamp.h"

class httpRequest {
    public:
        enum Method {
            kInvalid, kGet, kPost, kHead, kPut, kDelete
        };
        enum Version {
            kUnkonwn, kHttp10, kHttp11
        };

    private:
        Method method_;
        std::string path_;
        Version version_;
        TimeStamp revc_time_;
        std::map<std::string, std::string> headers_;
        std::string query_;
}
