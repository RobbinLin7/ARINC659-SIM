#ifndef SIMULINKWIDGET_H
#define SIMULINKWIDGET_H

#include <QWidget>

namespace Ui {
class SimulinkWidget;
}

class SimulinkWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SimulinkWidget(QWidget *parent = nullptr);
    ~SimulinkWidget();

private slots:
    void on_faultInjectionBtn_clicked();

private:
    Ui::SimulinkWidget *ui;
};

#endif // SIMULINKWIDGET_H
