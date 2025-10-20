#pragma once

#include <ctime>
#include <sstream>
#include "common.h"

class UriPath {
    private:
        std::string path;
        std::string query_string;
};

class HttpDate {
    public:
        static HttpDate from_string(const std::string& s);
        std::string to_string() const;
    private:
        std::time_t timestamp;
};

class HttpCommonHeader {
    // Content-Length: 512
    std::size_t content_length;
    // Transfer-Encoding: chunked
    std::vector<std::string> transfer_encoding;
    // text/html; charset=UTF-8
    std::string content_type;
};


struct HostHeader {
    std::string address;
    uint16_t port;
    HostHeader() : port(DEFAULT_PORT) {}
};


class HttpRequest : public HttpCommonHeader {
    Method method;
    HostHeader host;
    UriPath request_target;
};

class HttpResponse : public HttpCommonHeader {
    int status_code;
    std::string message;
    HttpDate date;
    std::string location;
};

class HttpParser {
    public:
        void http_request_parse();
        void http_response_parse();
    private:
        HttpRequest request;
        HttpResponse response;
};
