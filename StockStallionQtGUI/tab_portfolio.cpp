#include "tab_portfolio.h"
#include "ui_tab_portfolio.h"
#include "portfoliostockwidget.h"
#include "stockselection.h"
#include <QVBoxLayout>
#include <QPushButton>
tab_Portfolio::tab_Portfolio(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tab_Portfolio)
{
    ui->setupUi(this);

    layout = new QVBoxLayout(this);
}

tab_Portfolio::~tab_Portfolio()
{
    delete ui;
}

void tab_Portfolio::setPortfolioText(QString text)
{
    ui->userLabel->setText(text);
}

void tab_Portfolio::on_addStockButton_clicked()
{
    StockSelection selector;
    selector.setModal(true);
    selector.exec();

    if(selector.getTicker() != "NULL")
    {
        portfolioStockWidget *widget = new portfolioStockWidget(ui->stockListing);
        widget->setGeometry(0,0, 741, 32);
        layout->addWidget(widget);
        widget->move(0,0);
        ui->scrollAreaWidgetContents->setLayout(layout);
    }
}

void tab_Portfolio::on_addStockButton_pressed()
{
    ui->addStockButton->setStyleSheet("background-color: Silver;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

void tab_Portfolio::on_addStockButton_released()
{
    ui->addStockButton->setStyleSheet("background-color: Gainsboro;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

void tab_Portfolio::on_removeStockButton_pressed()
{
    ui->removeStockButton->setStyleSheet("background-color: Silver;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

void tab_Portfolio::on_removeStockButton_released()
{
    ui->removeStockButton->setStyleSheet("background-color: Gainsboro;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}
