

#ifndef STOCKSTALLION_USER_H
#define STOCKSTALLION_USER_H

#include <iostream>
#include <vector>
#include "Portfolio.h"
#include <sstream>

class User {
    private:
        std::string username;
        std::string password;
        std::vector<Portfolio> portfolios;
        std::string stockList;

    public:
        User(std::string username, std::string stockList);
        std::string getUsername();
        std::string getStockList();
        void appendStockList(std::string tickerSymbol, std::string priceAt, std::string numShares);
        void removeFromStockList(std::string, std::string priceAt, std::string numOwned, int numToRemove);
        std::vector<Stock>* getStocks();
};



#endif //STOCKSTALLION_USER_H
