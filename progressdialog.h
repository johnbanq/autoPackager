#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include <QTimer>
namespace Ui {
class progressDialog;
}

class progressDialog : public QDialog
{
    Q_OBJECT

public:
    void appendLog(QString inLog);
    void setTotalProgress(int percent);
    explicit progressDialog(QWidget *parent = 0);
    ~progressDialog();

private slots :
    void onTimerUpdate();

private:
    Ui::progressDialog *ui;
    QTimer* refreshTimer;
    int totalProgress;
    QList<QString> logAppendQueue;
};

#endif // PROGRESSDIALOG_H
