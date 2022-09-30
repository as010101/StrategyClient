#include "widget.h"

#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QTextEdit>
#include <QListView>



//#define TEXTT
#define LIST_VIEW

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;

    w.show();
    return a.exec();
}
