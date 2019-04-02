#include <QCoreApplication>
#include <QFile>
#include <QTime>
#include <QTextStream>
#include <iostream>

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

    QTextStream s(stdin);
    QString value = s.readAll();

    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyyMMddhhmmsszzz");
    QFile file(current_date+".txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream fileInput(&file);
    fileInput << value << endl;
    file.close();
    return 0;
}
