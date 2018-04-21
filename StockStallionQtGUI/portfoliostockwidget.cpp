#include "portfoliostockwidget.h"
#include "ui_portfoliostockwidget.h"

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
