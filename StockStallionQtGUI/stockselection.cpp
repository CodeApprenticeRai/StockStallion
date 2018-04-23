#include "stockselection.h"
#include "ui_stockselection.h"
#include <QMouseEvent>

//Initializes stock selection window.
StockSelection::StockSelection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StockSelection)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_QuitOnClose, false);

    //Make Window frameless
    Qt::WindowFlags flags = windowFlags();
    setWindowFlags(flags | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QWidget::setFixedSize(StockSelection::width(), StockSelection::height());

    //Colors the window white
    ui->centralWidget->setObjectName("centralWidget");
    ui->centralWidget->setStyleSheet(" #centralWidget {background-color: qlineargradient(x1:0, x2: 0, y1: 0, y2: .5, stop: 0 #84A9FF, stop: 0.28 #84A9FF, stop: 0.281 white, stop: 1 white); border-radius:5px; border:1px solid black}");

}

//Allows drag/drop on window
void StockSelection::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        isMouseDown = true;

        QRect pos = geometry();
        offset = new QPoint(event->globalPos().x() - pos.x(), event->globalPos().y() - pos.y());
    }
}

//Allows drag/drop on window
void StockSelection::mouseReleaseEvent(QMouseEvent *event)
{
    isMouseDown = false;
    offset = 0;
}

//Allows drag/drop on window
void StockSelection::mouseMoveEvent(QMouseEvent *event)
{
    if(isMouseDown)
    {
        QPoint *newPoint = new QPoint(event->globalPos().x() - offset->x(), event->globalPos().y() - offset->y());
        move(*newPoint);
    }
}

//Deconstructor
StockSelection::~StockSelection()
{
    delete ui;
}

//Closes window. Sends signal that it was canceled
void StockSelection::on_cancelButton_clicked()
{
    canceled = true;
    this->close();
}

//Colors button to signify press/release
void StockSelection::on_cancelButton_pressed()
{
    ui->cancelButton->setStyleSheet("background-color: Silver;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

//Colors button to signify press/release
void StockSelection::on_cancelButton_released()
{
    ui->cancelButton->setStyleSheet("background-color: Gainsboro;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

//Colors button to signify press/release
void StockSelection::on_addStockButton_pressed()
{
    ui->addStockButton->setStyleSheet("background-color: Silver;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

//Colors button to signify press/release
void StockSelection::on_addStockButton_released()
{
    ui->addStockButton->setStyleSheet("background-color: Gainsboro;"
                                   "border-style: solid;"
                                   "border-width:2px;"
                                   "border-radius:7px;"
                                   "border-color: black;"
                                   "font: 12pt \"Century Gothic\"");
}

//Closes window
void StockSelection::on_addStockButton_clicked()
{
    if(ui->tickerTextBox->text() == "")
        return;

    this->close();
}

//Returns the stock ticker
QString StockSelection::getTicker()
{
    return ui->tickerTextBox->text().toUpper();
}
