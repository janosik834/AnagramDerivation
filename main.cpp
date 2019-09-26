#include "mainwindow.h"
#include <QApplication>
#include "anagramderivation.h"
#include <iostream>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Anagrams w;


    w.show();


    return a.exec();
}
