#ifndef TICKERPRICE_H
#define TICKERPRICE_H

#include <QWidget>

class TickerPrice
{
public:
    TickerPrice(QString ticker, double price);
    QString getTicker();
    double getPrice();

private:
    QString ticker;
    double price;
};

#endif // TICKERPRICE_H
