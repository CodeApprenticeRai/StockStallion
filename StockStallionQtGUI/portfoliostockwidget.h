#ifndef PORTFOLIOSTOCKWIDGET_H
#define PORTFOLIOSTOCKWIDGET_H

#include <QWidget>

namespace Ui {
class portfolioStockWidget;
}

class portfolioStockWidget : public QWidget
{
    Q_OBJECT

public:
    explicit portfolioStockWidget(QWidget *parent = 0);

    void setStockName(QString name);
    void setShares(int shares);
    void setBoughtAt(double price);
    void setCurrentPrice(double price);
    double getTotalCurrentValue();
    double getPercentChange();
    QString getStockName();
    void deSelect();
    void select();

    ~portfolioStockWidget();

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    Ui::portfolioStockWidget *ui;
    int shares;
    double boughtAtPrice;
    double currentPrice;
    double totalCurrentValue;
    double percentChange;
    bool selected = false;

    void updateValues();
};

#endif // PORTFOLIOSTOCKWIDGET_H
