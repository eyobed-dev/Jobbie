#pragma once

#include <QWidget>
#include <QMap>
#include <QList>
#include "models.h"
#include "kanban_column.h"

class KanbanBoardWidget : public QWidget {
    Q_OBJECT

public:
    explicit KanbanBoardWidget(bool darkMode = false, QWidget *parent = nullptr);

    void setJobs(const QList<JobApplication> &jobs);
    void setDarkMode(bool darkMode);

signals:
    void jobMoved(const QString &jobId, const QString &newStatus);
    void addJobRequested(const QString &defaultStatus);
    void editJobRequested(const QString &jobId);
    void deleteJobRequested(const QString &jobId);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void initUI();

    bool m_darkMode;
    QMap<QString, KanbanColumnWidget*> m_columns;
};
