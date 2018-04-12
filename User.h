

#ifndef STOCKSTALLION_USER_H
#define STOCKSTALLION_USER_H

#include <iostream>
#include <vector>
#include "Portfolio.h"

class User {
    private:
        std::string username;
        std::string password;
    //        string email;
    //        double balance;
        std::vector<Portfolio> portfolios;

    public:
//        User(std::string username, std::string password, std::string email); // , double balance);
        User(std::string username, std::string stockList);
//        void addMoney(double money);
//        void removeMoney(double money);
//        void closePortfolio(Portfolio portfolio);
//        void openPortfolio(Portfolio portfolio);
        /*
        void recoverPassword();
        void recoverUsername();
        */



};



#endif //STOCKSTALLION_USER_H
