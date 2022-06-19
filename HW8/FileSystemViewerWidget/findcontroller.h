#ifndef FINDCONTROLLER_H
#define FINDCONTROLLER_H

#include <QObject>
#include <QSharedPointer>

#include <filefinder.h>

class FindController : public QObject
{
    Q_OBJECT
public:
    explicit FindController(QObject *parent = nullptr);
    virtual ~FindController();
    void startSearch(QString dir, QString file);
    void stopSearch();

signals:
    void notifySuccess(QString);
    void generateTargetPath(QString, bool isDir);
    void newSearch();

public slots:
    void deleteThread();
    void setCurrentPath(QString path);
    void generateStrTargetPath(QString target, bool isDir);

private:
    QSharedPointer<FileFinder> finderThread_ = nullptr;
    QString currentPath_ = "";
};

#endif // FINDCONTROLLER_H
