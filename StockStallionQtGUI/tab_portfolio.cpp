#include "tab_portfolio.h"
#include "ui_tab_portfolio.h"
#include "portfoliostockwidget.h"
#include "stockselection.h"
#include "sharesselection.h"
#include "tickerprice.h"
#include <QDebug>
#include <QtSql>
#include <QMessageBox>

#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <openssl/aes.h>

//Initializes the portfolio tab. Sets up the stock display. Loads stocks into display
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

    pageCount.append(0);

    QWidget *page = new QWidget();
    page->setLayout(layoutList.at(0));
    ui->stockListStacked->addWidget(page);
}

//Used to load stocks into database
void tab_Portfolio::initializeStocklist()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("stockstallion.db");
    if(db.open())
    {
        QSqlQuery query;
        QSqlTableModel *model = new QSqlTableModel(this);

        model->setTable("transactions");
        model->select();

        for(int i = 0; i < model->rowCount(); ++i)
        {
            if(model->record(i).value("username").toString() == username && model->record(i).value("has_been_sold").toString() == "0")
            {
                //qDebug() << model->record(i).value("ticker").toString();
                portfolioStockWidget *widget = new portfolioStockWidget(this);
                portfolioStockList.append(widget);
                widget->setStockName(model->record(i).value("ticker").toString());
                widget->setShares(model->record(i).value("num_Shares").toInt());
                widget->setCurrentPrice(getCurrentPrice(model->record(i).value("ticker").toString()));
                widget->setBoughtAt(model->record(i).value("buy_price").toDouble());
            }
        }

        double totalCurrentValue = 0;
        double percentChange = 0;
        for(int i = 0; i < portfolioStockList.count(); i++)
        {
            totalCurrentValue += portfolioStockList[i]->getTotalCurrentValue();
            percentChange += portfolioStockList[i]->getPercentChange();
        }
        ui->portfolioValueLabel->setText("Total Portfolio Value: $" + QString::number(totalCurrentValue));
        ui->growthLabel->setText("Total Growth of Included Securities: " + QString::number(percentChange) + "%");


        updateStockList();
    }
}

//Used to update stocks in database
void tab_Portfolio::updateTransactions(QString ticker, double price, int shares){
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("stockstallion.db");
    if(db.open())
    {
        QString buy_price = QString::number(price);
        QString num_shares = QString::number(shares);
        db.exec("INSERT INTO transactions(username, ticker, num_shares, buy_price, date_bought, has_been_sold) VALUES('"+username+"','" + ticker + "','" +num_shares+"','"+buy_price+"','" + QDate::currentDate().toString() + "','0');");
    }
}

//Used to unlist stocks from database
void tab_Portfolio::removeTransaction(QString ticker, double price, int shares)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("stockstallion.db");
    if(db.open())
    {
        QString buy_price = QString::number(price);
        QString num_shares = QString::number(shares);
        qDebug() << ticker << " " << price << " " << shares;
        db.exec("UPDATE transactions SET has_been_sold=1 WHERE username='" + username + "' AND ticker='" + ticker + "' AND buy_price='" + buy_price + "' AND num_shares='" + num_shares + "'");
    }
}

//Destructor
tab_Portfolio::~tab_Portfolio()
{
    delete ui;
    delete manager;
}

//Displays username on portfolio tab
void tab_Portfolio::setPortfolioText(QString text)
{
    ui->userLabel->setText(text);
}

//Passes username to the portfolio tab object
void tab_Portfolio::setUsername(QString name)
{
    username = name;

    initializeStocklist();
}

