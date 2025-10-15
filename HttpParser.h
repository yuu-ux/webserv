#pragma once

#include <ctime>
#include <sstream>
#include "common.h"

// TODO Location ヘッダー追加
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


// TODO HttpConfig でも同じような構造を定義してるからまとめても良さそう
struct HostHeader {
    std::string address;
    uint16_t port;
};


class HttpRequest : public HttpCommonHeader {
    Method::Method method;
    HostHeader host;
};

class HttpResponse : public HttpCommonHeader {
    HttpDate date;
    int status_code;
    std::string message;
};

class HttpParser {
    public:
        void http_request_parse();
        void http_response_parse();
    private:
        HttpRequest request;
        HttpResponse response;
};
