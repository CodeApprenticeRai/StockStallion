#include "stockstallionwindow.h"
#include "ui_stockstallionwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

StockStallionWindow::StockStallionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StockStallionWindow)
{
    ui->setupUi(this);

    //connect(manager, SIGNAL(finished(QNetworkReply*)), this, slot(requestRead(QNetworkReply*)));
}

StockStallionWindow::~StockStallionWindow()
{
    delete ui;
}

void StockStallionWindow::on_pushButton_clicked()
{
    //Network Request Code
    manager = new QNetworkAccessManager();
    QNetworkRequest request;
    //request.setUrl(new QUrl("https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=APPL&interval=1min&apikey=CRINRUHDWCNFTVK2"));
    //manager->get(request);
    //QNetworkReply *reply = NULL;

}

void StockStallionWindow::requestRead(QNetworkReply *reply)
{
    QByteArray data;
    data = reply->readAll();
    //QString dataString = QString::fromC(data.data());
    //ui->dataTextBox->setText(dataString);
}
