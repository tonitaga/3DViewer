#define _3D_VIEWER_HELP_FUNCTIONS_
#include "./s21_3dViewer.h"

/*===============================================================/
/                                                                /
/                 S21_3DVIEWER PROJECT SCHOOL 21                 /
/                 BY TONITAGA JERLENEM RASHIDAG                  /
/                                                                /
/===============================================================*/

/**
 * @brief Функция двигает модель по оси OX на
 * расстояние move_value
 * @param model модель которую нужно сдвинуть
 * @param move_value значение на которое нужно сдвинуть
 */
void moveModel3D_OX(Model3D *model, float move_value) {
  for (uint i = OX; i < model->_status.total_coords; i += 3) {
    model->_vertex_array[i] += move_value;
  }
}

/**
 * @brief Функция двигает модель по оси OY на
 * расстояние move_value
 * @param model модель которую нужно сдвинуть
 * @param move_value значение на которое нужно сдвинуть
 */
void moveModel3D_OY(Model3D *model, float move_value) {
  for (uint i = OY; i < model->_status.total_coords; i += 3) {
    model->_vertex_array[i] += move_value;
  }
}

/**
 * @brief Функция двигает модель по оси OZ на
 * расстояние move_value
 * @param model модель которую нужно сдвинуть
 * @param move_value значение на которое нужно сдвинуть
 */
void moveModel3D_OZ(Model3D *model, float move_value) {
  for (uint i = OZ; i < model->_status.total_coords; i += 3) {
    model->_vertex_array[i] += move_value;
  }
}

/**
 * @brief Функция увеличивает модель или уменьшает на
 * значение scale
 * @param model модель которую нужно сдвинуть
 * @param scale значение зума модели
 */
void scaleModel3D(Model3D *model, float scale) {
  if (scale == 0.0) {
    return;
  }
  for (uint i = 0; i < model->_status.total_coords; i++) {
    model->_vertex_array[i] *= scale;
  }
}

/**
 * @brief Функция вращает модель по одной из
 * выбранных осей на значение angle
 * @param model модель которую нужно сдвинуть
 * @param OX_OY_OZ  относительно какой оси будет вращение
 * @param 0 относительно оси OX
 * @param 1 относительно оси OY
 * @param 2 относительно оси OZ
 * @param angle угол поворота
 */
void rotateModel3D(Model3D *model, int OX_OY_OZ, double angle) {
  double temp_x = 0.0;
  double temp_y = 0.0;
  double temp_z = 0.0;
  switch (OX_OY_OZ) {
    case OX:
      for (uint i = 0; i < model->_status.total_coords; i += 3) {
        temp_x = model->_vertex_array[i];
        temp_y = model->_vertex_array[i + 1];
        temp_z = model->_vertex_array[i + 2];

        model->_vertex_array[i] = temp_x;
        model->_vertex_array[i + 1] = cos(angle) * temp_y - sin(angle) * temp_z;
        model->_vertex_array[i + 2] = sin(angle) * temp_y + cos(angle) * temp_z;
      }
      break;
    case OY:
      for (uint i = 0; i < model->_status.total_coords; i += 3) {
        temp_x = model->_vertex_array[i];
        temp_y = model->_vertex_array[i + 1];
        temp_z = model->_vertex_array[i + 2];

        model->_vertex_array[i] = cos(angle) * temp_x + sin(angle) * temp_z;
        model->_vertex_array[i + 1] = temp_y;
        model->_vertex_array[i + 2] =
            -sin(angle) * temp_x + cos(angle) * temp_z;
      }
      break;
    case OZ:
      for (uint i = 0; i < model->_status.total_coords; i += 3) {
        temp_x = model->_vertex_array[i];
        temp_y = model->_vertex_array[i + 1];
        temp_z = model->_vertex_array[i + 2];

        model->_vertex_array[i] = cos(angle) * temp_x - sin(angle) * temp_y;
        model->_vertex_array[i + 1] = sin(angle) * temp_x + cos(angle) * temp_y;
        model->_vertex_array[i + 2] = temp_z;
      }
      break;
  }
}

void copyStartStateModel(Model3D *this, Model3D *other) {
  other->_status = this->_status;
  __createVertexIndexArrays(other);
  for (uint i = 0; i < other->_status.total_coords; i++) {
    other->_vertex_array[i] = this->_vertex_array[i];
  }
  for (uint i = 0; i < other->_status.total_indexes; i++) {
    other->_index_array[i] = this->_index_array[i];
  }
}

void copyStartStateModel3m(Model3D *this_s, Model3D *other1, Model3D *other2,
                           Model3D *other3) {
  other1->_status = this_s->_status;
  other2->_status = this_s->_status;
  other3->_status = this_s->_status;
  __createVertexIndexArrays(other1);
  __createVertexIndexArrays(other2);
  __createVertexIndexArrays(other3);

  for (uint i = 0; i < this_s->_status.total_coords; i++) {
    other1->_vertex_array[i] = this_s->_vertex_array[i];
    other2->_vertex_array[i] = this_s->_vertex_array[i];
    other3->_vertex_array[i] = this_s->_vertex_array[i];
  }
  for (uint i = 0; i < this_s->_status.total_indexes; i++) {
    other1->_index_array[i] = this_s->_index_array[i];
    other2->_index_array[i] = this_s->_index_array[i];
    other3->_index_array[i] = this_s->_index_array[i];
  }
}
