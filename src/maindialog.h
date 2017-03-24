#ifndef MAINDIALOG_H
#define MAINDIALOG_H
#include "fileproject.h"
#include "settings.h"
#include <QDialog>
#include<QList>
#include <QTreeWidgetItem>
namespace Ui {
class mainDialog;
}

class mainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit mainDialog(QList<fileProject> &inProject,QWidget *parent = 0);
    void projectShow(const fileProject& inProject);
    void projectRemove(const fileProject& inProject);
    userInfo getUsrInfo();
    ~mainDialog();


private slots:

    void on_projectTree_itemClicked(QTreeWidgetItem *item, int column);

    void on_packButton_clicked();

    void on_addButton_clicked();

    void on_selButton_clicked();

    void on_deSelButton_clicked();

    void on_projectTree_customContextMenuRequested(const QPoint &pos);

    void on_renameProject_triggered();

private:
    Ui::mainDialog *ui;
    QList<fileProject> &listedProjects;
    //nasty variable to get the menu working
    QTreeWidgetItem* menuSelectedItem;
};

#endif // MAINDIALOG_H
