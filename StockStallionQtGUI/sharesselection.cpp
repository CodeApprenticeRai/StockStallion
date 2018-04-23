#include "sharesselection.h"
#include "ui_sharesselection.h"
#include <QMouseEvent>

//Initializes window
SharesSelection::SharesSelection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SharesSelection)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_QuitOnClose, false);

    //Make Window frameless
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QWidget::setFixedSize(SharesSelection::width(), SharesSelection::height());

    //Colors the window white
    ui->centralWidget->setObjectName("centralWidget");
    ui->centralWidget->setStyleSheet(" #centralWidget {background-color: qlineargradient(x1:0, x2: 0, y1: 0, y2: .5, stop: 0 #84A9FF, stop: 0.28 #84A9FF, stop: 0.281 white, stop: 1 white); border-radius:5px; border:1px solid black}");
}

//Updates the total value of the shares and stock
void SharesSelection::on_sharesSpinBox_valueChanged(const QString &arg1)
{
    ui->totalValueLabel->setText("Total Value: $" + QString::number(price*ui->sharesSpinBox->value()));
}

//Allows drag/drop on window
void SharesSelection::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        isMouseDown = true;

        QRect pos = geometry();
        offset = new QPoint(event->globalPos().x() - pos.x(), event->globalPos().y() - pos.y());
    }
}

//Allows drag/drop on window
void SharesSelection::mouseReleaseEvent(QMouseEvent *event)
{
    isMouseDown = false;
    offset = 0;
}

//Allows drag/drop on window
void SharesSelection::mouseMoveEvent(QMouseEvent *event)
{
    if(isMouseDown)
    {
        QPoint *newPoint = new QPoint(event->globalPos().x() - offset->x(), event->globalPos().y() - offset->y());
        move(*newPoint);
    }
}

//Destructor
SharesSelection::~SharesSelection()
{
    delete ui;
}

//Closes window. Sends signal that window was canceled
void SharesSelection::on_cancelButton_clicked()
{
    canceled = true;
    this->close();
}

//Returns the amount of shares chosen by user
int SharesSelection::getShares()
{
    return ui->sharesSpinBox->value();
}

//Sets the current price of one share of a stock
void SharesSelection::setPrice(double stockPrice)
{
    price = stockPrice;
    ui->totalValueLabel->setText("Total Value: $" + QString::number(price));
}

//Colors button based on whether it was pressed/release
void SharesSelection::on_cancelButton_pressed()
{
    ui->cancelButton->setStyleSheet("background-color: Silver;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

//Colors button based on whether it was pressed/release
void SharesSelection::on_cancelButton_released()
{
    ui->cancelButton->setStyleSheet("background-color: Gainsboro;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

//Colors button based on whether it was pressed/release
void SharesSelection::on_addStockButton_pressed()
{
    ui->addStockButton->setStyleSheet("background-color: Silver;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

//Colors button based on whether it was pressed/release
void SharesSelection::on_addStockButton_released()
{
    ui->addStockButton->setStyleSheet("background-color: Gainsboro;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

//Closes window normally
void SharesSelection::on_addStockButton_clicked()
{
    this->close();
}
