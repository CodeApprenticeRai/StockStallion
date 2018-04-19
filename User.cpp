//
// Created by tareg on 4/12/2018.
//

#include "User.h"
#include "Portfolio.h"
#include <iostream>
#include <vector>

User::User(std::string username, std::string stockList) // std::string email; // double balance)
{
    this->username = username;
    this->stockList = stockList;


    Portfolio *p = new Portfolio(stockList);
    this->portfolios.push_back(*p); ;
}

std::string User::getUsername() {
    return this->username;
}

std::string User::getStockList(){
    return this->stockList;
}


void User::appendStockList(std::string tickerSymbol, std::string priceAt){
    stockList = stockList + tickerSymbol + " bought at " + priceAt + ",\n";
}


std::vector<Stock>* User::getStocks() {
    return this->portfolios.at(0).getStocks();
}



//    this->password = password;
//    this->email = email;
//    this->balance = balance;


