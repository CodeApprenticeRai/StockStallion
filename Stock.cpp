//
// Created by tareg on 4/12/2018.
//

#include "Stock.h"
#include <iostream>

Stock::Stock(std::string ticker){
    //initializes a stock object using the parameter of its NYSE identifier
    this->ticker = ticker;
}

//accessor and mutator for company name
//void setCompanyName(string ticker){
//    //using stockname retrieve company name from API
//    string foundFromAPI;
//    string companyName = foundFromAPI;
//    this-> companyName = foundFromAPI;
//
//}

std::string Stock::getTicker(std::string ticker){
    return this->ticker;
}

//accessor and mutator for current price
//void Stock::setCurrentPricePerShare(std::string stockName){
//    //using stockname retrieve current price from API
//    int foundFromAPI;
//    int currentPricePerShare = foundFromAPI;
//    this->currentPricePerShare = foundFromAPI;
//
//}

//std::string Stock::getCurrentPricePerShare(std::string stockName){
//    return this->currentPricePerShare;
//}


//accessor and mutator for number of shares
//void setNumberOfShares(string stockName){
//    int numberAdded;
//    //number added should come from buy (+) and sell (-) number of shares methods in portfolio
//    this-> numberofSharesOwned += numberAdded;
//
//}

//string getNumberOfShares(string stockName){
//    return this-> numberofSharesOwned;
//}