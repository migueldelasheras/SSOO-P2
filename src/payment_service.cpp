/* Code realized by:
    - FRANCISCO JESÚS DÍAZ PELLEJERO
    - JAVIER VILLAR ASENSIO
    -MIGUEL DE LAS HERAS FUENTES
*/

#include <vector>
#include <mutex>
#include <iostream>
#include <array>

#include "payment_service.h"
#include "client.h"
#include "colors.h"

#define MAX_CLIENTS 50

extern int id_send;
extern std::mutex payment_sem;
extern std::mutex client_sem;
extern std::vector<std::tuple<int, int&>> balance_vec;
extern int *balances[MAX_CLIENTS];

void payment_service::update_balance(){
    /*for (std::tuple<int, int&>tuple_id_balance : balance_vec){
        if(std::get<0>(tuple_id_balance) == id_send){
            std::get<1>(tuple_id_balance) += 50;
            break;
        }
    }*/
    *balances[id_send] += 50;
    std::cout << BOLDGREEN << "[PAYMENT] Salary updated of client "<< id_send << RESET << std::endl;
}

void payment_service::operator()(){
    payment_sem.lock();
    while(1){  
        payment_sem.lock();
        update_balance();
        client_sem.unlock();
        
    }
}