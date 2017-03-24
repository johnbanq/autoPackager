#include "fileproject.h"
#include "extractor.h"
#include <QDateTime>
#include <QDebug>
//hidden static functions
void fileProject::recurItemSel(QTreeWidgetItem* root,Qt::CheckState targetState){
    if(root->childCount()){//base case,just delete itself
        for(int i=0;i<root->childCount();i++){
            recurItemSel(root->child(i),targetState);
        }
    }
    root->setCheckState(0,targetState);
}


void fileProject::recurItemDel(QTreeWidgetItem* root){
    if(root->childCount()){//if base case,just delete itself
        if(root->child(0)==NULL)return;
        for(int i=0;i<root->childCount();i++){
            recurItemDel(root->child(i));
        }
    }
    delete root;
}

//public static functions

void fileProject::updateCheckState(QTreeWidgetItem* item){
    Qt::CheckState targetState = item->checkState(0);
    if(targetState == Qt::Unchecked){//invert the status
        targetState = Qt::Checked;
    }else{
        targetState = Qt::Unchecked;
    }
    recurItemSel(item,targetState);

    //update the parent Nodes
    {
        QTreeWidgetItem* parent = item;
        if(parent->parent())parent = parent->parent();
        while(parent){
            //check the status of its children
            bool isAllChecked=true;
            bool isChecked = false;
            for(int i=0;i<parent->childCount();i++){
                switch(parent->child(i)->checkState(0)){
                    case Qt::Checked:
                        isChecked = true;
                    break;
                    case Qt::PartiallyChecked:
                        isAllChecked = false;
                        isChecked = true;
                    break;
                    case Qt::Unchecked:
                        isAllChecked = false;
                    break;
                 }
             }
            //if partially checked
            if(isChecked&&!isAllChecked){
                parent->setCheckState(0,Qt::PartiallyChecked);
            //if all checked
            }else if(isChecked&&isAllChecked){
                parent->setCheckState(0,Qt::Checked);
            }else{
            //if none checked
                parent->setCheckState(0,Qt::Unchecked);
            }
            parent = parent->parent();
        }
    }
}

//constructors & destructors

fileProject::fileProject(const fileProject& inProject):
    createdTime(inProject.createdTime),
    directory(inProject.directory),
    fileType(inProject.fileType),
    entryList(inProject.entryList),
    root(inProject.root),
    refCount(inProject.refCount)
{//DEAL WITH REF COUNT
    *refCount=(*refCount)+1;
}

fileProject::fileProject(QDir inDir,QList<QString> inType,QLabel* inLabel):
    fileProject(inDir.dirName(),inDir,inType,inLabel)
{}

fileProject::fileProject(QString inName,QDir inDir,QList<QString> inType,QLabel* inLabel):
    directory(inDir),fileType(inType)
{
    QRegExp suffixExp = generateSuffixRegExp(inType);
    QFileInfoList fileInfoList;
    createdTime = QFileInfo(inDir.absolutePath()).created();
    recurFind(fileInfoList,suffixExp,inDir,false,inLabel);

    //deal with the result
    for(int i=0;i<fileInfoList.size();i++){
        //track the date time
        QDateTime currentDate = QFileInfo(fileInfoList.at(i).absoluteFilePath()).created();
        if(createdTime<currentDate)createdTime = currentDate;//keeping the last modified date

        //insert the nodes
        fileEntry* currentEntry = new fileEntry(NULL,fileInfoList.at(i));
        currentEntry->setCheckState(0,Qt::Unchecked);
        currentEntry->setText(0,fileInfoList.at(i).fileName());
        currentEntry->setIcon(0,QIcon(":/icon/fileIcon"));
        entryList.append(currentEntry);

    }

    //construct the tree structure

    //assign the root
    root = new QTreeWidgetItem();
    root->setText(0,inName);
    root->setText(1,createdTime.toString());
    root->setCheckState(0,Qt::Unchecked);
    root->setIcon(0,QIcon(":/icon/projectIcon"));

    //form the tree structure
    for(int i=0;i<fileType.size();i++){
       //assign the type item
       QTreeWidgetItem* currentType = new QTreeWidgetItem(root);
       currentType->setText(0,"."+fileType.at(i));
       currentType->setCheckState(0,Qt::Unchecked);
       currentType->setIcon(0,QIcon(":/icon/typeIcon"));

       //assign parents to files
       for(int j=0;j<entryList.size();j++){
           fileEntry* currentEntry = entryList.at(j);
           if(currentEntry->getInfo().completeSuffix() == fileType.at(i)){
               currentType->addChild(currentEntry);
           }
       }
    }

    //ref counter
    refCount = new int;
    *refCount = 1 ;
}

