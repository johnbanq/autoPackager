#include "progressdialog.h"
#include "ui_progressdialog.h"

progressDialog::progressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::progressDialog)
{
    ui->setupUi(this);
    refreshTimer = new QTimer();
    connect(refreshTimer,SIGNAL(timeout()),this,SLOT(onTimerUpdate()));
    refreshTimer->start(100);
}

void progressDialog::appendLog(QString inLog){
    logAppendQueue.append(inLog);
    //set scroll down
}

void progressDialog::setTotalProgress(int percent){
    totalProgress = percent;
}

void progressDialog::onTimerUpdate(){

    ui->totalBar->setValue(totalProgress);

    while(!(logAppendQueue.empty())){
        ui->logBrowser->append(logAppendQueue.front());
        logAppendQueue.pop_front();
    }
    ui->logBrowser->moveCursor(QTextCursor::End);
}


progressDialog::~progressDialog()
{
    refreshTimer->stop();
    delete ui;
    delete refreshTimer;
}
