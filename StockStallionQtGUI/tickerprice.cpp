#include "tickerprice.h"
#include <QWidget>

//Used to encapsulate the values of a stock ticker and its price
TickerPrice::TickerPrice(QString ticker, double price)
{
    this->ticker = ticker;
    this->price = price;
}

//Returns the stocks price
double TickerPrice::getPrice()
{
    return price;
}

//Returns the stocks ticker
QString TickerPrice::getTicker()
{
    return ticker;
}
