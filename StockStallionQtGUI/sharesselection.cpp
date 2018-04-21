#include "sharesselection.h"
#include "ui_sharesselection.h"

SharesSelection::SharesSelection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SharesSelection)
{
    ui->setupUi(this);
}

SharesSelection::~SharesSelection()
{
    delete ui;
}
