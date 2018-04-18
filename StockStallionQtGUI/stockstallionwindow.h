#ifndef STOCKSTALLIONWINDOW_H
#define STOCKSTALLIONWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

namespace Ui {
class StockStallionWindow;
}

class StockStallionWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StockStallionWindow(QWidget *parent = 0);
    void requestRead(QNetworkReply *reply);
    ~StockStallionWindow();

private slots:
    void on_pushButton_clicked();

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    Ui::StockStallionWindow *ui;
};

#endif // STOCKSTALLIONWINDOW_H
