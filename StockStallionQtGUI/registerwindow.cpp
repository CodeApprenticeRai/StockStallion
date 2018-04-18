#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "./sqlite/sqlite3.h"
#include <QtSql>
#include <QFileInfo>
#include <QMouseEvent>

RegisterWindow::RegisterWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegisterWindow)
{
    ui->setupUi(this);

    //Make Window transparent and frameless
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QWidget::setFixedSize(RegisterWindow::width(), RegisterWindow::height());

    RegisterWindow::setCentralWidget(ui->windowFrame);

    //Colors the top portion of the window blue
    ui->windowFrame->setObjectName("windowFrame");
    ui->windowFrame->setStyleSheet(" #windowFrame {background-color: qlineargradient(x1:0, x2: 0, y1: 0, y2: .5, stop: 0 #84A9FF, stop: 0.205 #84A9FF, stop: 0.2051 white, stop: 1 white); border-radius:5px; border:1px solid black}");

    //Stock Stallion Logo
    QPixmap logo(":/StockStallionImages/StockStallion Images/Login Form/stallionLogoWhite.png");
    ui->stockStallionLogo->setPixmap(logo.scaled(ui->stockStallionLogo->width(), ui->stockStallionLogo->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}

void RegisterWindow::on_pushButton_2_clicked()
{
    this->close();
}

void RegisterWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        isMouseDown = true;

        QRect pos = geometry();
        offset = new QPoint(event->globalPos().x() - pos.x(), event->globalPos().y() - pos.y());
    }
}

void RegisterWindow::mouseReleaseEvent(QMouseEvent *event)
{
    isMouseDown = false;
    offset = 0;
}

void RegisterWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(isMouseDown)
    {
        QPoint *newPoint = new QPoint(event->globalPos().x() - offset->x(), event->globalPos().y() - offset->y());
        move(*newPoint);
    }
}

void RegisterWindow::on_registerButton_pressed()
{
    ui->registerButton->setStyleSheet("background-color: Silver;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

void RegisterWindow::on_registerButton_released()
{
    ui->registerButton->setStyleSheet("background-color: Gainsboro;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

void RegisterWindow::on_registerButton_clicked()
{
    if(checkUsername())
        ui->usernameErrorLabel->setStyleSheet("color: transparent");
    else
        ui->usernameErrorLabel->setStyleSheet("color: red");

    if(checkEmail())
        ui->emailErrorLabel->setStyleSheet("color: transparent");
    else
        ui->emailErrorLabel->setStyleSheet("color: red");

    if(checkPassword())
    {
        ui->passwordErrorLabel->setStyleSheet("color: transparent");

        if(ui->passwordTextBox->text() == ui->passwordConfirmTextBox->text())
        {
            ui->passwordConfirmErrorLabel->setStyleSheet("color: transparent");
        }
        else
        {
            ui->passwordConfirmErrorLabel->setStyleSheet("color: red");
        }
    }
    else
    {
        ui->passwordErrorLabel->setStyleSheet("color: red");
        ui->passwordConfirmErrorLabel->setStyleSheet("color: transparent");
    }

    if(checkUsername() && checkEmail() && checkPassword())
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("stockstallion.db");
        if(db.open())
        {
            QSqlQuery query;
            db.exec("CREATE TABLE IF NOT EXISTS users( id INT , username TEXT PRIMARY KEY NOT NULL, email TEXT NOT NULL, password TEXT NOT NULL, stocklist TEXT);");

            //Checks if username already exists
            if(query.exec("select * from users where username='" + ui->usernameTextBox->text() + "' or email='" + ui->emailTextBox->text() + "'"))
            {
                int count = 0;
                while(query.next())
                {
                    count++;
                }
                if(count == 0)
                {
                    db.exec("INSERT into users(username, email, password, stocklist) VALUES('" + ui->usernameTextBox->text() + "','" + ui->emailTextBox->text() + "','" + ui->passwordTextBox->text() + "','NOSTOCKS');");
                }
            }

            db.close();
        }
    }
}

bool RegisterWindow::checkUsername()
{
    QString username = ui->usernameTextBox->text();
    bool containsAlphaNumerics = true;
    if(username.length() >= 6 && username.length() <= 32)
    {
        for(int i = 0; i < username.size(); i++)
        {
            if(!username[i].isLetterOrNumber())
            {
                containsAlphaNumerics = false;
            }
        }
    }
    else
    {
        return false;
    }
    return containsAlphaNumerics;
}

bool RegisterWindow::checkEmail()
{
    if(ui->emailTextBox->text().contains('@') && !ui->emailTextBox->text().contains(' '))
    {
        QStringList emailList = ui->emailTextBox->text().split('@');
        if(emailList[0].length() != 0)
        {
            if(emailList[1].contains('.'))
            {
                QStringList domainList = emailList[1].split('.');
                if(domainList[0].length() != 0 && domainList[1].length() != 0)
                    return true;
            }
        }
    }

    return false;
}

bool RegisterWindow::checkPassword()
{
    QString password = ui->passwordTextBox->text();

    bool containsAlphaNumerics = true;
    bool containsLetter = false;
    bool containsNumber = false;

    if(password.length() > 7 && password.length() < 21)
    {
        for(int i = 0; i < password.size(); i++)
        {
            if(password[i].isDigit())
                containsNumber = true;
            else if(password[i].isLetter())
                containsLetter = true;

            if(!password[i].isLetterOrNumber())
            {
                containsAlphaNumerics = false;
            }
        }
    }
    else
    {
        return false;
    }

    return (containsAlphaNumerics && containsLetter && containsNumber);
}
