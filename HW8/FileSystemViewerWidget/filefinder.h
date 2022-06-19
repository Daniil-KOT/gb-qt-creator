#ifndef FILEFINDER_H
#define FILEFINDER_H

#include <QObject>
#include <QThread>

class FileFinder : public QThread
{
    Q_OBJECT
public:
    explicit FileFinder(QObject *parent = nullptr, QString dir = "", QString searchTarget = "");
    void stop();

protected:
    void run() override;

signals:
    void stopThread();
    void writePath(QString);
    void targetFound(QString, bool isDir);

private:
    QString dir_;
    QString searchTarget_;
    bool isWorking_ = false;
};

#endif // FILEFINDER_H
