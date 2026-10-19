#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QIcon>
#include "storage.h"
#include "main_window.h"

int main(int argc, char *argv[]) {
    // Enable High DPI scaling and native OS desktop portals
    qputenv("QT_USE_PORTAL", "1");
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication app(argc, argv);
    app.setStyle("Fusion");
    app.setApplicationName("Jobbie");
    app.setApplicationDisplayName("Jobbie");
    app.setOrganizationName("Jobbie");
    app.setWindowIcon(QIcon(":/icons/icon.png"));

    QCommandLineParser parser;
    parser.setApplicationDescription("Jobbie — Job Application Tracker");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption darkOption("dark", "Launch in dark mode.");
    parser.addOption(darkOption);

    QCommandLineOption dataOption("data", "Custom path to jobs.json", "path");
    parser.addOption(dataOption);

    parser.process(app);

    bool darkMode = parser.isSet(darkOption);
    QString dataPath = parser.value(dataOption);
    if (dataPath.isEmpty()) {
        dataPath = QDir::current().filePath("jobs.json");
    }

    StorageManager storage(dataPath);
    MainWindow window(storage, darkMode);
    window.showMaximized();

    return app.exec();
}
