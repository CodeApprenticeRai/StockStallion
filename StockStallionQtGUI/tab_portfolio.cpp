#include "tab_portfolio.h"
#include "ui_tab_portfolio.h"
#include "portfoliostockwidget.h"
#include "stockselection.h"
#include "sharesselection.h"
#include <QDebug>

#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

tab_Portfolio::tab_Portfolio(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tab_Portfolio)
{
    ui->setupUi(this);

    manager = new QNetworkAccessManager();
    QObject::connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(managerFinished(QNetworkReply*)));

    QObject::connect(manager, &QNetworkAccessManager::finished,
            this, [=](QNetworkReply *reply) {
            }
        );

    layoutList.append(new QVBoxLayout());
    layoutList[0]->setSpacing(0);
    layoutList[0]->setContentsMargins(0,0,0,0);
    //layoutList[0]->insertStretch( -1, 1 );

    pageCount.append(0);



    QWidget *page = new QWidget();
    page->setLayout(layoutList.at(0));
    ui->stockListStacked->addWidget(page);

    /* Save data from database
QVector<QStringList> lst;
while (query.next())
{
    QSqlRecord record = query.record();
    QStringList tmp;
    for(int i=0; i < record.count(); i++)
    {
        tmp << record.value(i).toString();
    }
    lst.append(tmp);
}*/
}

tab_Portfolio::~tab_Portfolio()
{
    delete ui;
    delete manager;
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

    if(selector.canceled)
        return;

    //Network code
    QString url1 = "https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=";
    QString symbol = selector.getTicker();
    QString url2 = "&interval=1min&apikey=CRINRUHDWCNFTVK2";
    //Verify that ticker actually exists
    QNetworkRequest request(QUrl(url1 + symbol + url2));
    //request.setHeader(QNetworkRequest::ContentTypeHeader, “application/json”);
    QNetworkReply* reply = manager->get(request);


    SharesSelection sharesSelector;
    sharesSelector.setModal(true);
    sharesSelector.exec();

    //Add stock to view
    if(!sharesSelector.canceled)
    {
        portfolioStockWidget *widget = new portfolioStockWidget(this);
        portfolioStockList.append(widget);
        QString ticker = selector.getTicker();
        int shares = sharesSelector.getShares();
        //double currentPrice = getCurrentPrice(ticker);
        //double boughtAtPrice = currentPrice;

        widget->setStockName(ticker);
        widget->setShares(shares);
        //widget->setCurrentPrice(currentPrice);
        //widget->setBoughtAt(boughtAtPrice);

        layoutList.at(lastPage)->addWidget(widget);
        pageCount[lastPage]++;
        if(pageCount[lastPage] == 6)
        {
            lastPage++;
            ui->maxPageLabel->setText("of " + QString::number(lastPage+1));
            ui->pageSelectSpinner->setMaximum(lastPage+1);
            pageCount.append(0);
            layoutList.append(new QVBoxLayout());
            QWidget *page = new QWidget();
            page->setLayout(layoutList.at(lastPage));
            ui->stockListStacked->addWidget(page);
        }
    }
}

void tab_Portfolio::on_removeStockButton_clicked()
{
    for(int i = 0; i < portfolioStockList.count(); i++)
    {
        //if(portfolioStockList[i]->selected)
        //{
            //portfolioStockList[i]->destroy();
        //}
    }
}

void tab_Portfolio::on_backButton_clicked()
{
    if(currentPage > 0)
    {
        currentPage--;
        ui->stockListStacked->setCurrentIndex(currentPage);
        ui->pageSelectSpinner->setValue(currentPage+1);
    }
}

void tab_Portfolio::on_nextButton_clicked()
{
    if(currentPage < lastPage)
    {
        currentPage++;
        ui->stockListStacked->setCurrentIndex(currentPage);
        ui->pageSelectSpinner->setValue(currentPage+1);
    }
}

void tab_Portfolio::on_pageSelectSpinner_valueChanged(const QString &arg1)
{
    currentPage = ui->pageSelectSpinner->value() - 1;
    ui->stockListStacked->setCurrentIndex(currentPage);
}

void tab_Portfolio::managerFinished(QNetworkReply *reply) {
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }
    QString stringResponse = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(stringResponse.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QStringList keys = jsonObject.keys();
    auto metadata = jsonObject["Meta Data"].toObject();
    auto date = metadata["3. Last Refreshed"];
    QStringList priceKeys = jsonObject[keys.at(1)].toObject().keys();
    auto prices = jsonObject[keys.at(1)].toObject();
    auto price = prices[priceKeys.at(0)].toObject()["1. open"];
    QString dateString = date.toString();
    QString priceString = price.toString();
    qDebug() << "Date: " << dateString << " Price: " + priceString;
    //ui->testLineEdit->setText("Date: " + dateString + " Price: " + priceString );
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

void tab_Portfolio::on_backButton_pressed()
{
    ui->backButton->setStyleSheet("background-color: Silver;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

void tab_Portfolio::on_backButton_released()
{
    ui->backButton->setStyleSheet("background-color: Gainsboro;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

void tab_Portfolio::on_nextButton_pressed()
{
    ui->nextButton->setStyleSheet("background-color: Silver;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

void tab_Portfolio::on_nextButton_released()
{
    ui->nextButton->setStyleSheet("background-color: Gainsboro;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}
