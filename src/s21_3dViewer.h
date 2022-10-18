#ifndef _SRC_S21_3DVIEWER_H_
#define _SRC_S21_3DVIEWER_H_

/*===============================================================/
/                                                                /
/                 S21_3DVIEWER PROJECT SCHOOL 21                 /
/                 BY TONITAGA JERLENEM RASHIDAG                  /
/                                                                /
/===============================================================*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OK 1
#define ERROR 0

typedef unsigned int uint;

/**
 * @brief struct MainDataModel
 * @param parse_result результат возврата
 * @param total_coords общее количество отделенных по осям координат
 * @param total_vertexes общее количество координат (вершин)
 * @param total_indexes общее количество индексов во всех полигонах
 * @param total_poligons общее количество полигонов
 * @param memory_status результат состояния памяти
 */
typedef struct {
  uint parse_result;
  uint total_coords;
  uint total_vertexes;
  uint total_indexes;
  uint total_poligons;
  uint memory_status;
} MainDataModel;

/**
 * @brief struct Model3D - главная структура проекта
 * @param _vertex_array массив вершин
 * @param _index_array массив индексов
 * @param _status структура данных и результатов
 */
typedef struct {
  float *_vertex_array;
  uint *_index_array;
  MainDataModel _status;
} Model3D;

/*      Main functions of project       */

void parseModel3D(Model3D *model, const char *obj_fileway);
void moveModel3D_OX(Model3D *model, float move_value);
void moveModel3D_OY(Model3D *model, float move_value);
void moveModel3D_OZ(Model3D *model, float move_value);
void scaleModel3D(Model3D *model, float scale);
void rotateModel3D(Model3D *model, int OX_OY_OZ, double angle);

void copyStartStateModel(Model3D *this_s, Model3D *other);
void copyStartStateModel3m(Model3D *this_s, Model3D *other1, Model3D *other2,
                           Model3D *other3);

#define OX 0
#define OY 1
#define OZ 2

#if defined _3D_VIEWER_HELP_FUNCTIONS_
void __countVertexesFacets(Model3D *model, FILE *file);
void __initializeModel3D(Model3D *model);
void __initializeVertexIndexArrays(Model3D *model, FILE *file);
void __fillIndexArray(Model3D *model, char *line, int *iai);
void __fillVertexArray(Model3D *model, char *line, int *vai);
int __isDigit(char s);
void __createVertexIndexArrays(Model3D *model);
void __removeVertexIndexArrays(Model3D *model);
#endif  // !_3D_VIEWER_HELP_FUNCTIONS_

#endif  // !_SRC_S21_3DVIEWER_H_
