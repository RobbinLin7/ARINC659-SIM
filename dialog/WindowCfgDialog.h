#ifndef WINDOWCFGDIALOG_H
#define WINDOWCFGDIALOG_H
#include <QDialog>
#include <QDebug>
#include "data/framewindow.h"

class WindowCfgDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WindowCfgDialog(QWidget *parent = nullptr):QDialog(parent){}
    ~WindowCfgDialog(){}

protected:
    FrameWindow window;
    void addNewWindowfunc(){
        qDebug() << "windowcfgdialog";
        emit addNewWindow(window);
        this->close();
    }
protected slots:
    void on_cancelPushButton_clicked(){
        this->close();
    }
signals:
    void addNewWindow(const FrameWindow& window);
};
#endif // WINDOWCFGDIALOG_H
