#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include <QDialog>

namespace Ui {
class NewProjectDialog;
}

class NewProjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewProjectDialog(QWidget *parent = nullptr);
    ~NewProjectDialog();

private slots:
    void on_okBtn_clicked();

signals:
    void sendProjInfo(QString name, QString info);

private:
    Ui::NewProjectDialog *ui;
};

#endif // NEWPROJECTDIALOG_H
