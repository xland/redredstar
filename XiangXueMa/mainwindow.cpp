#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


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
    bool isDirExist(QString fullPath)
    {
        QDir dir(fullPath);
        if(dir.exists())
        {
          return true;
        }
        else
        {
           bool ok = dir.mkpath(fullPath);//创建多级目录
           return ok;
        }
    }
    qDebug(qPrintable(strs.at(0)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

