#ifndef SELECTDIR_H
#define SELECTDIR_H

#include <QDialog>
#include <QDir>
namespace Ui {
class selectDir;
}

class selectDir : public QDialog
{
    Q_OBJECT

public:
    explicit selectDir(QWidget *parent = 0);
    QDir getSelection();
    ~selectDir();

private slots:

    void on_dirButton_clicked();

    void on_usrPath_toggled(bool checked);

    void on_acceptButton_clicked();

private:
    Ui::selectDir *ui;
};

#endif // SELECTDIR_H
