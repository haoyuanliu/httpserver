#include "httpRequest.h"

httpRequest::httpRequest(const char *buf) {
    std::istringstream iss(buf);
    std::string method;
    iss >> method >> uri_ >> version_;
    setMethod(method);
    setPath("../content" + uri_);
    std::string header;
    while(getline(iss, header)) {
        Util::trim(header);
        Util::split(header, ":", headers_);
    }
}

std::string httpRequest::toString() {
    std::string res = "";
    res += getStrMethod() + " ";
    res += getPath() + " ";
    res += getVersion() + "\r\n";
    std::map<std::string, std::string> headers;
    std::map<std::string, std::string>::iterator iter;
    headers = getMapHeaders();
    for(iter = headers.begin(); iter != headers.end(); ++iter) {
        res += iter->first + ":" + iter->second + "\r\n";
    }
    return res;
}
