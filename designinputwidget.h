#ifndef DESIGNINPUTWIDGET_H
#define DESIGNINPUTWIDGET_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class DesignInputWidget;
}

class DesignInputWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DesignInputWidget(QWidget *parent = nullptr);
    ~DesignInputWidget();

private:
    Ui::DesignInputWidget *ui;
};

#endif // DESIGNINPUTWIDGET_H
