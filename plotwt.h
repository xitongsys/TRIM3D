#ifndef PLOTWT_H
#define PLOTWT_H

#include <QWidget>

namespace Ui {
class PlotWT;
}

class PlotWT : public QWidget
{
    Q_OBJECT

public:
    explicit PlotWT(QWidget *parent = 0);
    ~PlotWT();    
    void paintEvent(QPaintEvent *event);

private:
    Ui::PlotWT *ui;
};

#endif // PLOTWT_H
