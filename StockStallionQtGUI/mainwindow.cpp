#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "registerwindow.h"
#include "stockstallionwindow.h"
#include <QtSql>
#include <QFileInfo>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Setup Database
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("stockstallion.db");

    //Make Window transparent and frameless
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QWidget::setFixedSize(MainWindow::width(), MainWindow::height());

    MainWindow::setCentralWidget(ui->windowFrame);

    //Colors the top portion of the window blue
    ui->windowFrame->setObjectName("windowFrame");
    ui->windowFrame->setStyleSheet(" #windowFrame {background-color: qlineargradient(x1:0, x2: 0, y1: 0, y2: .5, stop: 0 #84A9FF, stop: 0.29 #84A9FF, stop: 0.2901 white, stop: 1 white); border-radius:5px; border:1px solid black}");

    //Add drop shadow
    //QGraphicsDropShadowEffect* dropShadow = new QGraphicsDropShadowEffect();
    //dropShadow->setBlurRadius(1);
    //ui->frame->setGraphicsEffect(dropShadow);
    
    //Stock Stallion Logo
    QPixmap logo(":/StockStallionImages/StockStallion Images/Login Form/stallionLogoWhite.png");
    ui->stockStallionLogo->setPixmap(logo.scaled(ui->stockStallionLogo->width(), ui->stockStallionLogo->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    //Username Icon
    QPixmap userIcon(":/StockStallionImages/StockStallion Images/Login Form/userName.png");
    ui->usernameIcon->setPixmap(userIcon.scaled(ui->usernameIcon->width(), ui->usernameIcon->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    //Password Icon
    QPixmap passIcon(":/StockStallionImages/StockStallion Images/Login Form/passwordLock.png");
    ui->passwordIcon->setPixmap(passIcon.scaled(ui->passwordIcon->width(), ui->passwordIcon->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        isMouseDown = true;

        QRect pos = geometry();
        offset = new QPoint(event->globalPos().x() - pos.x(), event->globalPos().y() - pos.y());
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    isMouseDown = false;
    offset = 0;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(isMouseDown)
    {
        QPoint *newPoint = new QPoint(event->globalPos().x() - offset->x(), event->globalPos().y() - offset->y());
        move(*newPoint);
    }
}

void MainWindow::on_loginButton_pressed()
{
    ui->loginButton->setStyleSheet("background-color: Silver;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

void MainWindow::on_loginButton_released()
{
    ui->loginButton->setStyleSheet("background-color: Gainsboro;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}


void MainWindow::on_loginButton_clicked()
{
    //Login debug
    QSqlQuery query;
    if(db.open())
    {
        if(query.exec("select * from users where username='" + ui->usernameTextBox->text() + "' and password='" + ui->passwordTextBox->text() + "'"))
        {
            int count = 0;
            while(query.next())
            {
                count++;
            }
            if(count == 1)
            {
                this->hide();
                StockStallionWindow *stockStallion = new StockStallionWindow(this);
                stockStallion->setUserName(ui->usernameTextBox->text());
                stockStallion->setPassWord(ui->passwordTextBox->text());
                stockStallion->show();
                db.close();
            }
        }
    }
    ui->passwordTextBox->setText("");
    ui->usernameErrorLabel->setStyleSheet("color: red");
    db.close();
}

void MainWindow::on_pushButton_2_clicked()
{
    this->close();
}

void MainWindow::on_pushButton_clicked()
{
    this->hide();
    RegisterWindow *registerWindow = new RegisterWindow(this);
    registerWindow->show();
}
