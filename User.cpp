//
// Created by tareg on 4/12/2018.
//

#include "User.h"
#include "Portfolio.h"
#include <iostream>
#include <vector>

/*Assigns the username and stocklist values to their associated variables in the object
 *and then creates a portfolio pointer and adds it to a vector which holds portfolio pointer values.
 */
User::User(std::string username, std::string stockList) // std::string email; // double balance)
    {
    this->username = username;
    this->stockList = stockList;


    Portfolio *p = new Portfolio(stockList);
    this->portfolios.push_back(*p);

}

//Returns the user’s username
std::string User::getUsername() {
    return this->username;
}

//Returns the list of stocks
std::string User::getStockList(){
    return this->stockList;
}


/*
 * Appends the stocklist string to include the ticker symbol of the stock, the number of shares and
 * the price it was bought at
 */
void User::appendStockList(std::string tickerSymbol, std::string priceAt, std::string numShares){
    stockList = stockList + tickerSymbol + " x" + numShares + " bought at " + priceAt + ",\n";
}

/*
 * Finds the name of the stock to remove, then the original number of shares and then removes them
 * before updating the stocklist variable in the object
 */
void User::removeFromStockList(std::string tickerSymbol, std::string priceAt, std::string originalNum, int numToRemove) {
    std::istringstream stream(stockList);
    std::string line;

    std::string temp;

    while(std::getline(stream, line)){
        //find company name
        char until(' ');
        std::string compName = line.substr(0, line.find(until));

        //find price bought at
        int pos1 = line.find(" at ");
        std::string firstPrice = line.substr(pos1+4, line.find(' ')+1 - pos1+4);

        //find original number of shares
        int pos0 = line.find(" x");
        std::string numOwned = line.substr(pos0+2, line.find(' ')+1 - pos0+2);
        int numOwned0 = stoi(numOwned);
        int numOwned1 = stoi(originalNum);
        int start = pos0+2;
        int end = line.find(' ')+1 - pos0+2;

        //find line with correct parameters and then update
        if(tickerSymbol == compName){
            if(priceAt == firstPrice){
                if(numOwned1 == numOwned0){
                    int intermed = stoi(originalNum) - numToRemove;
                    std::string numShares = std::to_string(intermed);

                    if(intermed == 0){
                        line = "";
                    }
                    else{
                        line.replace(start, end, numShares);
                    }

                }
            }
        }

        if(line != ""){
            temp = temp + line + "\n";
        }


    }

    stockList = temp;
}

//Returns the stock pointer at the first position in the vector
std::vector<Stock>* User::getStocks() {
    return this->portfolios.at(0).getStocks();
}



