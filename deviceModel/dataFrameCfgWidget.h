#ifndef DATAFRAMECFGWIDGET_H
#define DATAFRAMECFGWIDGET_H

#include <QWidget>
#include <QValidator>
#include "bodyFrameGraphicsItem.h"

namespace Ui {
class DataFrameCfgWidget;
}

class DataFrameCfgWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DataFrameCfgWidget(QWidget *parent = nullptr);
    ~DataFrameCfgWidget();

private:
    Ui::DataFrameCfgWidget *ui;
    QValidator* framePeriodValidator = nullptr;
    QValidator* frameIdentificationValidator = nullptr;
    int dummy = 0;
    void installValidator();

private slots:
    void checkLineEditText();
};

#endif // DATAFRAMECFGWIDGET_H
