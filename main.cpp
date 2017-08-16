#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *codec=QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    tool::SetStyle();

    //加载中文字符
    QTranslator translator;
    translator.load(":/zh_CN");
    a.installTranslator(&translator);
    MainWindow w;
    w.show();

    return a.exec();
}
