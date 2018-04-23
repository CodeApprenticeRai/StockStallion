#include "tab_profile.h"
#include "ui_tab_profile.h"
#include <QtSql>
tab_Profile::tab_Profile(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tab_Profile)
{
    ui->setupUi(this);
}

void tab_Profile::setUsername(QString name)
{
    username = name;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("stockstallion.db");
    if(db.open())
    {
        QSqlQuery query;
        QSqlTableModel *model = new QSqlTableModel(this);

        model->setTable("transactions");
        model->select();

        int totalShares = 0;
        int totalStocks = 0;
        QString tickerMostShares = "";
        int mostShares = 0;
        for(int i = 0; i < model->rowCount(); ++i)
        {
            if(model->record(i).value("username").toString() == username && model->record(i).value("has_been_sold").toString() == "0")
            {
                totalStocks++;
                int shares = model->record(i).value("num_Shares").toInt();
                if(shares > mostShares)
                {
                    tickerMostShares = model->record(i).value("ticker").toString();
                    mostShares = shares;
                }
                totalShares += shares;
            }
        }
        ui->registeredSinceLabel->setText("Registered since: " + QDate::currentDate().toString());
        ui->stocksOwnedLabel->setText("Stocks Owned: " + QString::number(totalStocks));
        ui->totalStocksOwnedLabel->setText("Total Shares Owned: " + QString::number(totalShares));
        ui->mostStockShareLabel->setText("Stock With Most Shares Owned: " + tickerMostShares + " (" + QString::number(mostShares) + " Shares)");
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setAttribute(Qt::WA_QuitOnClose, false);
        msgBox.setText("Unable to access database");
        msgBox.exec();
        return;
    }
}

tab_Profile::~tab_Profile()
{
    delete ui;
}
