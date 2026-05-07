#include <QApplication>
#include "StartWidget.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    StartWidget w;
    w.show();
    return a.exec();
}

