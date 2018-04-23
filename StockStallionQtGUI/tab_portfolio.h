#ifndef TAB_PORTFOLIO_H
#define TAB_PORTFOLIO_H

#include <QWidget>
#include <QVBoxLayout>
#include "stockselection.h"
#include "portfoliostockwidget.h"
#include "tickerprice.h"

#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>

namespace Ui {
class tab_Portfolio;
}

class tab_Portfolio : public QWidget
{
    Q_OBJECT

public:
    explicit tab_Portfolio(QWidget *parent = 0);
    void setPortfolioText(QString text);
    void setUsername(QString name);

    ~tab_Portfolio();

private slots:

    void on_addStockButton_clicked();

    void on_addStockButton_pressed();

    void on_removeStockButton_released();

    void on_removeStockButton_pressed();

    void on_addStockButton_released();

    void on_backButton_clicked();

    void on_nextButton_clicked();

    void on_pageSelectSpinner_valueChanged(const QString &arg1);

    void on_backButton_pressed();

    void on_backButton_released();

    void on_nextButton_released();

    void on_nextButton_pressed();

    void on_removeStockButton_clicked();

    void managerFinished(QNetworkReply *reply);

private:
    void initializeStocklist();
    void updateTransactions(QString ticker, double price, int shares);
    void removeTransaction(QString ticker, double price, int shares);
    void updateStockList();
    double getCurrentPrice(QString ticker);

    QNetworkAccessManager *manager;
    QNetworkRequest request;

    Ui::tab_Portfolio *ui;
    QList<QVBoxLayout *> layoutList;
    QList<portfolioStockWidget *> portfolioStockList;
    int currentPage = 0;
    int lastPage = 0;
    QList<int> pageCount;
    double networkStockPrice;
    QString username;
    QString networkStockDate = "";
    QList<TickerPrice *> savedPrices;

};

#endif // TAB_PORTFOLIO_H
