#include "selectdir.h"
#include "extractor.h"
#include "ui_selectdir.h"
#include <QMessageBox>
selectDir::selectDir(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selectDir)
{
    ui->setupUi(this);

    QPixmap packIcon(":/icon/packageIcon");
    ui->Icon->setPixmap(packIcon);
    this->setWindowFlags(Qt::Window);

    ui->currentPath->toggle();
}

selectDir::~selectDir()
{
    delete ui;
}

void selectDir::on_dirButton_clicked()
{
    QDir selectedDir = getProjectDir();
    if(selectedDir.path() == ".")return;
    ui->pathEdit->setText(selectedDir.absolutePath());
}

void selectDir::on_usrPath_toggled(bool checked)
{
    ui->pathEdit->setEnabled(checked);
    ui->dirButton->setEnabled(checked);
}

QDir selectDir::getSelection(){
    if(ui->usrPath->isChecked()){
        return QDir(ui->pathEdit->text());
    }else{
        return QDir(".");
    }
}

void selectDir::on_acceptButton_clicked()
{
    if(ui->usrPath->isChecked()){
        QDir examineDir(ui->pathEdit->text());
        if(examineDir.exists()){
            emit accept();
            return;
        }else{
            QMessageBox::warning(this,tr("错误"),tr("文件夹不存在!"));
            return;
        }
    }else{
        emit accept();
    }
}
