
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
    //    std::string name;
    //    double investedValue;
    //    double currentValue;
        std::vector<Stock>* stocks;

        std::vector<Stock>* parseStockList(std::string stocklist);

    public:

        Portfolio( std::string stocklist);

        void add_stock(Stock, int);
        void rmv_stock(Stock);



    std::vector<Stock>* getStocks(){
        return this->stocks;
    }
    /* adds/removes a Stock to the stocks vector
           along with an initial # of shares, and
           returns the dollar value of the transaction */

    //    double close ();
        /* calls rmv_stock on all stocks in the stocks vector,
           and returns the total dolar value */

    //    void buy_shares(Stock, int);
    //    void sell_shares(Stock, int);
        /* adds/removes shares from a stock already in the vector,
           and returns the dollar value of the transaction */

    //    double update_value();
        /* reads the dollar value of each stock in the stocks value
           and returns the total */

    //    double roi();
        // returns current value divided by invested value



};


#endif //STOCKSTALLION_PORTFOLIO_H
