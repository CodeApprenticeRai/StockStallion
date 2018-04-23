#ifndef TAB_PROFILE_H
#define TAB_PROFILE_H

#include <QWidget>

namespace Ui {
class tab_Profile;
}

class tab_Profile : public QWidget
{
    Q_OBJECT

public:
    explicit tab_Profile(QWidget *parent = 0);
    void setUsername(QString name);
    void refresh();
    ~tab_Profile();

private:
    Ui::tab_Profile *ui;
    QString username;
};

#endif // TAB_PROFILE_H
