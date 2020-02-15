#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTreeWidgetItem* twi = new QTreeWidgetItem();
    twi->setText(0,"这是分类标题");
    QTreeWidgetItem* twi2 = new QTreeWidgetItem();
    twi2->setText(0,"这是分类标题");
    twi->addChild(twi2);
    ui->treeWidget->addTopLevelItem(twi);

    IconHelper::Instance()->SetIcon(ui->pushButton,QChar(0xe600),16);
    QStringList strs = QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation);
    QDir dir(strs.at(0));
    dir.mkpath(strs.at(0));//创建多级目录
    qDebug(qPrintable(strs.at(0)));

    QFile::copy(":/db/db.db", strs.at(0)+"/db.db");

    //    QSqlDatabase database;
    //    database = QSqlDatabase::addDatabase("QSQLITE");
    //    database.setDatabaseName(strs.at(0)+"//MyDataBase.db");
    //    if (!database.open())
    //    {
    //        qDebug() << "Error: Failed to connect database." << database.lastError();
    //    }
    //    else
    //    {
    //        qDebug() << "Succeed to connect database." ;
    //    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

