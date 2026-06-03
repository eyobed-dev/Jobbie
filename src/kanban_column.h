#pragma once

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QList>
#include "models.h"
#include "job_card.h"

class KanbanColumnWidget : public QFrame {
    Q_OBJECT

public:
    explicit KanbanColumnWidget(const QString &status, bool darkMode = false, QWidget *parent = nullptr);

    const QString& status() const { return m_status; }
    void setJobs(const QList<JobApplication> &jobs);
    void setDarkMode(bool darkMode);

signals:
    void jobDropped(const QString &jobId, const QString &newStatus);
    void addRequested(const QString &status);
    void editRequested(const QString &jobId);
    void deleteRequested(const QString &jobId);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    void initUI();

    QString m_status;
    bool m_darkMode;
    QLabel *m_countBadge = nullptr;
    QScrollArea *m_scrollArea = nullptr;
    QWidget *m_cardsContainer = nullptr;
    QVBoxLayout *m_cardsLayout = nullptr;
    QList<JobCardWidget*> m_cards;
};
