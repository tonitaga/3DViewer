#include "settingsmodel.h"
#include "ui_settingsmodel.h"
#include <QCloseEvent>
#include <QColorDialog>
#include <QMessageBox>
#include "viewmodel.h"



SettingsModel::SettingsModel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsModel)
{
    ui->setupUi(this);
    setWindowTitle("3DViewer");
    settings = new Model3DSettings();
    ui->comboBox_showness->setCurrentIndex(2);
}

SettingsModel::~SettingsModel()
{
    delete ui;
}



void SettingsModel::closeEvent(QCloseEvent *event)
{
    event->ignore();
    if (!settings->all_accept) {
        int ret = QMessageBox::question(this,
                                        "Close window", "Do you want to save settings?",
                                        QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            on_pushButton_acceptAll_clicked();
            event->accept();
        } else {
            emit window_before();
            event->accept();
        }
    } else {
        event->accept();
    }
}

void SettingsModel::on_pushButton_ribColorChoose_clicked()
{
    QColor ribs = QColorDialog::getColor(Qt::green, this);
    settings->rib_color[0] = ribs.redF();
    settings->rib_color[1] = ribs.greenF();
    settings->rib_color[2] = ribs.blueF();
}

void SettingsModel::on_pushButton_vertexColorChoose_clicked()
{
    QColor vertexes = QColorDialog::getColor(Qt::green, this);
    settings->vertex_color[0] = vertexes.redF();
    settings->vertex_color[1] = vertexes.greenF();
    settings->vertex_color[2] = vertexes.blueF();
}



void SettingsModel::on_pushButton_acceptAll_clicked()
{
    settings->all_accept = true;
    float value = ui->lineEdit_vertexSize->text().toFloat();
    if (value >= 0.0) {
        settings->vertex_size = value;
    }

    value = ui->lineEdit_ribWidth->text().toFloat();
    if (value >= 0.0) {
        settings->rib_size = value;
    }

    settings->rib_type = ui->comboBox_ribType->currentIndex();
    settings->vertex_showness = ui->comboBox_showness->currentIndex();

    emit signalSettings(settings);
    emit window_before();
    this->close();
}



void SettingsModel::on_pushButton_bgColorChoose_clicked()
{
    QColor bg = QColorDialog::getColor(Qt::black, this);
    settings->bg_color[0] = bg.redF();
    settings->bg_color[1] = bg.greenF();
    settings->bg_color[2] = bg.blueF();
}
