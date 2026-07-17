#pragma once

#include <QDialog>
#include <QString>

class ConfirmDialog : public QDialog {
    Q_OBJECT

public:
    explicit ConfirmDialog(const QString &title, const QString &message, QWidget *parent = nullptr);
};
