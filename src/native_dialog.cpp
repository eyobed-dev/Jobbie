#include "native_dialog.h"

#include <QStandardPaths>
#include <QProcess>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>

namespace NativeDialog {

static QString runCommand(const QString &program, const QStringList &args, int &exitCode) {
    QProcess process;
    process.start(program, args);
    process.waitForFinished(-1);
    exitCode = process.exitCode();
    if (exitCode == 0) {
        return QString::fromUtf8(process.readAllStandardOutput()).trimmed();
    }
    return QString();
}

QString getExistingDirectory(QWidget *parent, const QString &caption, const QString &startDir) {
    QString dir = startDir;
    if (dir.isEmpty()) {
        dir = QDir::homePath();
    }

    // 1. Try native KDE kdialog (matches user's KDE Plasma desktop)
    QString kdialogPath = QStandardPaths::findExecutable("kdialog");
    if (!kdialogPath.isEmpty()) {
        int exitCode = -1;
        QStringList args = {"--title", caption, "--getexistingdirectory", dir};
        QString result = runCommand(kdialogPath, args, exitCode);
        if (exitCode == 0) {
            return result;
        } else if (exitCode == 1) {
            // User intentionally pressed cancel
            return QString();
        }
    }

    // 2. Try native GNOME/GTK zenity
    QString zenityPath = QStandardPaths::findExecutable("zenity");
    if (!zenityPath.isEmpty()) {
        int exitCode = -1;
        QStringList args = {"--file-selection", "--directory", "--title=" + caption, "--filename=" + dir + "/"};
        QString result = runCommand(zenityPath, args, exitCode);
        if (exitCode == 0) {
            return result;
        } else if (exitCode == 1) {
            return QString();
        }
    }

    // 3. Fallback to Qt native dialog helper
    return QFileDialog::getExistingDirectory(parent, caption, dir, QFileDialog::ShowDirsOnly);
}

QString getSaveFileName(QWidget *parent, const QString &caption, const QString &startDir, const QString &filter) {
    QString dir = startDir;
    if (dir.isEmpty()) {
        dir = QDir::homePath();
    }

    // 1. Try native KDE kdialog
    QString kdialogPath = QStandardPaths::findExecutable("kdialog");
    if (!kdialogPath.isEmpty()) {
        int exitCode = -1;
        QStringList args = {"--title", caption, "--getsavefilename", dir, filter};
        QString result = runCommand(kdialogPath, args, exitCode);
        if (exitCode == 0) {
            return result;
        } else if (exitCode == 1) {
            return QString();
        }
    }

    // 2. Try native GNOME/GTK zenity
    QString zenityPath = QStandardPaths::findExecutable("zenity");
    if (!zenityPath.isEmpty()) {
        int exitCode = -1;
        QStringList args = {"--file-selection", "--save", "--confirm-overwrite", "--title=" + caption, "--filename=" + dir};
        QString result = runCommand(zenityPath, args, exitCode);
        if (exitCode == 0) {
            return result;
        } else if (exitCode == 1) {
            return QString();
        }
    }

    // 3. Fallback to Qt native dialog helper
    return QFileDialog::getSaveFileName(parent, caption, dir, filter);
}

QString getOpenFileName(QWidget *parent, const QString &caption, const QString &startDir, const QString &filter) {
    QString dir = startDir;
    if (dir.isEmpty()) {
        dir = QDir::homePath();
    }

    // 1. Try native KDE kdialog
    QString kdialogPath = QStandardPaths::findExecutable("kdialog");
    if (!kdialogPath.isEmpty()) {
        int exitCode = -1;
        QStringList args = {"--title", caption, "--getopenfilename", dir, filter};
        QString result = runCommand(kdialogPath, args, exitCode);
        if (exitCode == 0) {
            return result;
        } else if (exitCode == 1) {
            return QString();
        }
    }

    // 2. Try native GNOME/GTK zenity
    QString zenityPath = QStandardPaths::findExecutable("zenity");
    if (!zenityPath.isEmpty()) {
        int exitCode = -1;
        QStringList args = {"--file-selection", "--title=" + caption, "--filename=" + dir};
        QString result = runCommand(zenityPath, args, exitCode);
        if (exitCode == 0) {
            return result;
        } else if (exitCode == 1) {
            return QString();
        }
    }

    // 3. Fallback to Qt native dialog helper
    return QFileDialog::getOpenFileName(parent, caption, dir, filter);
}

} // namespace NativeDialog
