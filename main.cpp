#include <QtWidgets>

#include "mygui.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MyGUI myGUI;
    myGUI.init(APP_PRODUCT, APP_VERSION);
    myGUI.makeConnections();
    myGUI.show();

    return app.exec();
}
