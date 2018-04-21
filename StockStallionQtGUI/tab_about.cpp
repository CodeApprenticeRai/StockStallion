#include "tab_about.h"
#include "ui_tab_about.h"

tab_About::tab_About(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tab_About)
{
    ui->setupUi(this);
}

tab_About::~tab_About()
{
    delete ui;
}
