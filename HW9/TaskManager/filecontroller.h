#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QObject>
#include <QFile>

class FileController : public QObject
{
    Q_OBJECT
public:
    explicit FileController(QObject *parent = nullptr);
    virtual ~FileController();
    Q_INVOKABLE
    void getNextTask();
    Q_INVOKABLE
    void saveData(QString taskName, QString deadline, int progress);
    void writeDataVec(QString taskName, QString deadline, int progress);

signals:
    void loadTask(QString taskName, QString deadline, int progress);

private:
    QFile* file_;
    QVector<QString> taskNames_;
    QVector<QString> deadlines_;
    QVector<int> progresses_;
};

#endif // FILECONTROLLER_H
