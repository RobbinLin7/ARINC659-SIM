#ifndef HARDWAREMODEL_H
#define HARDWAREMODEL_H

#include <QWidget>

#include "dataFrame.h"

namespace Ui {
class HardwareModel;
}

class HardwareModel : public QWidget
{
    Q_OBJECT

public:
    explicit HardwareModel(QWidget *parent = nullptr);
    ~HardwareModel();

private:
    Ui::HardwareModel *ui;
};

#endif // HARDWAREMODEL_H
