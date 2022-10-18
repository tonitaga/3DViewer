#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qgifimage.h"
#include <QThread>
#include <clocale>
#include <unistd.h>
#include <cmath>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), count_taps_to_load_button(0),
      my_old_pos(pos()), model_is_inited(false),
      smth_is_happend(false), settings_is_clicked(false), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    setWindowTitle("3DViewer");

    model_saved =               new Model3D;
    default_state_model =       new Model3D;
    model_parralel_projection = new Model3D;
    model_central_projection =  new Model3D;
    easted_egg =                new EasterEggs;
    movement =                  new TotalMovement();
    settings_model =            new SettingsModel();
    model3d_settings =          new Model3DSettings();
    settings =                  new QSettings("tonijerl", "settings", this);

    connect(settings_model, &SettingsModel::window_before, this, &MainWindow::show);
    connect(easted_egg, &EasterEggs::window_egg, this, &MainWindow::show);
    connect(settings_model, &SettingsModel::signalSettings, this, &MainWindow::slotSettings);
    connect(ui->tabWidget,  &QTabWidget::tabBarClicked, this, &MainWindow::slotTabBar);

    readSettings();
}



MainWindow::~MainWindow()
{
    delete movement;
    delete ui;
}



/**********************************************************************/
/*                                                                    */
/*   Block of functions working at read and save the state of model   */
/*                                                                    */
/**********************************************************************/



void MainWindow::readSettings()
{
    setGeometry(settings->value("WindowGeometry", QRect(300, 200, 800, 600)).toRect());
    bool is_inited = settings->value("initedStatus", false).toBool();
    in_parallel_projec = settings->value("parallelStatus", false).toBool();
    in_central_proj = settings->value("centralStatus", false).toBool();
    if (is_inited) {
        reading_status = true;
        initStateWindow();
        initStateMovement();
        initSettingsState();
        reading_status = false;
    }
}


void MainWindow::saveSettings()
{
    settings->setValue("WindowGeometry", geometry());
    if (model_is_inited == true) {
        model3d_settings = ui->openGLWidget->getSettings();

        settings->setValue("initedFileway", fileway);
        settings->setValue("initedFilename", filename);

        qDebug() << "ox: " << movement->total_move_ox << " oy: " << movement->total_move_oy << " oz: " << movement->total_move_oz << "\n";
        qDebug() << movement->total_scale << "\n";

        settings->setValue("moveOx", movement->total_move_ox);
        settings->setValue("moveOy", movement->total_move_oy);
        settings->setValue("moveOz", movement->total_move_oz);
        settings->setValue("angleOx", movement->total_angle_ox);
        settings->setValue("angleOy", movement->total_angle_oy);
        settings->setValue("angleOz", movement->total_angle_oz);
        settings->setValue("scale", movement->total_scale);

        settings->setValue("vertexSize", model3d_settings->vertex_size);
        settings->setValue("ribSize", model3d_settings->rib_size);
        settings->setValue("ribType", model3d_settings->rib_type);
        settings->setValue("vertexType", model3d_settings->vertex_showness);

        settings->setValue("redBG", model3d_settings->bg_color[0]);
        settings->setValue("greenBG", model3d_settings->bg_color[1]);
        settings->setValue("blueBG", model3d_settings->bg_color[2]);
        settings->setValue("redRib", model3d_settings->rib_color[0]);
        settings->setValue("greenRib", model3d_settings->rib_color[1]);
        settings->setValue("blueRib", model3d_settings->rib_color[2]);
        settings->setValue("redVertex", model3d_settings->vertex_color[0]);
        settings->setValue("greenVertex", model3d_settings->vertex_color[1]);
        settings->setValue("blueVertex", model3d_settings->vertex_color[2]);

        settings->setValue("parallelStatus", in_parallel_projec);
        settings->setValue("centralStatus", in_central_proj);
        qDebug() << "in_central_proj = " << in_central_proj << "\n";
        qDebug() << "in_parallel_proj = " << in_parallel_projec << "\n";
    }
    settings->setValue("initedStatus", model_is_inited);
}



