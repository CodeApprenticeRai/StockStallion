#include "tab_profile.h"
#include "ui_tab_profile.h"

tab_Profile::tab_Profile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tab_Profile)
{
    ui->setupUi(this);
}

tab_Profile::~tab_Profile()
{
    delete ui;
}
