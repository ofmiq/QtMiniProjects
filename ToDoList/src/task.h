#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class Task
{
public:
    Task(int id, const QString& description, const QString& priority, const QDate& deadline, bool completed);

    int getId() const;
    QString getDescription() const;
    QString getPriority() const;
    QDate getDeadline() const;

    bool isCompleted() const;
    void setCompleted(bool completed);
    bool saveToDatabase();
    bool updateInDatabase();
    bool deleteFromDatabase();

private:
    int id;
    QString description;
    QString priority;
    QDate deadline;
    bool completed;
};

#endif // TASK_H
