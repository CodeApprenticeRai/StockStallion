//
// Created by tareg on 4/12/2018.
//

#include "User.h"
#include "Portfolio.h"
#include <iostream>

User::User(std::string username, std::string stockList) // std::string email; // double balance)
{
    this->username = username;

    Portfolio *p = new Portfolio(stockList);
//    this->portfolios.push_back(p);
}

//    this->password = password;
//    this->email = email;
//    this->balance = balance;


