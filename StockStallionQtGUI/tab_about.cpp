#include "tab_about.h"
#include "ui_tab_about.h"

//Loads about tab. Initializes UI
tab_About::tab_About(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tab_About)
{
    ui->setupUi(this);

    //Stock Stallion Logo
    QPixmap logo(":/StockStallionImages/StockStallion Images/Stock Stallion Logo.png");
    ui->logo->setPixmap(logo.scaled(ui->logo->width(), ui->logo->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

//Destructor
tab_About::~tab_About()
{
    delete ui;
}
