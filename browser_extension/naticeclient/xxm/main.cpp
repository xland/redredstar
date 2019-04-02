#include <QCoreApplication>
#include <QFile>
#include <QTime>
#include <QTextStream>
#include <iostream>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDir>
#include <QtSql>

#ifdef Q_OS_WIN32
#include <fcntl.h>
#include <io.h>
#endif

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
#ifdef Q_OS_WIN32
    _setmode(_fileno(stdin), _O_BINARY);
#endif
    QStringList locations = QStandardPaths::standardLocations(QStandardPaths::AppDataLocation);
    QDir dir(locations[0]);
    dir.cdUp();
    dir.cd("Electron");
    dir.cd("xxm");
    QString dbPath = dir.filePath("db");
    QFileInfo fileInfo(dbPath);
    if(!fileInfo.exists()){
        return 0;
    }
    QTextStream s(stdin);
    QString msgStr = s.readAll();
    //    QFile tempFile("/Users/xiaolunliu/Library/Application Support/Google/Chrome/NativeMessagingHosts/20190402141458440.txt");
    //    tempFile.open(QIODevice::ReadOnly | QIODevice::Text);
    //    QTextStream textinput(&tempFile);
    //    QString msgStr = textinput.readAll();
    //    tempFile.close();
    qint32 index = msgStr.indexOf('{');
    msgStr = msgStr.mid(index);
    QJsonDocument doc = QJsonDocument::fromJson(msgStr.toUtf8());
    QJsonObject articleObj = doc.object();
    QString title = articleObj.take("title").toString();
    QString content = articleObj.take("content").toString();
    qint64 now = QDateTime::currentDateTime().toMSecsSinceEpoch();
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(dbPath);
    database.open();
    QSqlQuery query;
    query.prepare("INSERT INTO articles  (\"title\", \"created_at\",\"updated_at\",\"editor_type\") VALUES (:title, :created_at, :updated_at,:editor_type)");
    query.bindValue(":title",title);
    query.bindValue(":created_at",now);
    query.bindValue(":updated_at",now);
    query.bindValue(":editor_type","html");
    query.exec();
    QString lastId = query.lastInsertId().toString();
    database.close();
    dir.mkdir(lastId);
    dir.cd(lastId);
    QFile file(dir.filePath("a.data"));
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(content.toUtf8());
    file.close();
    return 0;
}
