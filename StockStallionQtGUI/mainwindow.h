#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QFileInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void loginButton_Clicked(QString s);
    QPoint *offset;

private slots:
    void on_loginButton_pressed();

    void on_loginButton_released();

    void on_loginButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    QSqlDatabase db;
    Ui::MainWindow *ui;
    bool isMouseDown = false;
};

#endif // MAINWINDOW_H
