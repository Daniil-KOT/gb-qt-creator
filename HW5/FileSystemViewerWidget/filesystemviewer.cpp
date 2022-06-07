#include "filesystemviewer.h"

#include <QDir>
#include <QAction>
#include <QMessageBox>

FileSystemViewer::FileSystemViewer(QWidget *parent) :
    QWidget{parent},
    model_(nullptr)
{
    layout_ = new QGridLayout();
    this->setLayout(layout_);

    addrLine_ = new QLineEdit();

    list_ = new QListView(this);
    list_->setViewMode(QListView::IconMode);
    list_->setSpacing(10);
    list_->setTextElideMode(Qt::ElideNone);
    list_->setWordWrap(true);

    layout_->addWidget(addrLine_, 0, 1, 1, 9);
    layout_->addWidget(list_, 1, 0, 10, 10);

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
    layout_->addWidget(driveSelector_, 0, 0, 1, 1);

    connect(driveSelector_, &QComboBox::activated,
            this, &FileSystemViewer::chngDisk);

#elif Q_OS_UNIX

    rootDirBtn_ = new QPushButton(this);
    rootDirBtn_->setText("/");

    layout_->addWidget(rootDirBtn_, 0, 0, 1, 1);

    connect(rootDirBtn_, &QPushButton::clicked,
            this, &FileSystemViewer::goRootPath);

    rebuildModel("/");

#endif

    connect(list_, &QListView::doubleClicked, this, &FileSystemViewer::changeDirectory);
    connect(addrLine_, &QLineEdit::returnPressed, this, &FileSystemViewer::changeDirectoryAddrLine);
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
        QMessageBox::information(this, "Warning", "No such file or directory!");
        addrLine_->setText(currentPath_);
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

    setNewModel(model);
}

void FileSystemViewer::chngDisk(int index)
{
    currentPath_ = "";
    QFileInfoList drives = QDir::drives();
    rebuildModel(drives.at(index).path());
    addrLine_->clear();
    addrLine_->setText(drives.at(index).path());
}

void FileSystemViewer::goRootPath()
{
    rebuildModel("/");
    addrLine_->clear();
    addrLine_->setText("/");
}

void FileSystemViewer::changeDirectory(const QModelIndex &index)
{
    rebuildModel(currentPath_ + "/" + index.data().toString());
    addrLine_->clear();
    addrLine_->setText(currentPath_);
}

void FileSystemViewer::changeDirectoryAddrLine()
{
    if (addrLine_->text().isEmpty())
    {
        QMessageBox::information(this, "Warning", "Address line is empty!");
        return;
    }

    rebuildModel(addrLine_->text());
}
