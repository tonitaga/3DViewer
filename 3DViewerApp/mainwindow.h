#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QMoveEvent>
#include <QSettings>
#include <QVariant>
#include <QTimer>
#include <QDebug>

#include "settingsmodel.h"
#include "viewmodel.h"
#include "eastereggs.h"

#define _USE_MATH_DEFINES



struct TotalMovement {
    TotalMovement() {
        total_angle_ox = total_angle_oy = total_angle_oz = 0.0;
        total_move_ox = total_move_oy = total_move_oz = 0.0;
        total_scale = 1.0;
    }
public:
    float total_angle_ox;
    float total_angle_oy;
    float total_angle_oz;
    float total_scale;
    float total_move_ox;
    float total_move_oy;
    float total_move_oz;
};



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void readSettings();
    void saveSettings();
    void saveDefaultSettings();


public slots:
    void slotSettings(Model3DSettings *s);
    void slotTabBar(int index);

signals:
    void window_before();
    void window_egg();

private slots:
    void on_pushButton_loadmodel_clicked();
    void on_pushButton_moveOY_clicked();
    void on_pushButton_moveOX_clicked();
    void on_pushButton_moveOZ_clicked();
    void on_pushButton_rotateOX_clicked();
    void on_pushButton_rotateOY_clicked();
    void on_pushButton_rotateOZ_clicked();
    void on_pushButton_scale_clicked();
    void on_pushButton_settings_clicked();
    void on_pushButton_toStartPoint_clicked();
    void on_pushButton_saveImage_clicked();
    void on_pushButton_parallelProjection_clicked();
    void on_pushButton_centralProjection_clicked();
    void on_pushButton_easterEgg_clicked();
    void on_pushButton_recordScreen_clicked();

private:
    void initStateWindow();
    int  initStateProjection();
    void initStateMovement();
    void initSettingsState();
    void initDefaultLabelTexts();
    void initLabelTexts(Model3D *model_saved);
    void initZeroMovement();
    void checkProjectionPos();
    void throwDialog();

    Model3D *model_saved;                  // (c)-struct
    Model3D *default_state_model;          // (c)-struct
    Model3D *model_parralel_projection;    // (c)-struct
    Model3D *model_central_projection;     // (c)-struct
    SettingsModel *settings_model;         // (ui)-class
    EasterEggs *easted_egg;                // (ui)-class
    Model3DSettings *model3d_settings;     // (c++)-struct
    TotalMovement *movement;               // (c++)-struct

    GLuint count_taps_to_load_button;
    QString filename;
    QString fileway;
    QSettings *settings;
    QPoint my_old_pos;

    bool model_is_inited        = false;
    bool smth_is_happend        = false;
    bool settings_is_clicked    = false;
    bool in_parallel_projec     = false;
    bool in_central_proj        = false;
    bool reading_status         = false;

    Ui::MainWindow *ui;

protected:
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void moveEvent(QMoveEvent *event) override;
};
#endif // MAINWINDOW_H