void MainWindow::saveDefaultSettings()
{
    settings->setValue("WindowGeometry", QRect(300, 200, 800, 600));
    settings->setValue("initedStatus", false);
    settings->setValue("parallelStatus", false);
    settings->setValue("centralStatus", false);
    settings->setValue("tabwidgetIndex", 0);
}



void MainWindow::slotSettings(Model3DSettings *s)
{
    model3d_settings = s;
    if (model3d_settings->all_accept == true) {
        ui->openGLWidget->setSettings(model3d_settings);
        smth_is_happend = true;
    }
}

void MainWindow::slotTabBar(int index)
{
    if (!model_is_inited) {
        if (index == 1) {
            throwDialog();
            ui->tabWidget->setCurrentIndex(0);
        }
    }
}



/**********************************************************************/
/*                                                                    */
/*        Block of functions working at button clicked event          */
/*                                                                    */
/**********************************************************************/



void MainWindow::on_pushButton_loadmodel_clicked()
{
    smth_is_happend = true;
    count_taps_to_load_button += 1;
    if (count_taps_to_load_button % 2 == 0) {
        initZeroMovement();
        initDefaultLabelTexts();
        smth_is_happend = false;
    }

    fileway = QFileDialog::getOpenFileName(0, "Open File .obj", "/Users/", "*.obj");
    if (fileway.isEmpty()) {
        model_is_inited = false;
        return;
    }

    filename = QFileInfo(fileway).fileName();
    std::string tmp = fileway.toStdString();
    const char *fileway_c = tmp.c_str();

    model_saved = ui->openGLWidget->initModel(fileway_c);
    copyStartStateModel3m(model_saved, default_state_model,
                          model_parralel_projection,
                          model_central_projection);
    if (model_saved->_status.parse_result) {
        initLabelTexts(model_saved);
        model_is_inited = true;
    }
 }



void MainWindow::on_pushButton_moveOY_clicked()
{
    if (model_is_inited) {
        float value = ui->lineEdit_moveOY->text().toFloat();
        if (!reading_status) {
            movement->total_move_oy += value;
        }
        moveModel3D_OY(model_saved, value);
        model_saved = ui->openGLWidget->initModelAfterMove(model_saved);
        checkProjectionPos();
    }
}



void MainWindow::on_pushButton_moveOX_clicked()
{
    if (model_is_inited) {
        float value = ui->lineEdit_moveOX->text().toFloat();
        if (!reading_status) {
            movement->total_move_ox += value;
        }
        moveModel3D_OX(model_saved, value);
        model_saved = ui->openGLWidget->initModelAfterMove(model_saved);
        checkProjectionPos();
    }
}



void MainWindow::on_pushButton_moveOZ_clicked()
{
    if (model_is_inited) {
        float value = ui->lineEdit_moveOZ->text().toFloat();
        if (!reading_status) {
        movement->total_move_oz += value;
        }
        moveModel3D_OZ(model_saved, value);
        model_saved = ui->openGLWidget->initModelAfterMove(model_saved);
        checkProjectionPos();
    }
}



void MainWindow::on_pushButton_rotateOX_clicked()
{
    if (model_is_inited) {
        float angle = ui->lineEdit_rotateOX->text().toFloat();
        if (!reading_status) {
            movement->total_angle_ox += angle;
        }
        rotateModel3D(model_saved, OX, angle);
        model_saved = ui->openGLWidget->initModelAfterMove(model_saved);
        in_central_proj = false;
        in_parallel_projec = false;
    }
}



void MainWindow::on_pushButton_rotateOY_clicked()
{
    if (model_is_inited) {
        float angle = ui->lineEdit_rotateOY->text().toFloat();
        if (!reading_status) {
            movement->total_angle_oy += angle;
        }
        rotateModel3D(model_saved, OY,angle);
        model_saved = ui->openGLWidget->initModelAfterMove(model_saved);
        in_central_proj = false;
        in_parallel_projec = false;
    }
}



void MainWindow::on_pushButton_rotateOZ_clicked()
{
    if (model_is_inited) {
        float angle = ui->lineEdit_rotateOZ->text().toFloat();
        if (!reading_status) {
            movement->total_angle_oz += angle;
        }
        rotateModel3D(model_saved, OZ, angle);
        model_saved = ui->openGLWidget->initModelAfterMove(model_saved);
        in_central_proj = false;
        in_parallel_projec = false;
    }
}



