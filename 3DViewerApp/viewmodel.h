#ifndef VIEWMODEL_H
#define VIEWMODEL_H
#include "settingsmodel.h"

#include <QOpenGLWidget>

extern "C" {
    #define _3D_VIEWER_HELP_FUNCTIONS_
    #include "../src/s21_3dViewer.h"
}

class ViewModel : public QOpenGLWidget
{
    Q_OBJECT
public:
    ViewModel(QWidget *parent = nullptr);
    ~ViewModel();

    Model3D *initModel(const char *fileway);
    Model3D *initModelAfterMove(Model3D *m_model);
    void     orthoModel();
    void     zeroModel();
    void     setSettings(Model3DSettings *settings);
    Model3DSettings *getSettings();


protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void drawModel();
//    void setSettingsModel();

private:
    void slaceBigModel(float aspect);
    Model3DSettings *settings;
    SettingsModel *class_settings;
    Model3D *model;

    const char *m_fileway;
    GLfloat *vertexes_model;
    GLuint *indexes_model;
    GLuint indexes_model_size;
    GLuint vertexes_model_size;

    float *OX_max_element;
    float *OX_min_element;
    float *OY_max_element;
    float *OY_min_element;
    float *OZ_max_elenemt;
    float *OZ_min_elenemt;

    float *OXOYOZ_max_element;
    float aspected_widget_width;
    float aspected_widget_height;

    bool model_is_inited;
    bool memory_freed;

    GLushort line_type;
    GLint point_type;
};

#endif // VIEWMODEL_H