//Adds stock to the database and updates stocklist view
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

    ui->addStockButton->setEnabled(false);
    ui->removeStockButton->setEnabled(false);
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    ui->addStockButton->setEnabled(true);
    ui->removeStockButton->setEnabled(true);

    if(networkStockDate == "NA")
    {
        QMessageBox msgBox;
        msgBox.setAttribute(Qt::WA_QuitOnClose, false);
        msgBox.setText("Invalid Ticker.");
        msgBox.exec();
        return;
    }

    SharesSelection sharesSelector;
    sharesSelector.setPrice(networkStockPrice);
    sharesSelector.setModal(true);
    sharesSelector.exec();

    //Add stock to view
    if(!sharesSelector.canceled)
    {
        QString ticker = selector.getTicker();
        int shares = sharesSelector.getShares();
        double currentPrice = networkStockPrice;
        double boughtAtPrice = currentPrice;

        updateTransactions(ticker, currentPrice, shares);
        portfolioStockWidget *widget = new portfolioStockWidget(this);
        portfolioStockList.append(widget);
        widget->setStockName(ticker);
        widget->setShares(shares);
        widget->setCurrentPrice(currentPrice);
        widget->setBoughtAt(boughtAtPrice);

        savedPrices.append(new TickerPrice(ticker, currentPrice));

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


        double totalCurrentValue = 0;
        double percentChange = 0;
        for(int i = 0; i < portfolioStockList.count(); i++)
        {
            totalCurrentValue += portfolioStockList[i]->getTotalCurrentValue();
            percentChange += portfolioStockList[i]->getPercentChange();
        }
        ui->portfolioValueLabel->setText("Total Portfolio Value: $" + QString::number(totalCurrentValue));
        ui->growthLabel->setText("Total Growth of Included Securities: " + QString::number(percentChange) + "%");
    }

    networkStockDate = "";
    networkStockPrice = 0.00;
}

//Adds stocks already in database to stocklist view
void tab_Portfolio::updateStockList()
{
    for(int i = 0; i < portfolioStockList.count(); i++)
    {
        layoutList.at(lastPage)->addWidget(portfolioStockList[i]);
        pageCount[lastPage]++;
        if(pageCount[lastPage] == 6)
        {
            lastPage++;
            ui->maxPageLabel->setText("of " + QString::number(lastPage+1));
            ui->pageSelectSpinner->setMaximum(lastPage+1);
            pageCount.append(0);
            layoutList.append(new QVBoxLayout());
            layoutList[lastPage]->setSpacing(0);
            layoutList[lastPage]->setContentsMargins(0,0,0,0);
            QWidget *page = new QWidget();
            page->setLayout(layoutList.at(lastPage));
            ui->stockListStacked->addWidget(page);
        }
    }
}

//Removes a stock from the database and updates stock list view
void tab_Portfolio::on_removeStockButton_clicked()
{
    for(int i = ui->stockListStacked->count(); i >= 0; i--)
    {
        QWidget* widget = ui->stockListStacked->widget(i);
        ui->stockListStacked->removeWidget(widget);
        widget->deleteLater();
    }
    currentPage = 0;
    lastPage = 0;
    ui->maxPageLabel->setText("of 1");
    ui->pageSelectSpinner->setMaximum(1);
    ui->pageSelectSpinner->setValue(1);
    pageCount.clear();
    layoutList.clear();
    layoutList.append(new QVBoxLayout());
    layoutList[0]->setSpacing(0);
    layoutList[0]->setContentsMargins(0,0,0,0);

    pageCount.clear();
    pageCount.append(0);

    QWidget *page = new QWidget();
    page->setLayout(layoutList.at(0));
    ui->stockListStacked->addWidget(page);

    for(int i = 0; i < portfolioStockList.count(); i++)
    {
        if(portfolioStockList[i]->isSelected())
        {
            removeTransaction(portfolioStockList[i]->getStockName(), portfolioStockList[i]->getBoughtPrice(), portfolioStockList[i]->getShares());
        }
    }

    portfolioStockList.clear();
    //layoutList[0]->addWidget(new portfolioStockWidget);
    initializeStocklist();
}

