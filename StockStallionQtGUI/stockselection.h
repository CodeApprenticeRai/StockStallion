#ifndef STOCKSELECTION_H
#define STOCKSELECTION_H

#include <QDialog>

namespace Ui {
class StockSelection;
}

class StockSelection : public QDialog
{
    Q_OBJECT

public:
    explicit StockSelection(QWidget *parent = 0);
    QString getTicker();
    QString getShares();
    QString getTotalPrice();
    ~StockSelection();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QPoint *offset;

private slots:
    void on_cancelButton_clicked();

    void on_cancelButton_pressed();

    void on_cancelButton_released();

    void on_addStockButton_pressed();

    void on_addStockButton_released();

    void on_addStockButton_clicked();

private:
    Ui::StockSelection *ui;
    QString totalPrice;
    bool isMouseDown = false;
};

#endif // STOCKSELECTION_H
