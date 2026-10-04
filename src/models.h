#pragma once

#include <QString>
#include <QJsonObject>
#include <QDateTime>
#include <QUuid>

struct JobApplication {
    int applicationNumber = 0;
    QString id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    QString company;
    QString title;
    QString status = "Wishlist";
    QString jobType = "Full-time";
    QString salary;
    QString dateApplied = QDate::currentDate().toString("yyyy-MM-dd");
    QString link;
    QString folderPath; // Attached folder containing tailored CV and related documents
    QString notes;
    QString location = "Remote";
    QString createdAt = QDateTime::currentDateTime().toString(Qt::ISODate);
    QString updatedAt = QDateTime::currentDateTime().toString(Qt::ISODate);

    QJsonObject toJson() const;
    static JobApplication fromJson(const QJsonObject &obj);
};
