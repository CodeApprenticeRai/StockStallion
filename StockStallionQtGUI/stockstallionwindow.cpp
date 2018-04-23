#include "stockstallionwindow.h"
#include "ui_stockstallionwindow.h"
#include <QtSql>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMouseEvent>

StockStallionWindow::StockStallionWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StockStallionWindow)
{
    ui->setupUi(this);

    //Setup database
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("stockstallion.db");

    //Make Window frameless
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QWidget::setFixedSize(StockStallionWindow::width(), StockStallionWindow::height());

    //Colors the window white
    ui->centralwidget->setObjectName("centralWidget");
    ui->centralwidget->setStyleSheet(" #centralWidget {background-color: qlineargradient(x1:0, x2: 0, y1: 0, y2: .5, stop: 0 #84A9FF, stop: 0.16 #84A9FF, stop: 0.161 white, stop: 1 white); border-radius:5px; border:1px solid black}");

    //Stock Stallion Logo
    QPixmap logo(":/StockStallionImages/StockStallion Images/Login Form/stallionLogoWhite.png");
    ui->stockStallionLogo->setPixmap(logo.scaled(ui->stockStallionLogo->width(), ui->stockStallionLogo->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    //Set portfolio button icon
    QIcon portfolioIcon(":/StockStallionImages/StockStallion Images/StockStallion Form/portfolio-icon.png");
    ui->portfolioButton->setIcon(portfolioIcon);
    ui->portfolioButton->setIconSize(QSize(ui->portfolioButton->width()-25, ui->portfolioButton->height()-25));

    //Set profile button icon
    QIcon profileIcon(":/StockStallionImages/StockStallion Images/StockStallion Form/profile-icon.png");
    ui->profileButton->setIcon(profileIcon);
    ui->profileButton->setIconSize(QSize(ui->profileButton->width()-25, ui->profileButton->height()-25));

    //Set about button icon
    QIcon aboutIcon(":/StockStallionImages/StockStallion Images/StockStallion Form/about-icon.png");
    ui->aboutButton->setIcon(aboutIcon);
    ui->aboutButton->setIconSize(QSize(ui->aboutButton->width()-25, ui->aboutButton->height()-25));

    //Hide profile and about tab by default
    hideAllTabs();
    ui->portfolioTab->show();
}

StockStallionWindow::~StockStallionWindow()
{
    delete ui;
}

void StockStallionWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        isMouseDown = true;

        QRect pos = geometry();
        offset = new QPoint(event->globalPos().x() - pos.x(), event->globalPos().y() - pos.y());
    }
}

void StockStallionWindow::mouseReleaseEvent(QMouseEvent *event)
{
    isMouseDown = false;
    offset = 0;
}

void StockStallionWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(isMouseDown)
    {
        QPoint *newPoint = new QPoint(event->globalPos().x() - offset->x(), event->globalPos().y() - offset->y());
        move(*newPoint);
    }
}

void StockStallionWindow::on_pushButton_clicked()
{
    //Network Request Code
    manager = new QNetworkAccessManager();
    QNetworkRequest request;
    //request.setUrl(new QUrl("https://www.alphavantage.co/query?function=TIME_SERIES_INTRADAY&symbol=APPL&interval=1min&apikey=CRINRUHDWCNFTVK2"));
    //manager->get(request);
    //QNetworkReply *reply = NULL;

}

void StockStallionWindow::requestRead(QNetworkReply *reply)
{
    QByteArray data;
    data = reply->readAll();
    //QString dataString = QString::fromC(data.data());
    //ui->dataTextBox->setText(dataString);
}

//Close window
void StockStallionWindow::on_pushButton_2_clicked()
{
    this->close();
}

//Minimize window
void StockStallionWindow::on_pushButton_3_clicked()
{
    QWidget::setWindowState(Qt::WindowMinimized);
}

