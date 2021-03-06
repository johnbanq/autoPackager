#include "maindialog.h"
#include "ui_maindialog.h"
#include "fileproject.h"
#include "extractor.h"
#include "settings.h"
#include "registries.h"

#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QMenu>
#include<QInputDialog>

void mainDialog::projectShow(const fileProject &inProject){
    ui->projectTree->addTopLevelItem(inProject.getItemRoot());
}

void mainDialog::projectRemove(const fileProject &inProject){
    ui->projectTree->invisibleRootItem()->removeChild(inProject.getItemRoot());
}

userInfo mainDialog::getUsrInfo(){
    userInfo result = {ui->nameEdit->text(),ui->numEdit->text()};
    return result;
}

mainDialog::mainDialog(QList<fileProject> &inProject,QWidget *parent):
    QDialog(parent),
    ui(new Ui::mainDialog),
    listedProjects(inProject)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::Window);
    //ui->projectTree->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //setup tool tips
    ui->addButton->setToolTip(tr("添加新项目"));
    ui->selButton->setToolTip(tr("全选"));
    ui->deSelButton->setToolTip(tr("清空选择"));

    //setup weekbox
    ui->weekBox->setValue(getCurrentWeek(appSettings));
    userInfo currentUserInfo = getUserInfo(appSettings);

    //setup user name
    ui->nameEdit->setText(currentUserInfo.name);
    ui->numEdit->setText(currentUserInfo.studNumber);

}

mainDialog::~mainDialog()
{
    delete ui;
}



void mainDialog::on_projectTree_itemClicked(QTreeWidgetItem *item, int column)
{
    //get away with the warnings
    column =0;
    //routed to checkState
   fileProject::updateCheckState(item);
}

void mainDialog::on_packButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr("输入错误"));
    //if name is empty
    if(ui->nameEdit->text().isEmpty()){
        msgBox.setText(tr("姓名不能为空!"));
        msgBox.exec();
        return;
    }
    //if studnumber is empty
    if(ui->numEdit->text().isEmpty()){
        msgBox.setText(tr("学号不能为空!"));
        msgBox.exec();
        return;
    }
    //if studnumber is invalid
    QRegExp isNumOnly("^[0-9]*$");
    if(!(isNumOnly.exactMatch(ui->numEdit->text()))){
        msgBox.setText("学号不能含有数字以外的字符!");
        msgBox.exec();
        return;
    }

    //if no problem,then accept,start packaging
    setWeek(appSettings,ui->weekBox->value());
    userInfo tempInfo = {ui->nameEdit->text(),ui->numEdit->text()};
    setUserInfo(appSettings,tempInfo);
    emit accept();
}

void mainDialog::on_addButton_clicked()
{
    QDir projectDir = getProjectDir();
    //check for validity
    if(projectDir.path()==".")return;
    listedProjects.append(fileProject(projectDir,codeSuffix));
    projectShow(listedProjects.last());
}

void mainDialog::on_selButton_clicked()
{
    for(int i=0;i<listedProjects.size();i++){
        fileProject::recurItemSel(listedProjects.at(i).getItemRoot(),Qt::Checked);
    }
}

void mainDialog::on_deSelButton_clicked()
{
    for(int i=0;i<listedProjects.size();i++){
        fileProject::recurItemSel(listedProjects.at(i).getItemRoot(),Qt::Unchecked);
    }
}

void mainDialog::on_projectTree_customContextMenuRequested(const QPoint &pos)
{
    menuSelectedItem  = ui->projectTree->itemAt(pos);
    //only to projects
    if(menuSelectedItem->parent()!=NULL)return;
    QMenu projectMenu;
    projectMenu.addAction(ui->renameProject);
    projectMenu.exec(QCursor::pos());
}

void mainDialog::on_renameProject_triggered()
{
    if(!menuSelectedItem)return;
    QString newName = QInputDialog::getText(this,
                                            tr("项目名"),
                                            tr("请输入新的项目名:"),
                                            QLineEdit::Normal,
                                            "name");
    //ERROR CHECK:if empty
    if(!newName.size()){
        QMessageBox::information(this,tr("提示"),tr("项目名不能为空"));
        return;
    }
    menuSelectedItem->setText(0,newName);
    return;
}
