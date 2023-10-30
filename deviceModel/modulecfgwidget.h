#ifndef MODULECFGWIDGET_H
#define MODULECFGWIDGET_H

#include <QWidget>
#include <QValidator>
#include "bodyFrameGraphicsItem.h"
#include "dataFrameCfgWidget.h"

namespace Ui {
class ModuleCfgWidget;
}

class ModuleCfgWidget : public QWidget
{
    Q_OBJECT

public:
    enum Type{add, modify};
    explicit ModuleCfgWidget(const Module& module, QWidget *parent = nullptr);
    explicit ModuleCfgWidget(uint moduleNumber, QWidget* parent = nullptr);
    ~ModuleCfgWidget();

private:
    Ui::ModuleCfgWidget *ui;
    QValidator* moduleNumberValidator = nullptr;
    QValidator* initialWaitTimeValidator = nullptr;
    int dummy = 0;
    void installValidator();
    bool check(QWidget*);
    Module module;
    Type type = add;
    void setForm();
private slots:
    void checkLineEditTextSlot();
    void on_okPushButton_clicked(bool);
    void on_cancelPushButton_clicked(bool);
signals:
    void saveModuleSignal(const Module&);
};

#endif // MODULECFGWIDGET_H
