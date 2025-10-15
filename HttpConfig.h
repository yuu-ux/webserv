#pragma once

#include <cstddef>
#include <cstdint>
#include "common.h"

// GET・HEAD 以外は GET に変換される
typedef struct ErrorPageDirective {
    std::vector<int> statuses;
    std::string target;
    // 初期化子リストで初期化
    int override_status;
    ErrorPageDirective(): override_status(-1) {}
} ErrorPageDirective;

typedef struct ReturnDirective {
    int status;
    std::string target;

} ReturnDirective;

struct AddressRule {
    std::string cidr_or_host;
};

struct LimitExceptDirective {
    std::vector<Method> method;
    unsigned int allowed_methods_mask;
    // all を指定すると全て拒否, 基本的には address が入る
    std::vector<AddressRule> allow;
    std::vector<AddressRule> deny;

    LimitExceptDirective() : allowed_methods_mask(0) {}
};


// localhost も実装コストほぼ 0 だから対応できると良い
// localhost:8080 とか
// 127.0.0.1 に置換して格納する
typedef struct ListenDirective {
    std::string address;
    // nginx 準拠
    // range: 0 ~ 65535
    // 初期化子リストで初期化
    uint16_t port;
    ListenDirective() : port(80) {}
} ListenDirective;

class CommonConfig {
    private:
        // 値が 0 の場合制限なしを意味する
        off_t client_max_body_size;
        ErrorPageDirective error_page;
        // return が予約後のため prefix に _ をつける
        ReturnDirective redirect;
        std::string root;
        // 初期化子リストで初期化
        bool autoindex;
        std::vector<std::string> index_files;
        std::string upload_store;

        CommonConfig() : client_max_body_size(0), autoindex(false) {}
};

class LocationConfig : public CommonConfig {
    private:
        LimitExceptDirective limit_except;
        std::string cgi_path;
        std::string cgi_extension;
        std::string match;
};

class ServerConfig : public CommonConfig {
    private:
        std::vector<LocationConfig> locations;
        ListenDirective listens;
        std::vector<std::string> server_name;
};


class HttpConfig {
    private:
        std::vector<ServerConfig> servers;
        CommonConfig defaults;
};
