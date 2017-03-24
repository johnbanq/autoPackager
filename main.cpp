#include "maindialog.h"
#include "selectdir.h"
#include "progressdialog.h"
#include "extractor.h"
#include "fileproject.h"
#include "settings.h"
#include "registries.h"
#include "settings.h"
#include "JlCompress.h"

#include <QStyleFactory>
#include <QApplication>
#include <QDirIterator>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include <QList>
#include <QListIterator>
#include <QMessageBox>

//global Settings
QStringList codeSuffix;
QStringList projectSuffix;

QDir tempDir("./temp");
QSettings appSettings("./config.ini",QSettings::IniFormat);

int main(int argc, char *argv[])
{
    //NASTY FIX:add default suffixes
    {
        const int suffixListLen = 3;
        QString suffixList[suffixListLen] = {"cpp","h","exe"};

        for(int i=0;i<suffixListLen;i++){
            codeSuffix.append(suffixList[i]);
        }
        const int projectListLen = 3;
        QString projectList[projectListLen] = {"cbp,sln"};
        for(int i=0;i<projectListLen;i++){
            projectSuffix.append(projectList[i]);
        }
    }

    //setup config settings
    appSettings.setIniCodec("UTF8");

    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QApplication a(argc, argv);
    QList<fileProject> foundProjects;

    mainDialog w(foundProjects,NULL);
    selectDir dirSelection;


    do{
    //Step 1 Open selection window,Locate the project dirs
    dirSelection.show();
    if(dirSelection.exec()==QDialog::Rejected){
        return 0;
    }
    QDir selectedDir = dirSelection.getSelection();

    //Step 2 start searching the files

    //locate the project folders
    QFileInfoList prjDirList;
    QRegExp prjExpr(generateSuffixRegExp(projectSuffix));

    recurFind(prjDirList,prjExpr,selectedDir,false,NULL);


    //scan and add to the list
    for(int i=0;i<prjDirList.size();i++){
        fileProject currentProject(prjDirList.at(i).absolutePath(),codeSuffix,NULL);

        //if created this week,then select it
        if(currentProject.getCreatedTime().daysTo(QDateTime::currentDateTime())<7){
            fileProject::recurItemSel(currentProject.getItemRoot(),Qt::Checked);
        }
        foundProjects.append(currentProject);
    }

    //show up for further info
    for(int i=0;i<foundProjects.size();i++){
        w.projectShow(foundProjects.at(i));
    }
    w.show();
    }while(w.exec() == QDialog::Rejected);

    //Step 3 collect info and start compression
    progressDialog progress;
    bool isAllSuccess = true;
    progress.setTotalProgress(0);
    progress.show();
    userInfo usrInfo = w.getUsrInfo();
    for(int i=0;i<foundProjects.size();i++){
        //if its empty,then simply skip it
        if(foundProjects.at(i).getSelectedFileList().size() == 0)continue;
        //get the result and copy
        QFileInfoList failedFiles = extractProject(foundProjects.at(i),tempDir);
        progress.setTotalProgress(((double)i/foundProjects.size())*100);
        QString log;
        //if has failed files
        if(!failedFiles.size()){
            log.append("项目"+foundProjects.at(i).getName()+"打包成功");
        }else{
            isAllSuccess = false;
            log.append("\n项目"+foundProjects.at(i).getName()+"打包失败:\n");
            for(int j=0;j<failedFiles.size();j++){
                log.append("    文件"+failedFiles.at(j).fileName()+"复制失败\n");
            }
        }
        progress.appendLog(log);
    }

    //fill the progressBar,just in case
    progress.setTotalProgress(100);

    //Step 3 compress them
    QString fileName(usrInfo.studNumber+"-"+usrInfo.name+".zip");
    JlCompress::compressDir("./"+fileName,tempDir.absolutePath(),true);
    //Step 4 clean up the temp dir
    recurDel(tempDir);
    //Step 5 show up end info
    QString mailTitle("第"+QString::number(getCurrentWeek(appSettings))+"周实验课作业:"+usrInfo.name+"的作业");
    if(isAllSuccess){
        progress.appendLog(QObject::tr("项目打包完成!\n邮件标题:\n")+mailTitle);
        QMessageBox::information(&progress,
                                 QObject::tr("打包完成"),
                                 QObject::tr("项目全部打包完成!\n保存位置:本工具所在文件夹\n邮件标题: ")+mailTitle);
    }else{
        QMessageBox::critical(&progress,QObject::tr("打包错误"),QObject::tr("有项目未能成功打包!"));
        progress.appendLog(QObject::tr("错误:有项目未能成功打包!请检查"));
        progress.exec();
    }
    //progress.exec();

    //STEP 6 TODO:FIX THE UGLY BUGFIX
    int foundPrjSize = foundProjects.size();
    for(int i=0;i<foundPrjSize;i++){
        foundProjects.pop_front();
    }
    return a.exec();
}
