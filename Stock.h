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

    public:
        Stock(std::string ticker);
        std::string getTicker(std::string);
};


#endif //STOCKSTALLION_STOCK_H
