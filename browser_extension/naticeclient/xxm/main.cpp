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
    _setmode(_fileno(stdout), _O_BINARY);
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
    QTextStream cIn(stdin);
    QString msgStr = cIn.readLine();
    //    QFile tempWriteFile("a.txt");
    //    tempWriteFile.open(QIODevice::WriteOnly | QIODevice::Text);
    //    tempWriteFile.write(msgStr.toUtf8());

    //    QFile tempFile("/Users/xiaolunliu/Library/Application Support/Google/Chrome/NativeMessagingHosts/20190402141458440.txt");
    //    tempFile.open(QIODevice::ReadOnly | QIODevice::Text);
    //    QTextStream textinput(&tempFile);
    //    QString msgStr = textinput.readAll();
    //    tempFile.close();

    //获取数据
    qint32 index = msgStr.indexOf('{');
    if(index < 0){
        return 0;
    }
    msgStr = msgStr.mid(index);
    QJsonDocument doc = QJsonDocument::fromJson(msgStr.toUtf8());
    QJsonObject articleObj = doc.object();
    QString title = articleObj.take("title").toString();
    QString content = articleObj.take("content").toString();
    qint64 now = QDateTime::currentDateTime().toMSecsSinceEpoch();
    //数据入库
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
    //保存正文
    database.close();
    dir.mkdir(lastId);
    dir.cd(lastId);
    QFile file(dir.filePath("a.data"));
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(content.toUtf8());
    file.close();
    //回馈消息
    QString backJson ="{'ok':'true'}";
    QTextStream cOut(stdout);
    cOut<<backJson.length();
    cOut<<backJson;
    cOut.flush();
}
