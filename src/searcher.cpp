/* Code realized by:
    - FRANCISCO JESÚS DÍAZ PELLEJERO
    - JAVIER VILLAR ASENSIO
    -MIGUEL DE LAS HERAS FUENTES
*/

#include <iostream>
#include <thread>
#include <queue>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include <mutex>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <condition_variable>
#include <atomic>
#include <memory>

#include "request.h"
#include "colors.h"
#include "searcher.h"

/*GLOBAL VARIABLES*/
std::string colours[] = {BOLDBLUE, BOLDGREEN, BOLDYELLOW, BOLDMAGENTA};
extern std::vector<std::string> files;

/*EXTERN VARIABLES*/
extern std::queue<Request> premium_requests;
extern std::queue<Request> normal_requests;
extern std::condition_variable condition;
extern std::mutex sem;
extern std::mutex sem_premium;
extern std::mutex sem_normal;
extern std::atomic<int> occupied_threads;

void Searcher::operator()(){

    while(1){
        std::vector<thread_searcher> v_objetos;
        Request *req;
        std::string results = "";
        req = retrieve_from_queue();

        int client_id = req->getClientID();
        client_type type = req->getType();
        std::string word = req->getWord();

        unsigned t0 = clock();

        wait_for_results(std::ref(v_objetos), client_id, type, word, req->get_balance());

        unsigned t1 = clock();

        results = results + set_result(std::ref(v_objetos), client_id, type, word, (double(t1-t0) / CLOCKS_PER_SEC));

        req->set_promise_value(results);
        
        occupied_threads = occupied_threads + 1;
        std::cout << BOLDBLUE << "[SEARCHER " << id << "]: " << "Finished request of client " << 
        std::to_string(client_id) << ". Free searchers: " << occupied_threads << RESET << std::endl;
    }
}

std::string Searcher::set_result(std::vector<thread_searcher> &v_objetos, int client_id, client_type type, std::string word, double time){
    std::string results = "[Client id " + std::to_string(client_id) + "] with type " + std::to_string(type) + ". 0(Free account), 1(Limited premium), 2(Unlimited_premium)\n";
    results = results + "Time of execution in seconds: " + std::to_string(time) + " \nResults for: " + BOLDYELLOW + word + RESET + "\n";
    for(long unsigned int i = 0; i < v_objetos.size(); i++){
        results += v_objetos[i].to_string();
    }
    return results;
}

void Searcher::wait_for_results(std::vector<thread_searcher> &v_objetos, int client_id, client_type type, std::string word, int& balance){
    
    std::mutex balance_sync;
    std::vector<std::thread> v_hilos;

    for (long unsigned i = 0; i < files.size(); i++)
        {
            thread_searcher s{(int)(i+1),files[i], word, colours[i % 4], std::ref(balance),
            type, client_id, std::ref(balance_sync)};
            v_objetos.push_back(s);
        }

        for (long unsigned int i = 0; i < files.size(); i++){
            v_hilos.push_back(std::thread(std::ref(v_objetos[i])));
        }

        //wait until all threads are finished
        std::for_each(v_hilos.begin(),v_hilos.end(),std::mem_fn(&std::thread::join));
}


Request* Searcher::retrieve_from_queue(){
    std::unique_lock<std::mutex>queue_size(sem);
    Request *req;
    condition.wait(queue_size, [&]{return !premium_requests.empty() || !normal_requests.empty();});
    occupied_threads = occupied_threads - 1;
    int random_num = (rand() % 10) + 1;

        if(premium_requests.empty() && !normal_requests.empty()){
            sem_normal.lock();
            req = new Request(std::ref(normal_requests.front()));
            normal_requests.pop();
            sem_normal.unlock();
        }

        else if(!premium_requests.empty() && normal_requests.empty()){
            sem_premium.lock();
            req = new Request(std::ref(premium_requests.front()));
            premium_requests.pop();
            sem_premium.unlock();
        }

        else if(!premium_requests.empty() && !normal_requests.empty() && random_num >= 1 && random_num <= 8){
            sem_premium.lock();
            req = new Request(std::ref(premium_requests.front()));
            premium_requests.pop();
            sem_premium.unlock();
        }
        
        else{
            sem_normal.lock();
            std::cout << "[SEARCHER " << id << "]: Retrieving request for normal requests queue" << std::endl;
            req = new Request(std::ref(normal_requests.front()));
            normal_requests.pop();
            sem_normal.unlock();
        }

        std::cout << BOLDBLUE << "[SEARCHER " << id << "]: Element retreived from queue: " << req->to_string() << RESET << std::endl;

        queue_size.unlock();
        return req;
}

