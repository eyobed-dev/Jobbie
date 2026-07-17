#include "confirm_dialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

ConfirmDialog::ConfirmDialog(const QString &title, const QString &message, QWidget *parent)
    : QDialog(parent) {
    setWindowTitle(title);
    setFixedWidth(380);
    setModal(true);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(24, 24, 24, 20);
    layout->setSpacing(16);

    auto *headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(12);

    auto *iconLabel = new QLabel("⚠️", this);
    iconLabel->setStyleSheet("font-size: 24px;");
    headerLayout->addWidget(iconLabel);

    auto *titleLabel = new QLabel(title, this);
    titleLabel->setObjectName("DialogHeaderTitle");
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    layout->addLayout(headerLayout);

    auto *msgLabel = new QLabel(message, this);
    msgLabel->setWordWrap(true);
    msgLabel->setStyleSheet("color: #64748b; font-size: 13px; line-height: 1.4;");
    layout->addWidget(msgLabel);

    layout->addSpacing(8);

    auto *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(12);

    auto *cancelBtn = new QPushButton("Cancel", this);
    cancelBtn->setCursor(Qt::PointingHandCursor);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    auto *confirmBtn = new QPushButton("Delete", this);
    confirmBtn->setObjectName("DangerButton");
    confirmBtn->setCursor(Qt::PointingHandCursor);
    connect(confirmBtn, &QPushButton::clicked, this, &QDialog::accept);

    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(confirmBtn);

    layout->addLayout(btnLayout);
}
