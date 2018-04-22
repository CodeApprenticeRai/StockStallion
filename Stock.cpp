//
// Created by tareg on 4/12/2018.
//

#include "Stock.h"
#include <iostream>

//Initializes a stock object using the parameter of its New York Stock Exchange identifier
Stock::Stock(std::string ticker){
    //initializes a stock object using the parameter of its NYSE identifier
    this->ticker = ticker;
}

//Returns the name of the stock object
std::string Stock::getTicker(std::string ticker){
    return this->ticker;
}
