#ifndef STOCKSTALLIONWINDOW_H
#define STOCKSTALLIONWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QFileInfo>
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
    void setUserName(QString name);
    void setPassWord(QString pass);
    ~StockStallionWindow();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QPoint *offset;

private slots:

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_portfolioButton_clicked();

    void on_profileButton_clicked();

    void on_aboutButton_clicked();

    void on_portfolioButton_pressed();

    void on_profileButton_pressed();

    void on_aboutButton_pressed();

    void on_portfolioButton_released();

    void on_aboutButton_released();

    void on_profileButton_released();

private:
    void hideAllTabs();

    QString username;
    QString password;
    QSqlDatabase db;
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    Ui::StockStallionWindow *ui;
    bool isMouseDown = false;

};

#endif // STOCKSTALLIONWINDOW_H
