#include "portfoliostockwidget.h"
#include "ui_portfoliostockwidget.h"
#include <QMouseEvent>

//Initializes widget
portfolioStockWidget::portfolioStockWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::portfolioStockWidget)
{
    ui->setupUi(this);
}

//Deconstructor
portfolioStockWidget::~portfolioStockWidget()
{
    delete ui;
}

//Displays the ticker to the user
void portfolioStockWidget::setStockName(QString name)
{
    ui->tickerLabel->setText(name);
}

//Displays shares to the user
void portfolioStockWidget::setShares(int shares)
{
    this->shares = shares;
    ui->sharesLabel->setText(QString::number(shares));
}

//Displays the bought at price to the user
void portfolioStockWidget::setBoughtAt(double price)
{
    boughtAtPrice = price;
    ui->boughtAtLabel->setText(QString::number(price));
    updateValues();
}

//Displays the current price to the user
void portfolioStockWidget::setCurrentPrice(double price)
{
    currentPrice = price;
    ui->currentPriceLabel->setText(QString::number(price));
    updateValues();
}

//Displays the percent change and total current value to the user
void portfolioStockWidget::updateValues()
{
    //Determine percent change value
    percentChange = ((currentPrice-boughtAtPrice)/currentPrice)*100;
    ui->changeLabel->setText(QString::number(percentChange) + "%");

    if(percentChange < 0)
        ui->changeLabel->setStyleSheet("color: red;font: 12pt \"Century Gothic\";");
    else if(percentChange >= 0)
        ui->changeLabel->setStyleSheet("color: black;font: 12pt \"Century Gothic\";");


    //Determine current total value
    totalCurrentValue = currentPrice * shares;
    ui->totalValueLabel->setText("$" + QString::number(totalCurrentValue));
}

//Returns the total current value
double portfolioStockWidget::getTotalCurrentValue()
{
    return totalCurrentValue;
}

//Returns the percent change
double portfolioStockWidget::getPercentChange()
{
    return percentChange;
}

//Allows user to select widget
void portfolioStockWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(!selected)
            select();
        else
            deSelect();
    }
}

//Returns the stock name
QString portfolioStockWidget::getStockName()
{
    return ui->tickerLabel->text();
}

//Returns the bought price
double portfolioStockWidget::getBoughtPrice()
{
    return boughtAtPrice;
}

//Returns the amount of shares
int portfolioStockWidget::getShares()
{
    return shares;
}

//Signifies to user that the widget was selected
void portfolioStockWidget::select()
{
    selected = true;
    ui->frame->setStyleSheet("background-color:rgb(238,233,233)");
}

//Signifies to user that the widget was deselected
void portfolioStockWidget::deSelect()
{
    selected = false;
    ui->frame->setStyleSheet("background-color:transparent");
}

//Returns if the widget is selected
bool portfolioStockWidget::isSelected()
{
    return selected;
}
