#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <QWidget>

namespace Ui {
class DataFrame;
}

class DataFrame : public QWidget
{
    Q_OBJECT

public:
    explicit DataFrame(QWidget *parent = nullptr);
    ~DataFrame();

private:
    Ui::DataFrame *ui;
};

#endif // DATAFRAME_H
