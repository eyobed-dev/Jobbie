#pragma once

#include <QString>
#include <QWidget>

namespace NativeDialog {

QString getExistingDirectory(QWidget *parent, const QString &caption, const QString &startDir = QString());

QString getSaveFileName(QWidget *parent, const QString &caption, const QString &startDir = QString(), const QString &filter = QString());

QString getOpenFileName(QWidget *parent, const QString &caption, const QString &startDir = QString(), const QString &filter = QString());

} // namespace NativeDialog