void MainWindow::on_pushButton_scale_clicked()
{
    if (model_is_inited) {
        float scale = ui->lineEdit_scale->text().toFloat();
        if (scale <= 0.0) {
            return;
        }
        if (!reading_status) {
            movement->total_scale *= scale;
        }
        scaleModel3D(model_saved, scale);
        model_saved = ui->openGLWidget->initModelAfterMove(model_saved);
        checkProjectionPos();
    }
}



void MainWindow::on_pushButton_toStartPoint_clicked()
{
    if (model_is_inited) {
        ui->openGLWidget->initModelAfterMove(default_state_model);
        copyStartStateModel3m(default_state_model, model_saved,
                              model_parralel_projection, model_central_projection);
        initZeroMovement();
        in_central_proj = false;
        in_parallel_projec = true;
    }
}



void MainWindow::on_pushButton_saveImage_clicked()
{
    if (model_is_inited) {
        QPixmap pixmap(ui->openGLWidget->size());
        ui->openGLWidget->render(&pixmap, QPoint(), QRegion());
        QString way_file_screen = QFileDialog::getSaveFileName(this, "Загрузить файл", "/Users/",
                                                                "All Files (*.*);; JPEG Image (*.jpeg);; bmp Image (*.bmp)");
        pixmap.save(way_file_screen);
    }
}



void MainWindow::on_pushButton_recordScreen_clicked()
{
    if (movement->total_angle_ox == 0.0) {
        rotateModel3D(model_saved, OX, -0.2);
        ui->openGLWidget->initModelAfterMove(model_saved);
    }
    QGifImage gif(QSize(300, 300));

    QVector<QRgb> ctable;
    ctable << qRgb(255, 255, 255)
           << qRgb(0, 0, 0)
           << qRgb(255, 0, 0)
           << qRgb(0, 255, 0)
           << qRgb(0, 0, 255)
           << qRgb(255, 255, 0)
           << qRgb(0, 255, 255)
           << qRgb(255, 0, 255);

    gif.setGlobalColorTable(ctable, Qt::black);
    gif.setDefaultTransparentColor(Qt::black);
    gif.setDefaultDelay(100);

    QPixmap pixmap(ui->openGLWidget->size());
    ui->openGLWidget->render(&pixmap, QPoint(), QRegion());

    gif.addFrame(pixmap.toImage());

    for (int i=0; i<150; ++i) {
        QPixmap pixmap(ui->openGLWidget->size());
        rotateModel3D(model_saved, OY, M_PI_4 / 16);
        ui->openGLWidget->initModelAfterMove(model_saved);

        ui->openGLWidget->render(&pixmap, QPoint(), QRegion());
        gif.insertFrame(i, pixmap.toImage());
        QThread::msleep(10);
    }

    QString way_file_gif = QFileDialog::getSaveFileName(this, "Загрузить файл", "/Users/",
                                                            "All Files (*.*);; JPEG Image (*.jpeg);; bmp Image (*.bmp)");
    gif.save(way_file_gif);
    on_pushButton_toStartPoint_clicked();
}



void MainWindow::on_pushButton_parallelProjection_clicked()
{
    if (model_is_inited) {
        ui->openGLWidget->initModelAfterMove(default_state_model);
        copyStartStateModel(default_state_model, model_parralel_projection);
            scaleModel3D(model_parralel_projection, movement->total_scale);
            moveModel3D_OX(model_parralel_projection, movement->total_move_ox);
            moveModel3D_OY(model_parralel_projection, movement->total_move_oy);
            moveModel3D_OZ(model_parralel_projection, movement->total_move_oz);
        copyStartStateModel(model_parralel_projection, model_saved);
        copyStartStateModel(model_parralel_projection, model_central_projection);
        ui->openGLWidget->initModelAfterMove(model_parralel_projection);
        movement->total_angle_ox = 0.0;
        movement->total_angle_oy = 0.0;
        movement->total_angle_oz = 0.0;
        in_parallel_projec = true;
        in_central_proj = false;
        smth_is_happend = true;
    }
}



