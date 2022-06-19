#include "findcontroller.h"

FindController::FindController(QObject *parent)
    : QObject{parent}
{

}

FindController::~FindController()
{
    if (finderThread_ != nullptr)
    {
        if (finderThread_->isRunning())
        {
            finderThread_->stop();
            finderThread_->terminate();
        }
        finderThread_.reset();
    }
}

void FindController::startSearch(QString dir, QString file)
{
    if (finderThread_.get())
    {
        finderThread_->stop();
        finderThread_->terminate();
        finderThread_.reset();
        return;
    }

    finderThread_ = QSharedPointer<FileFinder>::create(nullptr, dir, file);
    finderThread_->start(QThread::NormalPriority);
    connect(finderThread_.get(), &FileFinder::stopThread, this, &FindController::deleteThread);
    connect(finderThread_.get(), &FileFinder::writePath, this, &FindController::setCurrentPath);
    connect(finderThread_.get(), &FileFinder::targetFound, this, &FindController::generateStrTargetPath);
    emit newSearch();
}

void FindController::stopSearch()
{
    if (finderThread_.get())
    {
        finderThread_->stop();
        finderThread_->terminate();
        finderThread_.reset();
        emit notifySuccess(tr("Search complete."));
        return;
    }
}

void FindController::deleteThread()
{
    finderThread_->stop();
    finderThread_->terminate();
    finderThread_.reset();
    emit notifySuccess(tr("Search complete."));
}

void FindController::setCurrentPath(QString path)
{
    currentPath_ = path;
}

void FindController::generateStrTargetPath(QString target, bool isDir)
{
    QString result = currentPath_ + target;
    emit generateTargetPath(result, isDir);
}
