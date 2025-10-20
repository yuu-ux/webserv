// サーバークラス

#include "common.h"
#include "Router.h"

class EventLoop {
    public:
        void add(int fd, uint32_t events, void* user);
        void mod(int fd, uint32_t events, void* user);
        void del(int fd);
        void run();
};

struct Socket {
    int fd;
    static Socket listen_tcp(const std::string& host, uint16_t port, int backlog);
    static Socket accept_nonblock(int listen_fd);
    static void set_nonblock(int fd);
    static void set_reuseaddr(int fd);
    static void close(int fd);
    Socket() : fd(-1) {};
};

class Server {
    public:
        Server(std::string host, uint16_t port, EventLoop& loop, Router& router);
        void start();
    private:
        EventLoop& loop_;
        Router& route_;
        int listen_fd_;
        static void on_acceptable(int fd, uint32_t events, void* self);
};

