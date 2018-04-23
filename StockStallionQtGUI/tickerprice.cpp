#include "tickerprice.h"
#include <QWidget>

TickerPrice::TickerPrice(QString ticker, double price)
{
    this->ticker = ticker;
    this->price = price;
}

double TickerPrice::getPrice()
{
    return price;
}

QString TickerPrice::getTicker()
{
    return ticker;
}
