#include "extractor.h"
#include<QObject>
#include<QFileInfoList>
#include<QListIterator>
#include<QRegExp>
#include <QTimer>

//generate reg,suffix does not contains the dot
QRegExp generateSuffixRegExp(QStringList suffixes){

    QString targetExp;
    QStringListIterator iter(suffixes);

    while(iter.hasNext()){
        targetExp += iter.next();
        //targetExp += "$"; // make sure its a suffix
        if(iter.hasNext()){//prevents the extra | at the end of pattern
            targetExp += "|";
        }
    }
    return QRegExp(targetExp);
}

QDir getProjectDir(){
    QFileDialog prjSelect(NULL,QObject::tr("请选择项目文件夹"));
    QStringList selectedDir;
    //configure the dialog
    prjSelect.setFileMode(QFileDialog::Directory);
    prjSelect.setOption(QFileDialog::ShowDirsOnly,true);
    if(prjSelect.exec()){
        selectedDir = prjSelect.selectedFiles();
    }
    if(selectedDir.empty())return QDir(".");
    return QDir(*(selectedDir.begin()));
}

QFileInfoList& recurFind(QFileInfoList& result,const QRegExp& pattern,QDir currentDir
                           ,bool RETOnFound,QLabel* indicator){
    QFileInfoList dirEntires = currentDir.entryInfoList(QDir::AllEntries|QDir::NoDotAndDotDot);
    QListIterator<QFileInfo> fileIter(dirEntires);
    while(fileIter.hasNext()){
        QFileInfo currentEntry = fileIter.next();
        if(currentEntry.isDir()){//if its a directory
            recurFind(result,pattern,QDir(currentEntry.absoluteFilePath()),RETOnFound,indicator);
        }
        if(indicator!=NULL)indicator->setText(currentEntry.absoluteFilePath());
        if(pattern.exactMatch(currentEntry.completeSuffix())){//if its a file and suffix matches
            result.append(currentEntry);
            if(RETOnFound)return result;//return on found
        }

    }
    return result;
}
//for fallback methods
#if QT_VERSION >= 0x050800
void recurDel(QDir inDir){
    inDir.removeRecursively();
}
#else
void recurDel(QDir inDir){
    QDirIterator it(inDir, QDirIterator::Subdirectories);
    while(it.next()!=QString::null){
        QFileInfo currentFileInfo = it.fileInfo();
        if(currentFileInfo.isDir()){
            QDir currentDir(currentFileInfo.absoluteFilePath());
            recurDel(currentDir);
            inDir.rmdir(currentDir.dirName());
        }else{
            QFile(currentFileInfo.absoluteFilePath()).remove();
        }
    }
}
#endif

QFileInfoList extractProject(const fileProject& inProject, QDir tempDir){
    //setup the temp cache for storing project files
    QFileInfoList failedFiles;
    if(!tempDir.exists()){
        //if not exist and failed to make the dir
        if(!QDir(".").mkdir("temp")){
            //push the dir into the list to indicate the whole failure
            failedFiles.append(QFileInfo(inProject.getDirectory().absolutePath()));
            return failedFiles;
        }
    }

    //preparing project dir
    QDir newProjectDir(tempDir.absolutePath()+"/"+inProject.getName());
    //if exist,clean the project dir
    if(newProjectDir.exists()){
        recurDel(newProjectDir);
    }
    //if empty,return
    if(inProject.getSelectedFileList().size()==0)return failedFiles;
    if(!tempDir.mkdir(inProject.getName())){
        //push the dir into the list to indicate the whole failure
        failedFiles.append(QFileInfo(inProject.getDirectory().absolutePath()));
        return failedFiles;
    }
    //iter and copy all the target files
    QString storagePath = newProjectDir.absolutePath();
    QFileInfoList fileList = inProject.getSelectedFileList();
    for(int i=0;i<fileList.size();i++){
        QFile currentFile(fileList.at(i).absoluteFilePath());
        bool isCopied = currentFile.copy(storagePath+"/"+fileList.at(i).fileName());
        //if exists
        if(!isCopied&&!currentFile.exists()){
            failedFiles.append(fileList.at(i).absoluteFilePath());
        }
    }
    return failedFiles;
}
