#include "filefinder.h"

#include <QDir>

FileFinder::FileFinder(QObject *parent, QString dir, QString searchTarget_)
    : QThread{parent},
      dir_(dir),
      searchTarget_(searchTarget_),
      isWorking_(true)
{}

void FileFinder::stop()
{
    isWorking_ = false;
}

void FileFinder::run()
{
    QStringList dirs = {dir_};

    while (dirs.count() && isWorking_)
    {
        QDir search(dirs.at(0));
        emit writePath(dirs.at(0));

        search.setFilter(QDir::Hidden | QDir::Dirs | QDir::NoSymLinks);
        QStringList fndDirs = search.entryList();

        for (int i = 0; i < fndDirs.count() && isWorking_; ++i)
        {
            QString newPath = dirs.at(0) + fndDirs.at(i) + "/";

            if (newPath.indexOf("/.") == -1)
            {
                dirs << newPath;
            }

            if (fndDirs.at(i).contains(searchTarget_))
            {
                emit targetFound(fndDirs.at(i), true);
            }
        }

        search.setFilter(QDir::Hidden | QDir::Files | QDir::NoSymLinks);
        QStringList fndFiles = search.entryList();

        for (int i = 0; i < fndFiles.count() && isWorking_; ++i)
        {
            QString filename = fndFiles.at(i);
            if (filename.indexOf(searchTarget_) != -1)
            {
                emit targetFound(filename, false);
            }
        }

        dirs.removeAt(0);
    }

    emit stopThread();
}
