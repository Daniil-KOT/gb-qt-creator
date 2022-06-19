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
#include <QTranslator>

#include <findcontroller.h>

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

    void findFile();
    void popUpInfo(QString info);
    void targetFound(QString path, bool isDir);

    void switchLang();

private:
    void translate(QString lang);

private:
    QGridLayout* layout_;
    QListView* list_;
    QPushButton* rootDirBtn_;
    QLineEdit* addrLine_;    
    QLineEdit* search_;
    QComboBox* driveSelector_;
    QString currentPath_;
    QString prevPath_;
    QStandardItemModel* model_;
    FindController* controller_;
    QMap<QModelIndex, QString> foundDirs_;
    QTranslator translator_;
    QPushButton* translateBtn_;
};

#endif // FILESYSTEMVIEWER_H