void MainWindow::on_pushButton_centralProjection_clicked()
{
    if (model_is_inited) {
        ui->openGLWidget->initModelAfterMove(default_state_model);
        copyStartStateModel(default_state_model, model_central_projection);
            on_pushButton_parallelProjection_clicked();
            rotateModel3D(model_central_projection, OY, M_PI_2);
        copyStartStateModel(model_central_projection, model_saved);
        ui->openGLWidget->initModelAfterMove(model_central_projection);
        movement->total_angle_oy = M_PI_2;
        in_central_proj = true;
        in_parallel_projec = false;
        smth_is_happend = true;
    }
}



void MainWindow::on_pushButton_settings_clicked()
{
    if (model_is_inited) {
        settings_model->show();
        smth_is_happend = true;
    }
}



void MainWindow::on_pushButton_easterEgg_clicked()
{
    if (model_is_inited) {
        easted_egg->show();
        smth_is_happend = true;
    }
}



void MainWindow::checkProjectionPos()
{
    if (movement->total_angle_ox == 0.0 && movement->total_angle_oy == 0.0 && movement->total_angle_oz == 0.0) {
        in_parallel_projec = true;
    } else {
        in_parallel_projec = false;
    }
    if (movement->total_angle_ox == 0.0 && fabs(movement->total_angle_oy - M_PI_2) <= 1e-6 && movement->total_angle_oz == 0.0) {
        in_central_proj = true;
    } else {
        in_central_proj = false;
    }
}



/**********************************************************************/
/*                                                                    */
/*    Block of functions working at initialize the state of model     */
/*                                                                    */
/**********************************************************************/



void MainWindow::initStateWindow()
{
    filename = settings->value("initedFilename", "").toString();
    fileway = settings->value("initedFileway", "").toString();

    std::string tmp = fileway.toStdString();
    const char *fileway_c = tmp.c_str();
    model_saved = ui->openGLWidget->initModel(fileway_c);

    copyStartStateModel3m(model_saved, default_state_model,
                          model_parralel_projection,
                          model_central_projection);
    if (model_saved->_status.parse_result) {
        initLabelTexts(model_saved);
        count_taps_to_load_button = 1;
        model_is_inited = true;
        smth_is_happend = true;
    }
}



void MainWindow::initStateMovement()
{
    movement->total_move_ox =   settings->value("moveOx", 0.0).toFloat();
    movement->total_move_oy =   settings->value("moveOy", 0.0).toFloat();
    movement->total_move_oz =   settings->value("moveOz", 0.0).toFloat();
    movement->total_angle_ox =  settings->value("angleOx", 0.0).toFloat();
    movement->total_angle_oy =  settings->value("angleOy", 0.0).toFloat();
    movement->total_angle_oz =  settings->value("angleOz", 0.0).toFloat();
    movement->total_scale =     settings->value("scale", 1.0).toFloat();

    int have_projection = initStateProjection();

    if (!have_projection) {
        if (movement->total_scale != 1.0) {
            scaleModel3D(model_saved, movement->total_scale);
        }
        if (movement->total_move_ox != 0.0) {
            moveModel3D_OX(model_saved, movement->total_move_ox);
        }
        if (movement->total_move_oy != 0.0) {
            moveModel3D_OY(model_saved, movement->total_angle_oy);
        }
        if (movement->total_move_oz != 0.0) {
            moveModel3D_OZ(model_saved, movement->total_angle_oz);
        }
        if (movement->total_angle_ox != 0.0) {
            rotateModel3D(model_saved, OX, movement->total_angle_ox);
        }
        if (movement->total_angle_oy != 0.0) {
            rotateModel3D(model_saved, OY, movement->total_angle_oy);
        }
        if (movement->total_angle_oz != 0.0) {
            rotateModel3D(model_saved, OZ, movement->total_angle_oz);
        }
        ui->openGLWidget->initModelAfterMove(model_saved);
    }
    smth_is_happend = true;
}



int MainWindow::initStateProjection()
{
    int have_projection = 0;
    if (in_parallel_projec) {
        on_pushButton_parallelProjection_clicked();
        have_projection = 1;
    } else if (in_central_proj) {
        on_pushButton_centralProjection_clicked();
        have_projection = 1;
    } else {
        have_projection = 0;
    }
    return have_projection;
}



