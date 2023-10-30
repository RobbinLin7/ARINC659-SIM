#ifndef STYLESHEETDIALOG_H
#define STYLESHEETDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QResource>
#include <QFile>
#include <QDir>
#include <QDebug>

namespace Ui {
class StyleSheetDialog;
}

class StyleSheetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StyleSheetDialog(QWidget *parent = nullptr);
    ~StyleSheetDialog();

private:
    Ui::StyleSheetDialog *ui;
signals:
    void changeStyleSheetSignal(QString styleSheet);
};

#endif // STYLESHEETDIALOG_H
