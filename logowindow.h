#ifndef LOGOWINDOW_H
#define LOGOWINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class LogoWindow;
}

class LogoWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LogoWindow(QWidget *parent = 0);
    ~LogoWindow();

private:
    Ui::LogoWindow *ui;

private slots:
    void hideLogo();
};

#endif // LOGOWINDOW_H