void MainWindow::initSettingsState()
{
    model3d_settings->rib_size = settings->value("ribSize", 0.5f).toFloat();
    model3d_settings->vertex_size = settings->value("vertexSize", 5.0f).toFloat();

    model3d_settings->rib_type = settings->value("ribType", 0).toInt();
    model3d_settings->vertex_showness = settings->value("vertexType", 2).toInt();

    model3d_settings->all_accept = true;
    model3d_settings->bg_color[0] = settings->value("redBG", 0.0).toFloat();
    model3d_settings->bg_color[1] = settings->value("greenBG", 0.0).toFloat();
    model3d_settings->bg_color[2] = settings->value("blueBG", 0.0).toFloat();

    model3d_settings->rib_color[0] = settings->value("redRib", 0.0).toFloat();
    model3d_settings->rib_color[1] = settings->value("greenRib", 1.0).toFloat();
    model3d_settings->rib_color[2] = settings->value("blueRib", 0.0).toFloat();

    model3d_settings->vertex_color[0] = settings->value("redVertex", 1.0).toFloat();
    model3d_settings->vertex_color[1] = settings->value("greenVertex", 0.0).toFloat();
    model3d_settings->vertex_color[2] = settings->value("blueVertex", 0.0).toFloat();

    ui->openGLWidget->setSettings(model3d_settings);
    smth_is_happend = true;
}



/**********************************************************************/
/*                                                                    */
/*        Block of functions working at initialize the labels         */
/*                                                                    */
/**********************************************************************/



void MainWindow::initDefaultLabelTexts()
{
    ui->label_total_vertexes->setText("None");
    ui->label_total_poligons->setText("None");
    ui->label_location_model->setText("None");
    ui->label_name_of_model->setText("None");
    ui->openGLWidget->zeroModel();
    ui->lineEdit_moveOX->setText("0.25");
    ui->lineEdit_moveOY->setText("0.25");
    ui->lineEdit_moveOZ->setText("0.25");
    ui->lineEdit_rotateOX->setText("0.25");
    ui->lineEdit_rotateOY->setText("0.25");
    ui->lineEdit_rotateOZ->setText("0.25");
    ui->lineEdit_scale->setText("0.95");
}



void MainWindow::initLabelTexts(Model3D *model_saved)
{
    ui->label_total_vertexes->setText(QString::number(model_saved->_status.total_vertexes));
    ui->label_total_poligons->setText(QString::number(model_saved->_status.total_poligons));
    ui->label_location_model->setText(fileway);
    ui->label_name_of_model->setText(filename);
}



void MainWindow::initZeroMovement()
{
    movement->total_angle_ox = 0.0;
    movement->total_angle_oy = 0.0;
    movement->total_angle_oz = 0.0;
    movement->total_move_ox = 0.0;
    movement->total_move_oy = 0.0;
    movement->total_move_oz = 0.0;
    movement->total_scale = 1.0;
}



/**********************************************************************/
/*                                                                    */
/*      Override functions working at different event of window       */
/*                                                                    */
/**********************************************************************/



void MainWindow::closeEvent(QCloseEvent *event)
{
    if (smth_is_happend) {
        event->ignore();
        int ret = QMessageBox::question(this,
                                        "Close window", "Do you want to save the state of program?",
                                        QMessageBox::Yes | QMessageBox::No);

        if (QMessageBox::Yes == ret) {
            saveSettings();
            event->accept();
        } else if (QMessageBox::No == ret) {
            saveDefaultSettings();
            event->accept();
        }
    } else {
        event->accept();
    }
}



void MainWindow::resizeEvent(QResizeEvent *event)
{
    event->accept();
    if (QRect(300, 200, 800, 600) == geometry() && !model_is_inited) {
        smth_is_happend = false;
    } else {
        smth_is_happend = true;
    }
}



void MainWindow::moveEvent(QMoveEvent *event)
{
    event->accept();
    if (my_old_pos == pos()) {
        smth_is_happend = false;
    } else {
        smth_is_happend = true;
    }
}



void MainWindow::throwDialog()
{
    QMessageBox::information(this, "3DViewer", "Load the model to use! <3");
}
