#ifndef SHARESSELECTION_H
#define SHARESSELECTION_H

#include <QDialog>

namespace Ui {
class SharesSelection;
}

class SharesSelection : public QDialog
{
    Q_OBJECT

public:
    explicit SharesSelection(QWidget *parent = 0);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    int getShares();
    QPoint *offset;

    bool canceled = false;

    ~SharesSelection();

private slots:
    void on_cancelButton_clicked();

    void on_cancelButton_pressed();

    void on_cancelButton_released();

    void on_addStockButton_pressed();

    void on_addStockButton_released();

    void on_addStockButton_clicked();

private:
    Ui::SharesSelection *ui;
    bool isMouseDown = false;
};

#endif // SHARESSELECTION_H
