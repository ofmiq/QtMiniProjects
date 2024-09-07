#include "task.h"

Task::Task(int id, const QString& description, const QString& priority, const QDate& deadline, bool completed)
    : id(id), description(description), priority(priority), deadline(deadline), completed(completed) {}

int Task::getId() const { return id; }

QString Task::getDescription() const { return description; }

QString Task::getPriority() const { return priority; }

QDate Task::getDeadline() const { return deadline; }

bool Task::isCompleted() const { return completed; }

void Task::setCompleted(bool completed) { this->completed = completed; }

bool Task::saveToDatabase()
{
    QSqlQuery query;

    query.prepare("INSERT INTO tasks (description, priority, deadline, completed) "
                  "VALUES (:description, :priority, :deadline, :completed)");
    query.bindValue(":description", description);
    query.bindValue(":priority", priority);
    query.bindValue(":deadline", deadline.toString("yyyy-MM-dd"));
    query.bindValue(":completed", completed);

    if (!query.exec()) {
        qDebug() << "Failed to save task to database:" << query.lastError().text();
        return false;
    }

    id = query.lastInsertId().toInt();

    return true;
}

bool Task::updateInDatabase()
{
    QSqlQuery query;

    query.prepare("UPDATE tasks SET description = :description, priority = :priority, "
                  "deadline = :deadline, completed = :completed WHERE id = :id");
    query.bindValue(":description", description);
    query.bindValue(":priority", priority);
    query.bindValue(":deadline", deadline.toString("yyyy-MM-dd"));
    query.bindValue(":completed", completed);
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Failed to update task in database:" << query.lastError().text();
        return false;
    }

    return true;
}

bool Task::deleteFromDatabase()
{
    QSqlQuery query;

    query.prepare("DELETE FROM tasks WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Failed to delete task from database:" << query.lastError().text();
        return false;
    }

    return true;
}
