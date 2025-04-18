#include "MainWindow.h"
#include "SettingsDialog.h"
#include "Settings.h"

#include <QApplication>

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

    if(!gSettings.load())
    {
        SettingsDialog settingsDialog;
        settingsDialog.exec();
        return 0;
    }

    MainWindow w;
    w.dbgStartServer();
    dbgStartSecondClient(static_cast<void*>(&w));
    w.initClient(gSettings);
    w.show();
    return a.exec();
}