fileProject::~fileProject(){
    //return;
    if(*refCount == 1){
    //just in case,delete all the 'wild' nodes
    /*for(int i=0;i<entryList.size();i++){

        if(entryList.at(i)->parent() == NULL){
            delete entryList.at(i);
            entryList.replace(i,NULL);
        }
    }*/
    //clear all the nodes
    recurItemDel(root);
    }else{
        *refCount=(*refCount)-1;
    }
}

//setters and getters

void fileProject::setName(QString inName){
    root->setText(0,inName);
}



QFileInfoList fileProject::getFileList() const {
    QFileInfoList infoList;
    for(int i=0;i<entryList.size();i++){
        infoList.append(entryList.at(i)->getInfo());
    }
    return infoList;
}

QFileInfoList fileProject::getSelectedFileList() const {
    QFileInfoList infoList;
    for(int i=0;i<entryList.size();i++){
        if(entryList.at(i)->checkState(0)==Qt::Checked)
            infoList.append(entryList.at(i)->getInfo());
    }
    return infoList;
}


QString fileProject::addFile(QFileInfo inInfo){
    //preCheck:isExist
    for(int i=0;i<entryList.size();i++){
        if(entryList.at(i)->getInfo() == inInfo)
            return QObject::tr("文件已经存在");
    }
    //find the type to insert
    QTreeWidgetItem* typeItem =NULL;
    if(!(fileType.contains(inInfo.completeSuffix()))){
        //FIX:if the type doesnt exist
        fileType.append(inInfo.completeSuffix());
        QTreeWidgetItem* currentType = new QTreeWidgetItem(root);
        currentType->setText(0,"."+inInfo.completeSuffix());
        currentType->setCheckState(0,Qt::Checked);
        currentType->setIcon(0,QIcon(":/icon/typeIcon"));
        typeItem = currentType;
    }else{
        //find the type node
        for(int i=0;i<root->childCount();i++){
            if(root->child(i)->text(0) == "."+inInfo.completeSuffix()){
                typeItem=root->child(i);
            }
        }
    }

    //construct the entry and insert it
    fileEntry* currentEntry = new fileEntry(typeItem,inInfo);
    currentEntry->setCheckState(0,Qt::Checked);
    currentEntry->setText(0,inInfo.fileName());
    currentEntry->setIcon(0,QIcon(":/icon/fileIcon"));
    entryList.append(currentEntry);

    //update the entry status
    updateCheckState(currentEntry);
    return "";
}

bool fileProject::removeFile(QString inName){
    //locate the entry
    fileEntry* targetEntry = NULL;
    for(int i=0;i<entryList.size();i++){
        if(entryList.at(i)->getInfo().fileName() == inName)
            targetEntry = entryList.at(i);
    }
    //if not exist
    if(!targetEntry){
        return false;
    }

    //if its the only file
    if(targetEntry->parent()->childCount() == 1){
        //remove the type
        fileType.removeAt(fileType.indexOf(targetEntry->getInfo().completeSuffix()));
        root->removeChild(targetEntry->parent());
        delete targetEntry->parent();
    }
    targetEntry->setCheckState(0,Qt::Unchecked);
    updateCheckState(targetEntry);
    delete targetEntry;
    return true;
}

//operator
fileProject& fileProject::operator=(const fileProject& inProject){
    *(inProject.refCount) = *(inProject.refCount) + 1 ;
    directory = inProject.directory;
    createdTime = inProject.createdTime;
    fileType = inProject.fileType;
    root = inProject.root;
    entryList = inProject.entryList;
    refCount = inProject.refCount;
    return *this;
}

