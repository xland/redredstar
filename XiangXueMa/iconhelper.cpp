#include "iconhelper.h"

IconHelper* IconHelper::_instance = 0;
IconHelper::IconHelper(QObject *parent):QObject(parent)
{
    int fontId = QFontDatabase::addApplicationFont(":/font/iconfont.ttf");
    QString fontName = QFontDatabase::applicationFontFamilies(fontId).at(0);
    iconFont = QFont(fontName);
}
void IconHelper::SetIcon(QLabel *lab, QChar c, int size)
{
    iconFont.setPointSize(size);
    lab->setFont(iconFont);
    lab->setText(c);
}

void IconHelper::SetIcon(QPushButton *btn, QChar c, int size)
{
    iconFont.setPointSize(size);
    btn->setFont(iconFont);
    btn->setText(c);
}

//QIcon* IconHelper::CreateIcon(){
//    //准备把图形字体绘制到按钮上
//    QPixmap pix(80, 70);
//    pix.fill(Qt::transparent);
//    //设置画笔
//    QPainter painter;
//    painter.begin(&pix);
//    //消除按钮按下的效果
//    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
//    //设置画笔颜色
//    painter.setPen(QColor("#1570A5"));
//    //设置刷子颜色
//    painter.setBrush(QColor("#1570A5"));
//    //设置图形大小，这个大小主要取决与icon大小，icon设置的比较小，这个设置再大也没用
//    iconFont.setPointSize(50);
//    painter.setFont(iconFont);
//    //画图形字体
//    painter.drawText(pix.rect(), Qt::AlignCenter, QChar(0xf11b));
//    painter.end();
//    return new QIcon(pix);
//}
