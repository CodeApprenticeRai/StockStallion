//
// Created by tareg on 4/12/2018.
//

#ifndef STOCKSTALLION_STOCK_H
#define STOCKSTALLION_STOCK_H

#include <iostream>
#include <vector>


class Stock {
    private:
        std::string ticker;
//        std::string companyName;
        int currentPricePerShare;
//        int numberofSharesOwned;


//        void setCompanyName(std::string);
        void setCurrentPrice(std::string);
        void setNumberOfShares(std::string);

    public:
        Stock(std::string ticker);
        std::string getTicker(std::string);
        int getCurrentPrice(std::string);
        int getNumberOfShares(std::string);

};


#endif //STOCKSTALLION_STOCK_H
