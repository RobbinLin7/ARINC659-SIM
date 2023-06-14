#ifndef FAULTWIDGET_H
#define FAULTWIDGET_H

#include <QWidget>

namespace Ui {
class FaultWidget;
}

class FaultWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FaultWidget(QWidget *parent = nullptr);
    ~FaultWidget();

private:
    Ui::FaultWidget *ui;
};

#endif // FAULTWIDGET_H
