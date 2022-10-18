#ifndef EASTEREGGS_H
#define EASTEREGGS_H

#include <QWidget>

namespace Ui {
class EasterEggs;
}

class EasterEggs : public QWidget
{
    Q_OBJECT

public:
    explicit EasterEggs(QWidget *parent = 0);
    ~EasterEggs();

signals:
    void window_egg();

private slots:
    void on_pushButton_return_clicked();

private:
    Ui::EasterEggs *ui;
};

#endif // EASTEREGGS_H
