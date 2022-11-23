#ifndef SEARCHER_H
#define SEARCHER_H

#include "thread_searcher.h"
class Searcher{
    private:
        int id;
    public:
        Searcher(int id): id(id){};
        void operator()();
        Request* retrieve_from_queue();
        void wait_for_results(std::vector<thread_searcher> &v_objetos, int client_id, client_type type, std::string word, int& balance);
        std::string set_result(std::vector<thread_searcher> &v_objetos, int client_id, client_type type, std::string word, double time);
};  

#endif
