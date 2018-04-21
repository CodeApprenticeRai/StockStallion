#ifndef TAB_ABOUT_H
#define TAB_ABOUT_H

#include <QWidget>

namespace Ui {
class tab_About;
}

class tab_About : public QWidget
{
    Q_OBJECT

public:
    explicit tab_About(QWidget *parent = 0);
    ~tab_About();

private:
    Ui::tab_About *ui;
};

#endif // TAB_ABOUT_H
