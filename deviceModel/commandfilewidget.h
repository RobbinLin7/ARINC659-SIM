#ifndef COMMANDFILEWIDGET_H
#define COMMANDFILEWIDGET_H

#include <QWidget>
#include <QFileSystemWatcher>

namespace Ui {
class CommandFileWidget;
}

class CommandFileWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CommandFileWidget(const QString fileName, QWidget *parent = nullptr);
    ~CommandFileWidget();

private:
    Ui::CommandFileWidget *ui;
    const QString fileName;
    QFileSystemWatcher watcher;
    void loadFile();

private slots:
    void on_closePushButton_clicked();
    void fileChanged();
};

#endif // COMMANDFILEWIDGET_H
