//
// Created by tareg on 4/12/2018.
//

#include "Stock.h"
#include <iostream>

Stock::Stock(std::string ticker){
    //initializes a stock object using the parameter of its NYSE identifier
    this->ticker = ticker;
}

std::string Stock::getTicker(std::string ticker){
    return this->ticker;
}
