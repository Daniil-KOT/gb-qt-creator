#include "filesystemviewer.h"

#include <QDir>
#include <QMenu>
#include <QAction>
#include <QMessageBox>

FileSystemViewer::FileSystemViewer(QWidget *parent) :
    QWidget{parent},
    model_(nullptr)
{
    layout_ = new QGridLayout();
    this->setLayout(layout_);

    addrLine_ = new QLineEdit();
    search_ = new QLineEdit();
    QPushButton* startSearch = new QPushButton(QIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowRight)), "");
    translateBtn_ = new QPushButton(QIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload)), tr("Translate"));

    list_ = new QListView(this);
    list_->setViewMode(QListView::IconMode);
    list_->setSpacing(10);
    list_->setTextElideMode(Qt::ElideNone);
    list_->setWordWrap(true);

    layout_->addWidget(addrLine_, 1, 1, 1, 7);
    layout_->setHorizontalSpacing(10);
    layout_->addWidget(search_, 1, 8, 1, 2);
    layout_->addWidget(startSearch, 1, 10, 1, 1);
    layout_->addWidget(translateBtn_, 12, 10, 1, 1);
    layout_->addWidget(list_, 2, 0, 10, 11);

    this->setMinimumSize(960, 540);

#ifdef Q_OS_WIN

    driveSelector_ = new QComboBox(this);
    QFileInfoList drives = QDir::drives();

    for (const QFileInfo &drive : drives)
    {
        driveSelector_->addItem(drive.path());
    }

    if (drives.count() > 0)
    {
        rebuildModel(drives.at(0).path());
    }

    driveSelector_->setMaximumSize(40, 25);
    layout_->addWidget(driveSelector_, 1, 0, 1, 1);

    connect(driveSelector_, &QComboBox::activated,
            this, &FileSystemViewer::chngDisk);

#elif Q_OS_UNIX

    rootDirBtn_ = new QPushButton(this);
    rootDirBtn_->setText("/");

    layout_->addWidget(rootDirBtn_, 1, 0, 1, 1);

    connect(rootDirBtn_, &QPushButton::clicked,
            this, &FileSystemViewer::goRootPath);

    rebuildModel("/");

#endif

    controller_ = new FindController(this);

    translate("en");

    connect(controller_, &FindController::notifySuccess, this, &FileSystemViewer::popUpInfo);
    connect(controller_, &FindController::generateTargetPath, this, &FileSystemViewer::targetFound);
    connect(controller_, &FindController::newSearch, model_, &QStandardItemModel::clear);

    connect(list_, &QListView::doubleClicked, this, &FileSystemViewer::changeDirectory);
    connect(addrLine_, &QLineEdit::returnPressed, this, &FileSystemViewer::changeDirectoryAddrLine);
    connect(search_, &QLineEdit::returnPressed, this, &FileSystemViewer::findFile);
    connect(startSearch, &QPushButton::clicked, this, &FileSystemViewer::findFile);
    connect(translateBtn_, &QPushButton::clicked, this, &FileSystemViewer::switchLang);
}

void FileSystemViewer::setNewModel(QStandardItemModel *newModel)
{
    list_->setModel(newModel);
    model_ = newModel;
}

void FileSystemViewer::rebuildModel(QString path)
{
    QStandardItemModel* model = new QStandardItemModel(this);

    QDir src(path);

    if (!src.exists())
    {
        QMessageBox::information(this, tr("Warning"), tr("No such file or directory!"));
        addrLine_->setText(currentPath_);
        delete model;
        return;
    }

    currentPath_ = src.absolutePath();

    src.setFilter(QDir::Hidden | QDir::NoSymLinks | QDir::Dirs);
    QStringList list = src.entryList();

    for (const QString &dir : list)
    {
        QStandardItem* folderItem = new QStandardItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_DirIcon)), dir);
        folderItem->setSizeHint(QSize(80, 80));
        model->appendRow(folderItem);
    }

    src.setFilter(QDir::Hidden | QDir::NoSymLinks | QDir::Files);
    list = src.entryList();

    for (const QString &file : list)
    {
        QStandardItem* fileItem = new QStandardItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon)), file);
        fileItem->setSizeHint(QSize(80, 80));
        model->appendRow(fileItem);
    }

    if (!foundDirs_.empty())
    {
        foundDirs_.clear();
    }

    setNewModel(model);
}

void FileSystemViewer::chngDisk(int index)
{
    controller_->stopSearch();
    currentPath_ = "";
    QFileInfoList drives = QDir::drives();
    rebuildModel(drives.at(index).path());
    addrLine_->clear();
    addrLine_->setText(drives.at(index).path());
}

void FileSystemViewer::goRootPath()
{
    controller_->stopSearch();
    rebuildModel("/");
    addrLine_->clear();
    addrLine_->setText("/");
}

void FileSystemViewer::changeDirectory(const QModelIndex &index)
{
    controller_->stopSearch();

    if (!foundDirs_.empty() && foundDirs_.contains(index))
    {
        rebuildModel(foundDirs_.value(index));
    }
    else
    {
        rebuildModel(currentPath_ + "/" + index.data().toString());
    }

    addrLine_->clear();
    addrLine_->setText(currentPath_);
}

void FileSystemViewer::changeDirectoryAddrLine()
{
    if (addrLine_->text().isEmpty())
    {
        QMessageBox::information(this, tr("Warning"), tr("Address line is empty!"));
        return;
    }

    controller_->stopSearch();
    rebuildModel(addrLine_->text());
}

void FileSystemViewer::findFile()
{
    QString search = search_->text();
    if (search.length() == 0)
    {
        return;
    }
    model_->clear();
    controller_->startSearch(driveSelector_->currentText(), search);
}

void FileSystemViewer::popUpInfo(QString info)
{
    QMessageBox::information(this, tr("Success"), info);
}

void FileSystemViewer::targetFound(QString path, bool isDir)
{
    QDir res(path);
    QIcon icon(isDir ? QApplication::style()->standardIcon(QStyle::SP_DirIcon) :
                       QApplication::style()->standardIcon(QStyle::SP_FileIcon));

    QStandardItem* item = new QStandardItem(icon, res.dirName());
    item->setSizeHint(QSize(80, 80));
    model_->appendRow(item);

    if (isDir)
    {
        foundDirs_.insert(model_->indexFromItem(item), path);
    }

    setNewModel(model_);
}

void FileSystemViewer::switchLang()
{
    if ("ru_RU" == translator_.language())
    {
        translate("en");
    }
    else
    {
        translate("ru");
    }
}

void FileSystemViewer::translate(QString lang)
{
    if (translator_.load(":/Resource/filesviewer_" + lang))
    {
        qApp->installTranslator(&translator_);
    }
    translateBtn_->setText(tr("Translate"));
}
