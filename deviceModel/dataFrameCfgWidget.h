#ifndef DATAFRAMECFGWIDGET_H
#define DATAFRAMECFGWIDGET_H

#include <QWidget>
#include <QValidator>
#include "bodyFrameGraphicsItem.h"
#include "validator/stringidvalidator.h"

namespace Ui {
class DataFrameCfgWidget;
}

class DataFrameCfgWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DataFrameCfgWidget(const std::map<std::string, DataFrame>& dataframes, QWidget *parent = nullptr);
    explicit DataFrameCfgWidget(const DataFrame& dataFrame, const std::map<std::string, DataFrame>& dataframes, QWidget *parent = nullptr);
    ~DataFrameCfgWidget();

private:
    Ui::DataFrameCfgWidget *ui;
    QValidator* framePeriodValidator = nullptr;
    //QValidator* frameIdentificationValidator = nullptr;
    std::map<std::string, DataFrame> dataframes;
    StringIdValidator frameIdentificationValidator;
    DataFrame dataFrame;
    int dummy = 0;
    void installValidator();
    bool check(QWidget*);
    void setForm();

private slots:
    void checkLineEditText();
    void on_okPushButton_clicked(bool);
    void on_cancelPushButton_clicked(bool);

signals:
    void saveDataFrameSignal(const DataFrame&);
};

#endif // DATAFRAMECFGWIDGET_H
