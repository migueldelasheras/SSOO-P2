/*Code ralized by:
    - FRANCISCO JESÚS DÍAZ PELLEJERO
    - JAVIER VILLAR ASENSIO
    - MIGUEL DE LAS HERAS FUENTES
*/

#include <iostream>
#include <queue>
#include <fstream>
#include <atomic>
#include <tuple>
#include <vector>

#include "client.h"
#include "request.h"
#include "colors.h"

#define MAX_CLIENTS 50

extern std::vector<std::string> dictionary;
extern std::queue<Request> premium_requests;
extern std::queue<Request> normal_requests;
extern std::condition_variable condition;
extern std::mutex sem_premium;
extern std::mutex sem_normal;
extern std::atomic<int> occupied_threads;
extern std::mutex notifications;
extern std::vector<std::tuple<int, int&>> balance_vec;
extern std::mutex sem_vec_tup;
extern int *balances[MAX_CLIENTS];

std::string Client::make_search(){
    /*sem_vec_tup.lock();
    std::tuple<int, int&> t = std::make_tuple(id, std::ref(balance));
    balance_vec.push_back(t);
    sem_vec_tup.unlock();*/
    balances[id] = &balance;
    std::promise<std::string> prom;
    std::future<std::string> fut;
    fut = prom.get_future();
    Request req{std::ref(balance), id, type, choose_word(), std::ref(prom)};
    req.to_string();

    if(type == free_acc) {
        sem_normal.lock();
        normal_requests.push(req);
        sem_normal.unlock();
    }
    else {
        sem_premium.lock();
        premium_requests.push(req);
        sem_premium.unlock();
    }

    notifications.lock();
    while(occupied_threads == 0);
    
    condition.notify_one();
    notifications.unlock();
    
    std::string result = fut.get();

    return result;
}

std::string Client::choose_word(){
    int random_num = rand() % dictionary.size();
    return dictionary[random_num];
}

void Client::log_result(std::string results){
    std::string filename = LOG_DIR + std::to_string(id) + ".txt";
    std::ofstream out(filename);
    out << results;
}

void Client::operator()(){
   log_result(make_search());
   std::cout << "[Client id " << id << "]: finished search" << std::endl;
};

