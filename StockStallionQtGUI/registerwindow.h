#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QMainWindow>

namespace Ui {
class RegisterWindow;
}

class RegisterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterWindow(QWidget *parent = 0);
    ~RegisterWindow();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QPoint *offset;

private slots:
    void on_pushButton_2_clicked();

    void on_registerButton_pressed();

    void on_registerButton_released();

    void on_registerButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::RegisterWindow *ui;
    bool isMouseDown = false;

    bool checkUsername();
    bool checkEmail();
    bool checkPassword();
};

#endif // REGISTERWINDOW_H
