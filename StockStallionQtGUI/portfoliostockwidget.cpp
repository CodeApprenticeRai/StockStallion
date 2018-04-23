#include "portfoliostockwidget.h"
#include "ui_portfoliostockwidget.h"
#include <QMouseEvent>

portfolioStockWidget::portfolioStockWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::portfolioStockWidget)
{
    ui->setupUi(this);
}

portfolioStockWidget::~portfolioStockWidget()
{
    delete ui;
}

void portfolioStockWidget::setStockName(QString name)
{
    ui->tickerLabel->setText(name);
}

void portfolioStockWidget::setShares(int shares)
{
    this->shares = shares;
    ui->sharesLabel->setText(QString::number(shares));
}

void portfolioStockWidget::setBoughtAt(double price)
{
    boughtAtPrice = price;
    ui->boughtAtLabel->setText(QString::number(price));
    updateValues();
}

void portfolioStockWidget::setCurrentPrice(double price)
{
    currentPrice = price;
    ui->currentPriceLabel->setText(QString::number(price));
    updateValues();
}

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

double portfolioStockWidget::getTotalCurrentValue()
{
    return totalCurrentValue;
}

double portfolioStockWidget::getPercentChange()
{
    return percentChange;
}

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

QString portfolioStockWidget::getStockName()
{
    return ui->tickerLabel->text();
}

double portfolioStockWidget::getBoughtPrice()
{
    return boughtAtPrice;
}

int portfolioStockWidget::getShares()
{
    return shares;
}

void portfolioStockWidget::select()
{
    selected = true;
    ui->frame->setStyleSheet("background-color:rgb(238,233,233)");
}

void portfolioStockWidget::deSelect()
{
    selected = false;
    ui->frame->setStyleSheet("background-color:transparent");
}

bool portfolioStockWidget::isSelected()
{
    return selected;
}
