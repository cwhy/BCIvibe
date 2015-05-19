#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QMessageBox::critical(this, "", "You're using Qt < 5, the realtime data demo needs functions that are available with Qt 4.7 to work properly");
#endif
  QApplication a(argc, argv);
  MainWindow w;
  w.showMaximized();

  return a.exec();
}
