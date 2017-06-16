// В файле main.cpp:

#include <QDesktopWidget>
         
// потом тут всякая фигня, потом создаётся главное окно, например:
         
MainWindow w;
         
// и вот дальше настраивается
         
QRect screenGeometry = QApplication::desktop()->screenGeometry();
int x = (screenGeometry.width()-w.width()) / 2;
int y = (screenGeometry.height()-w.height()) / 2;
w.move(x, y);
         
// теперь можно уже показать окно
         
w.show();

