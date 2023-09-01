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
    explicit ModuleCfgWidget(QWidget *parent = nullptr);
    ~ModuleCfgWidget();

private:
    Ui::ModuleCfgWidget *ui;
    QValidator* moduleNumberValidator = nullptr;
    QValidator* initialWaitTimeValidator = nullptr;
    int dummy = 0;
    void installValidator();
    uint moduleNumber;      //妯″潡鍙
private slots:
    void checkLineEditText();
};

#endif // MODULECFGWIDGET_H
