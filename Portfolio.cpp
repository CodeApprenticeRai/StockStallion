//
// Created by tareg on 4/12/2018.
//

#include "Portfolio.h"

//Calls the parseStockList method to use the string stocklist and adds it to the stock object in the portfolio object
Portfolio::Portfolio( std::string stocklist){
this->stocks = parseStockList(stocklist);
}

//If the list is empty it creates a new pointer vector pointer that holds stock objects
std::vector<Stock>* Portfolio::parseStockList(std::string stocklist){
    if (stocklist == ""){
        std::vector<Stock>* v = new std::vector<Stock>;
        return v;
    }
    else{
        // parse as csv
    }
};