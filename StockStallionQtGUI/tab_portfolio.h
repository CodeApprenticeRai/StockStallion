#ifndef TAB_PORTFOLIO_H
#define TAB_PORTFOLIO_H

#include <QWidget>
#include <QVBoxLayout>
#include "stockselection.h"

namespace Ui {
class tab_Portfolio;
}

class tab_Portfolio : public QWidget
{
    Q_OBJECT

public:
    explicit tab_Portfolio(QWidget *parent = 0);
    void setPortfolioText(QString text);
    ~tab_Portfolio();

private slots:

    void on_addStockButton_clicked();

    void on_addStockButton_pressed();

    void on_removeStockButton_released();

    void on_removeStockButton_pressed();

    void on_addStockButton_released();

private:
    Ui::tab_Portfolio *ui;
    QVBoxLayout *layout;
};

#endif // TAB_PORTFOLIO_H
