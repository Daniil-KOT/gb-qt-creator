#ifndef FILESYSTEMVIEWER_H
#define FILESYSTEMVIEWER_H

#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QStandardItemModel>
#include <QApplication>
#include <QListView>

class FileSystemViewer : public QWidget
{
    Q_OBJECT
public:
    explicit FileSystemViewer(QWidget *parent = nullptr);
    inline QStandardItemModel* getCurrentModel() const
    {
        return model_;
    }
    void setNewModel(QStandardItemModel* newModel);
    void rebuildModel(QString path);

private slots:
    void chngDisk(int index);
    void goRootPath();
    void changeDirectory(const QModelIndex &index);
    void changeDirectoryAddrLine();

private:
    QGridLayout* layout_;
    QListView* list_;
    QPushButton* rootDirBtn_;
    QLineEdit* addrLine_;
    QComboBox* driveSelector_;
    QString currentPath_;
    QString prevPath_;
    QStandardItemModel* model_;
};

#endif // FILESYSTEMVIEWER_H
