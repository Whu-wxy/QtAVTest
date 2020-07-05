#include "zndkandroiddlg.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ZNDKAndroidDlg w;
    w.show();

    return a.exec();
}