void StockStallionWindow::on_portfolioButton_clicked()
{
    hideAllTabs();
    ui->portfolioTab->show();
    ui->portfolioButton->setStyleSheet("background-color:rgb(225, 225, 225); border-style: solid; border-color: black; font: 12pt \"Century Gothic\"");
    ui->profileButton->setStyleSheet("background-color:rgb(245, 245, 245); border-style: solid; border-color: black; font: 12pt \"Century Gothic\"");
    ui->aboutButton->setStyleSheet("background-color:rgb(245, 245, 245); border-style: solid; border-color: black; font: 12pt \"Century Gothic\"");
}

void StockStallionWindow::on_profileButton_clicked()
{
    hideAllTabs();
    ui->profileTab->show();
    ui->profileButton->setStyleSheet("background-color:rgb(225, 225, 225); border-style: solid; border-color: black; font: 12pt \"Century Gothic\"");
    ui->portfolioButton->setStyleSheet("background-color:rgb(245, 245, 245); border-style: solid; border-color: black; font: 12pt \"Century Gothic\"");
    ui->aboutButton->setStyleSheet("background-color:rgb(245, 245, 245); border-style: solid; border-color: black; font: 12pt \"Century Gothic\"");
}

void StockStallionWindow::on_aboutButton_clicked()
{
    hideAllTabs();
    ui->aboutTab->show();
    ui->aboutButton->setStyleSheet("background-color:rgb(225, 225, 225); border-style: solid; border-color: black; font: 12pt \"Century Gothic\"");
    ui->portfolioButton->setStyleSheet("background-color:rgb(245, 245, 245); border-style: solid; border-color: black; font: 12pt \"Century Gothic\"");
    ui->profileButton->setStyleSheet("background-color:rgb(245, 245, 245); border-style: solid; border-color: black; font: 12pt \"Century Gothic\"");
}

//Hides all tabs
void StockStallionWindow::hideAllTabs()
{
    ui->portfolioTab->hide();
    ui->profileTab->hide();
    ui->aboutTab->hide();
}

void StockStallionWindow::on_portfolioButton_pressed()
{
    ui->portfolioButton->setStyleSheet("background-color:rgb(225, 225, 225); border-style: solid; border-color: black; font: 12pt \"Century Gothic\"");
}

void StockStallionWindow::on_profileButton_pressed()
{
    ui->profileButton->setStyleSheet("background-color:rgb(225, 225, 225); border-style: solid; border-color: black; font: 12pt \"Century Gothic\"");
}

void StockStallionWindow::on_aboutButton_pressed()
{
    ui->aboutButton->setStyleSheet("background-color:rgb(225, 225, 225); border-style: solid; border-color: black; font: 12pt \"Century Gothic\"");
}

void StockStallionWindow::on_portfolioButton_released()
{
    ui->portfolioButton->setStyleSheet("background-color:rgb(245, 245, 245); border-style: solid; border-color: black; font: 12pt \"Century Gothic\"");
}

void StockStallionWindow::on_aboutButton_released()
{
    ui->aboutButton->setStyleSheet("background-color:rgb(245, 245, 245); border-style: solid; border-color: black; font: 12pt \"Century Gothic\"");
}

void StockStallionWindow::on_profileButton_released()
{
    ui->profileButton->setStyleSheet("background-color:rgb(245, 245, 245); border-style: solid; border-color: black; font: 12pt \"Century Gothic\"");
}

//Used to determine which user is using the form
void StockStallionWindow::setUserName(QString name)
{
    username = name;
    QString displayName = username;
    //displayName[0] = QString.toUpper(displayName[0]);
    ui->portfolioTab->setPortfolioText(displayName.left(1).toUpper() + displayName.right(displayName.length()-1) + "'s Portfolio:");
    ui->portfolioTab->setUsername(name);
    ui->profileTab->setUsername(name);
}

//Used to verfiy that the user is valid
void StockStallionWindow::setPassWord(QString pass)
{
    password = pass;
}
