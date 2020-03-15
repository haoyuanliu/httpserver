#include <map>
#include <sstream>
#include "Util.h"

class httpRequest {
    public:
        enum Method {
            kInvalid, kGet, kPost, kHead, kPut, kDelete
        };

        httpRequest() : method_(kInvalid) {}
        httpRequest(const char *buf);

        void setUri(std::string uri) {
            uri_ = uri;
        }

        std::string getUri() const {
            return uri_;
        }

        void setVersion(std::string version) {
            version_ =  version;
        }

        std::string getVersion() const {
            return version_;
        }

        bool setMethod(std::string &method) {
            if ("GET" == method) {
                method_ = kGet;
            } else if ("POST" == method) {
                method_ = kPost;
            } else if ("HEAD" == method) {
                method_ = kHead;
            } else if ("PUT" == method) {
                method_ = kPut;
            } else if ("DELETE" == method) {
                method_ = kDelete;
            } else {
                method_ = kInvalid;
            }
            return method_ != kInvalid;
        }

        Method getMethod() const {
            return method_;
        }

        std::string getStrMethod() const {
            std::string res;
            switch(method_) {
                case kGet:
                    res = "GET";
                    break;
                case kPost:
                    res = "POST";
                    break;
                case kHead:
                    res = "HEAD";
                    break;
                case kPut:
                    res = "PUT";
                    break;
                case kDelete:
                    res = "DELETE";
                    break;
                default:
                    res = "UNKNOWN";
                    break;
            }
            return res;
        }

        void setPath(std::string path) {
            if ("./content/" == path)
                path = "../content/index.html";
            path_ = path;
        }

        const std::string getPath() {
            return path_;
        }

        void setQuery(std::string query) {
            query_ = query;
        }

        const std::string getQuery() {
            return query_;
        }

        void setRecvTime(TimeStamp time) {
            recv_time_ = time;
        }

        TimeStamp getRecvTime() {
            return recv_time_;
        }

        void addHeaders(const std::string key, const std::string value) {
            headers_[key] = value;
        }

        std::string getHeader(const std::string key) {
            std::string res;
            std::map<std::string, std::string>::iterator it = headers_.find(key);
            if (it != headers_.end()) {
                res = it->second;
            }
            return res;
        }

        std::map<std::string, std::string> getMapHeaders() {
            return headers_;
        }

        std::string toString();

    private:
        Method method_;
        std::string uri_;
        std::string path_;
        std::string version_;
        std::map<std::string, std::string> headers_;
        std::string query_;
        TimeStamp recv_time_;
};
