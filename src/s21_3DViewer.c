#define _3D_VIEWER_HELP_FUNCTIONS_
#include "./s21_3dViewer.h"

/*===============================================================/
/                                                                /
/                 S21_3DVIEWER PROJECT SCHOOL 21                 /
/                 BY TONITAGA JERLENEM RASHIDAG                  /
/                                                                /
/===============================================================*/

/**
 * @brief
 * Функция занимается парсингом файла с расширением .obj
 * Создает динамические одномерные массивы с уже нужным
 * форматом данных для проекта. Для очистки памяти используйте функцию
 * void __removeVertexIndexArrays(Model3D *model)
 * @param model структура куда будут сохранены данные
 * @param obj_fileway путь к файлу с которым нужно работать
 * @return Функция ничего не возвращает, но при неуспешном парсинге
 * файла возвращает в model->_status.parse_result = ERROR
 */
void parseModel3D(Model3D *model, const char *obj_fileway) {
  FILE *file;
  if ((file = fopen(obj_fileway, "r")) != NULL) {
    __initializeModel3D(model);
    __countVertexesFacets(model, file);
    if (model->_status.parse_result) {
      __createVertexIndexArrays(model);
      if (model->_status.memory_status == OK)
        __initializeVertexIndexArrays(model, file);
      else
        model->_status.parse_result = ERROR;
    } else
      model->_status.parse_result = ERROR;
    fclose(file);
  } else
    model->_status.parse_result = ERROR;
}

/**
 * @brief
 * Функция считает количество общее число вершин и полигонов
 * в файле, также подсчитывает суммарное количество вершин для
 * соединения, и считает общее количество координат, т.е умножает
 * количество вершин на три, так как три координаты (X, Y, Z)
 * @param model Структура куда будут сохранены данные
 * @param file Файл в котором нужно подсчитать данные
 *
 * @return Функция ничего не возвращает, но, при неуспешном проходе
 * функции должна возвращать в _status.parse_result = ERROR
 */
void __countVertexesFacets(Model3D *model, FILE *file) {
  char line[1024] = {0};
  while (fgets(line, sizeof(line), file) != NULL) {
    switch (line[0]) {
      case '#':
        continue;
        break;
      case 'v':
        if (line[1] == ' ') model->_status.total_vertexes += 1;
        continue;
        break;
      case 'f':
        if (line[1] == ' ') model->_status.total_poligons += 1;
        for (uint i = 0; i < strlen(line); i++) {
          if (line[i] == ' ' && __isDigit(line[i + 1]))
            model->_status.total_indexes += 1;
        }
        continue;
        break;
    }
  }

  model->_status.total_coords = model->_status.total_vertexes * 3;
  model->_status.total_indexes *= 2;

  fseek(file, 0, SEEK_SET);
  model->_status.parse_result = OK;
}

/**
 * @brief
 * Функция заполняет вершинный и индексный массивы данными из
 * файла в уже нужной форме
 * @param model Структура куда будут сохранены данные
 * @param file Файл в котором нужно подсчитать данные
 */
void __initializeVertexIndexArrays(Model3D *model, FILE *file) {
  char line[1024] = {0};
  int vai = 0;  // vai = vertex array index
  int iai = 0;  // iai = indexes array index

  while ((fgets(line, sizeof(line), file)) != NULL) {
    switch (line[0]) {
      case 'v':
        if (line[1] == ' ') {
          __fillVertexArray(model, line, &vai);
        }
        break;
      case 'f':
        if (line[1] == ' ') {
          __fillIndexArray(model, line, &iai);
        }
        break;
    }
  }
  fseek(file, 0, SEEK_SET);
  model->_status.parse_result = OK;
}

/**
 * @brief
 * Функция заполняет массив вершин данными из строки.
 * Функция сделана для декомпозиции кода
 * @param model Структура куда будут сохранены данные
 * @param line Строка из которой нужно вытащить данные
 * @param vai Индекс массива вершин  vai = vertex array index
 */
void __fillVertexArray(Model3D *model, char *line, int *vai) {
  model->_vertex_array[*vai] = 0.0f;
  model->_vertex_array[(*vai) + 1] = 0.0f;
  model->_vertex_array[(*vai) + 2] = 0.0f;
  sscanf(line, "v %f %f %f", &model->_vertex_array[*vai],
         &model->_vertex_array[(*vai) + 1], &model->_vertex_array[(*vai) + 2]);
  (*vai) += 3;
}

/**
 * @brief
 * Функция заполняет массив индексов данными из строки.
 * Функция сделана для декомпозиции кода
 * @param model Структура куда будут сохранены данные
 * @param line Строка из которой нужно вытащить данные
 * @param iai Индекс массива индексов iai = indexes array index
 */
void __fillIndexArray(Model3D *model, char *line, int *iai) {
  uint stored_value = 0;  // Для дублирования чисел
  uint first_value = 0;   // Для замыкания полигона
  uint ordinal_value = 0;  // Порядковый номер записанного числа
  for (uint i = 0; i < strlen(line); i++) {
    if (line[i] == ' ' && __isDigit(line[i + 1])) {
      ordinal_value += 1;
      char *str = &line[i];
      while (__isDigit(line[i])) {
        i++;
        str++;
        str = &line[i];
      }

      stored_value = atoi(str);
      model->_index_array[*iai] = stored_value;

      if (ordinal_value == 1) {
        first_value = model->_index_array[*iai];
        (*iai)++;
      }
      if (ordinal_value != 1) {
        model->_index_array[++(*iai)] = stored_value;
        (*iai)++;
      }
    }
  }
  model->_index_array[(*iai)++] = first_value;
}

/**
 * @brief
 * Функция выделяет память для массива вершин и индексов
 * @param model Структура где будут выделена память
 */
void __createVertexIndexArrays(Model3D *model) {
  model->_vertex_array =
      (float *)malloc(sizeof(float) * model->_status.total_coords);
  if (model->_vertex_array == NULL) {
    model->_status.memory_status = ERROR;
    free(model->_vertex_array);
    return;
  }
  model->_index_array =
      (uint *)malloc(sizeof(uint) * model->_status.total_indexes);
  if (model->_index_array == NULL) {
    model->_status.memory_status = ERROR;
    free(model->_index_array);
    return;
  }
}

void __removeVertexIndexArrays(Model3D *model) {
  free(model->_vertex_array);
  free(model->_index_array);
}

/**
 * @brief
 * Функция заполняет структуры начальными значениями
 * @param model Структура
 */
void __initializeModel3D(Model3D *model) {
  model->_status.parse_result = OK;
  model->_status.total_coords = 0;
  model->_status.total_indexes = 0;
  model->_status.total_vertexes = 0;
  model->_status.total_poligons = 0;
  model->_status.memory_status = OK;
}

int __isDigit(char s) {
  int result = ERROR;
  if ((s >= '0' && s <= '9') || (s == '.' || s == ',')) result = OK;
  return result;
}
