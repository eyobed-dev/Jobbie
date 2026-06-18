#pragma once

#include <QFrame>
#include <QPoint>
#include "models.h"

class JobCardWidget : public QFrame {
    Q_OBJECT

public:
    explicit JobCardWidget(const JobApplication &job, bool darkMode = false, QWidget *parent = nullptr);

    const JobApplication& job() const { return m_job; }

signals:
    void editRequested(const QString &jobId);
    void deleteRequested(const QString &jobId);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void openLink();
    void openFolder();

private:
    void initUI();

    JobApplication m_job;
    bool m_darkMode;
    QPoint m_dragStartPos;
};
