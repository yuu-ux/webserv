#include "HttpParser.h"

class ClientHandler {
    public:
        void onReadable();
        void onWritable();
    private:
        HttpParser parser;
        HttpRequest request;
        HttpResponse response;
};
