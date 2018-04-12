//
// Created by tareg on 4/12/2018.
//

#include "Portfolio.h"


Portfolio::Portfolio( std::string stocklist){
this->stocks = parseStockList(stocklist);
}

std::vector<Stock>* Portfolio::parseStockList(std::string stocklist){
    if (stocklist == ""){
        std::vector<Stock>* v = new std::vector<Stock>;
        return v;
    }
    else{
        // parse as csv
    }
};