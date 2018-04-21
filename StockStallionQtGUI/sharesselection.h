#ifndef SHARESSELECTION_H
#define SHARESSELECTION_H

#include <QDialog>

namespace Ui {
class SharesSelection;
}

class SharesSelection : public QDialog
{
    Q_OBJECT

public:
    explicit SharesSelection(QWidget *parent = 0);
    ~SharesSelection();

private:
    Ui::SharesSelection *ui;
};

#endif // SHARESSELECTION_H
