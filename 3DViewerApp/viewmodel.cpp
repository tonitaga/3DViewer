#define GL_SILENCE_DEPRECATION
#include "viewmodel.h"
#include <QDebug>
#include <clocale>
#include <algorithm>

ViewModel::ViewModel(QWidget *parent)
    : QOpenGLWidget{parent}, indexes_model_size(0),
      vertexes_model_size(0), OZ_max_elenemt(0), OZ_min_elenemt(0),
      model_is_inited(false), memory_freed(true), line_type(0xFFFF), point_type(2)
{
    setlocale(LC_ALL, "en_US.UTF-8");
    model = new Model3D;
    settings = new Model3DSettings();
}




ViewModel::~ViewModel()
{
    if (!memory_freed) {
        delete [] vertexes_model;
        delete [] indexes_model;
    }
    if (model_is_inited) {
        __removeVertexIndexArrays(model);
    }
    delete model;
}




void ViewModel::initializeGL()
{
    glClearColor(settings->bg_color[0], settings->bg_color[1], settings->bg_color[2], 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}





void ViewModel::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    aspected_widget_width = w / 20;
    aspected_widget_height = h / 20;
    int widget_left = -aspected_widget_width / 2;
    int widget_right = aspected_widget_width  / 2;
    int widget_bottom = -aspected_widget_height / 2;
    int widget_top = aspected_widget_height / 2;
    int view_zone = 100;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(widget_left, widget_right,
            widget_bottom, widget_top,
            view_zone, -view_zone);
}




void ViewModel::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(settings->bg_color[0], settings->bg_color[1], settings->bg_color[2], 1.0);
    drawModel();
}





Model3D *ViewModel::initModel(const char *fileway)
{
    setlocale(LC_ALL, "en_US.UTF-8");
    parseModel3D(model, fileway);
    if (model->_status.parse_result) {
        vertexes_model_size = model->_status.total_coords;
        indexes_model_size = model->_status.total_indexes;

        vertexes_model = new GLfloat[vertexes_model_size];
        indexes_model =  new GLuint[indexes_model_size];

        for (uint i = 0; i < vertexes_model_size; i++) {
            vertexes_model[i] = model->_vertex_array[i];
        }

        for (uint i = 0; i < indexes_model_size; i++) {
            indexes_model[i] = model->_index_array[i] - 1;
        }
        model_is_inited = true;
    }
    memory_freed = false;

    GLfloat *fabs_vertex_array = new GLfloat[model->_status.total_coords];
    for (uint i = 0; i < model->_status.total_coords; i++) {
        fabs_vertex_array[i] = fabs(vertexes_model[i]);
    }

    OXOYOZ_max_element = std::max_element(fabs_vertex_array, fabs_vertex_array + model->_status.total_coords);
    if (*OXOYOZ_max_element >= 20) {
        float aspect = *OXOYOZ_max_element / 10.0;
        slaceBigModel(aspect);
    }
    qDebug() << "OXOYOZ_max fabs element: " << *OXOYOZ_max_element << "\n";
    return model;
}



void ViewModel::slaceBigModel(float aspect)
{
    for (uint i = 0; i < model->_status.total_coords; i++) {
        model->_vertex_array[i] /= aspect;
    }
    initModelAfterMove(model);
}




Model3D *ViewModel::initModelAfterMove(Model3D *m_model)
{
    for (uint i = 0; i < vertexes_model_size; i++) {
        vertexes_model[i] = m_model->_vertex_array[i];
    }
    update();
    return m_model;
}

void ViewModel::orthoModel()
{
    int widget_left = -aspected_widget_width / 2;
    int widget_right = aspected_widget_width  / 2;
    int widget_bottom = -aspected_widget_height / 2;
    int widget_top = aspected_widget_height / 2;

    glLoadIdentity();
    glOrtho(widget_left, widget_right, widget_bottom, widget_top, 50, 50);
    update();
}

void ViewModel::zeroModel()
{
    if (!memory_freed) {
        delete [] vertexes_model;
        delete [] indexes_model;
        indexes_model_size = 0;
        vertexes_model_size = 0;
        memory_freed = true;
    }
    update();
}




void ViewModel::setSettings(Model3DSettings *s)
{
    settings = s;
    line_type = 0xFFFF;
    if (settings->rib_type == 1) {
        line_type = 0x3F07;
    }
    point_type = settings->vertex_showness;
    repaint();
    update();
}

Model3DSettings *ViewModel::getSettings()
{
    return settings;
}



void ViewModel::drawModel()
{
    glLineStipple(1, line_type);
    glEnable(GL_LINE_STIPPLE);
    glVertexPointer(3, GL_FLOAT, 0, vertexes_model);
    glEnableClientState(GL_VERTEX_ARRAY);
        if (point_type != 0) {
            if (point_type == 1) {
                glEnable(GL_POINT_SMOOTH);
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            } else {
                glDisable(GL_POINT_SMOOTH);
                glDisable(GL_BLEND);
            }
            glPointSize(settings->vertex_size);
            glColor3f(settings->vertex_color[0], settings->vertex_color[1], settings->vertex_color[2]);
            glDrawElements(GL_POINTS, indexes_model_size, GL_UNSIGNED_INT, indexes_model);
        }
        glLineWidth(settings->rib_size);
        glColor3f(settings->rib_color[0], settings->rib_color[1], settings->rib_color[2]);
        glDrawElements(GL_LINES, indexes_model_size, GL_UNSIGNED_INT, indexes_model);
        glDisableClientState(GL_VERTEX_ARRAY);
}

