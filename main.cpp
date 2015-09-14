#include "videoplayer.h"
#include <QApplication>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    VideoPlayer w;

    if(argc > 1)
    {
        w.playSelectedFile(argv[1]);
    }

    QString appHomeDir = app.applicationDirPath();
    QFile File(appHomeDir + "/styles.qss");
    QFileInfo fileInfo(File);

    QString StyleSheet;
    if(File.open(QFile::ReadOnly | QFile::Text) == false)
    {
        QMessageBox::information(0,"error",File.errorString() + " " + fileInfo.absoluteFilePath());
    }

    StyleSheet = QLatin1String(File.readAll());
    File.close();
    app.setStyleSheet(StyleSheet);

    w.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    w.showMaximized();

    return app.exec();
}
