
//

#ifndef STOCKSTALLION_PORTFOLIO_H
#define STOCKSTALLION_PORTFOLIO_H

#include <iostream>
#include <vector>
#include "Stock.h"


// For now, this'll just be a wrapper function


class Portfolio
{
    private:
        std::vector<Stock>* stocks;

        std::vector<Stock>* parseStockList(std::string stocklist);

    public:

        Portfolio( std::string stocklist);

        void add_stock(Stock, int);
        void rmv_stock(Stock);



    std::vector<Stock>* getStocks(){
        return this->stocks;
    }
};


#endif //STOCKSTALLION_PORTFOLIO_H