//If a stock was already added, this function returns the price without connecting to the API
//Otherwise, it connects to the API to return the price
double tab_Portfolio::getCurrentPrice(QString ticker)
{
    for(int i = 0; i < savedPrices.count(); i++)
    {
        if(savedPrices[i]->getTicker() == ticker)
            return savedPrices[i]->getPrice();
    }

    //Network code
    QString url1 = "https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=";
    QString symbol = ticker;
    QString url2 = "&interval=1min&apikey=CRINRUHDWCNFTVK2";
    //Verify that ticker actually exists
    QNetworkRequest request(QUrl(url1 + symbol + url2));
    //request.setHeader(QNetworkRequest::ContentTypeHeader, “application/json”);
    QNetworkReply* reply = manager->get(request);

    ui->addStockButton->setEnabled(false);
    ui->removeStockButton->setEnabled(false);
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    ui->addStockButton->setEnabled(true);
    ui->removeStockButton->setEnabled(true);

    if(networkStockDate == "NA")
        return -1;

    savedPrices.append(new TickerPrice(ticker, networkStockPrice));
    return networkStockPrice;

}

//Moves the page backwards
void tab_Portfolio::on_backButton_clicked()
{
    if(currentPage > 0)
    {
        currentPage--;
        ui->stockListStacked->setCurrentIndex(currentPage);
        ui->pageSelectSpinner->setValue(currentPage+1);
    }
}

//Moves the page foward
void tab_Portfolio::on_nextButton_clicked()
{
    if(currentPage < lastPage)
    {
        currentPage++;
        ui->stockListStacked->setCurrentIndex(currentPage);
        ui->pageSelectSpinner->setValue(currentPage+1);
    }
}

//Changes the stock page being viewed
void tab_Portfolio::on_pageSelectSpinner_valueChanged(const QString &arg1)
{
    currentPage = ui->pageSelectSpinner->value() - 1;
    ui->stockListStacked->setCurrentIndex(currentPage);
}

//Parses stock values from API
void tab_Portfolio::managerFinished(QNetworkReply *reply) {
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }
    try
    {
        QString stringResponse = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(stringResponse.toUtf8());
        QJsonObject jsonObject = jsonResponse.object();
        QStringList keys = jsonObject.keys();
        auto metadata = jsonObject["Meta Data"].toObject();
        auto date = metadata["3. Last Refreshed"];

        if(stringResponse.indexOf(QString("Error")) == -1)
        {
            QStringList priceKeys = jsonObject[keys.at(1)].toObject().keys();
            auto prices = jsonObject[keys.at(1)].toObject();
            auto price = prices[priceKeys.at(0)].toObject()["1. open"];
            QString dateString = date.toString();
            QString priceString = price.toString();
            networkStockDate = dateString;
            networkStockPrice = priceString.toDouble();
            if(networkStockDate.indexOf(QString("Error")) == -1)
                qDebug() << "Date: " << dateString << " Price: " + priceString;
            else
                networkStockDate = "NA";
        }
        else
        {
            networkStockDate = "NA";
            qDebug() << "Invalid Ticker";
        }
    }
    catch(...)
    {
        networkStockDate = "NA";
        qDebug() << "Invalid Ticker";
    }
}

//Colors buttons when pressed/released
void tab_Portfolio::on_addStockButton_pressed()
{
    ui->addStockButton->setStyleSheet("background-color: Silver;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

//Colors buttons when pressed/released
void tab_Portfolio::on_addStockButton_released()
{
    ui->addStockButton->setStyleSheet("background-color: Gainsboro;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

//Colors buttons when pressed/released
void tab_Portfolio::on_removeStockButton_pressed()
{
    ui->removeStockButton->setStyleSheet("background-color: Silver;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

//Colors buttons when pressed/released
void tab_Portfolio::on_removeStockButton_released()
{
    ui->removeStockButton->setStyleSheet("background-color: Gainsboro;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

//Colors buttons when pressed/released
void tab_Portfolio::on_backButton_pressed()
{
    ui->backButton->setStyleSheet("background-color: Silver;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

//Colors buttons when pressed/released
void tab_Portfolio::on_backButton_released()
{
    ui->backButton->setStyleSheet("background-color: Gainsboro;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

//Colors buttons when pressed/released
void tab_Portfolio::on_nextButton_pressed()
{
    ui->nextButton->setStyleSheet("background-color: Silver;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

//Colors buttons when pressed/released
void tab_Portfolio::on_nextButton_released()
{
    ui->nextButton->setStyleSheet("background-color: Gainsboro;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}
