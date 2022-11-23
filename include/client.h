#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <future>

#define LOG_DIR "Results/"

enum client_type {free_acc, limited_prem, unlimited_prem};

class Client{
    private:
        int id;
        client_type type;
        int balance;
    public:
        Client(int id, client_type type, int balance): id(id), type(type), balance(balance){};
        std::string make_search();
        void operator()();
        std::string choose_word();
        void log_result(std::string result);
};

#endif
