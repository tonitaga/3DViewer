#include "eastereggs.h"
#include "ui_eastereggs.h"
#include <QPixmap>



EasterEggs::EasterEggs(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EasterEggs)
{
    ui->setupUi(this);
    this->setWindowTitle("3DViewer");
    ui->label_bratva->setPixmap(QPixmap(":/2.png"));
}



EasterEggs::~EasterEggs()
{
    delete ui;
}



void EasterEggs::on_pushButton_return_clicked()
{
    emit window_egg();
    this->close();
}
