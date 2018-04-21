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
    ~portfolioStockWidget();

private:
    Ui::portfolioStockWidget *ui;
};

#endif // PORTFOLIOSTOCKWIDGET_H
