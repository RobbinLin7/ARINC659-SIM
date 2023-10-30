#include "commandfilewidget.h"
#include "ui_commandfilewidget.h"
#include <QFile>
#include <QMessageBox>

CommandFileWidget::CommandFileWidget(const QString fileName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommandFileWidget),
    fileName(fileName)
{
    ui->setupUi(this);
    loadFile();
    connect(&watcher, &QFileSystemWatcher::fileChanged, this, &CommandFileWidget::fileChanged);
}

CommandFileWidget::~CommandFileWidget()
{
    delete ui;
}

void CommandFileWidget::loadFile()
{
    QFile commandFile(fileName);
    if(commandFile.open(QIODevice::ReadOnly) == true){
        watcher.addPath(fileName);
        QString commands = commandFile.readAll();
        ui->textBrowser->setText(commands);
        commandFile.close();
    }
    else{

    }
}

void CommandFileWidget::on_closePushButton_clicked()
{
    this->close();
}

void CommandFileWidget::fileChanged()
{
//    QMessageBox box(QMessageBox::Warning, "命令表文件已修改", "命令表文件已修改，要重新加载吗");
//    box.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
//    box.setDefaultButton(QMessageBox::Cancel);
//    box.setButtonText(QMessageBox::Yes, QString("确定"));
//    box.setButtonText(QMessageBox::Cancel, QString("取消"));
//    //QMessageBox box("文件已修改", "文件已修改，要重新加载吗", )
//    //QMessageBox box(QMessageBox::warning, "File Change", "The file has changed. Do you want to reload it?");
//    int answer = box.exec();
//    if (answer == QMessageBox::Yes) {
//        loadFile();
//    }
    loadFile();
}
