#ifndef FILEPROJECT_H
#define FILEPROJECT_H

#include <QTreeWidgetItem>
#include <QDir>
#include <QDateTime>
#include <QLabel>
class fileProject{
public:
    class fileEntry;
    //spec-static functions
    static void updateCheckState(QTreeWidgetItem* inItem);
    static void recurItemSel(QTreeWidgetItem* root,Qt::CheckState targetState);
    static void recurItemDel(QTreeWidgetItem* root);
    //constructors
    fileProject(){}
    fileProject(const fileProject &inProject);
    fileProject(QDir inDir,QList<QString> inType,QLabel* inLabel =NULL);
    fileProject(QString inName,QDir inDir,QList<QString> inType,QLabel* inLabel =NULL);
    ~fileProject();
    //setters and getters
    QString getName() const {return root->text(0);}
    void setName(QString inName);
    QDateTime getCreatedTime() const {return createdTime;}
    QDir getDirectory() const {return directory;}
    QTreeWidgetItem* getItemRoot() const {return root;}

    QList<QFileInfo> getFileList() const ;
    QList<QFileInfo> getSelectedFileList() const ;
    QString addFile(QFileInfo inInfo);
    bool removeFile(QString inName);
    //operators
    fileProject& operator=(const fileProject& inProject);
private:
    //TODO: isolate refCount since its not fileProject's job
    QDateTime createdTime;
    QDir directory;
    QList<QString> fileType;
    QList<fileEntry*> entryList;
    QTreeWidgetItem* root;
    int* refCount;
};

class fileProject::fileEntry: public QTreeWidgetItem{
private:
    const QFileInfo entry;
public:
    //constructors
    fileEntry(){}
    //fileEntry(fileEntry inEntry):QTreeWidgetItem(inEntry),entry(inEntry){}
    fileEntry(QTreeWidgetItem* parent,QFileInfo inInfo):QTreeWidgetItem(parent),entry(inInfo){}

    //methods
    QFileInfo getInfo(){return entry;}
};

#endif // FILEPROJECT_H
