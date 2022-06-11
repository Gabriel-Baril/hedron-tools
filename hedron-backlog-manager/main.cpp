#include "BacklogManager.h"
#include <QtWidgets/QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    BacklogManager w;
    w.show();



    return a.exec();
}
