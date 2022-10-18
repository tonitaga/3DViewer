#include <check.h>
#include <stdio.h>
#define _3D_VIEWER_HELP_FUNCTIONS_
#include "./s21_3dViewer.h"

/*===============================================================/
/                                                                /
/                 S21_3DVIEWER PROJECT SCHOOL 21                 /
/                 BY TONITAGA JERLENEM RASHIDAG                  /
/                                                                /
/===============================================================*/

START_TEST(result_test_1) {
  printf("------------------1-test-begin------------------\n");
  Model3D *thiss = (Model3D *)malloc(sizeof(Model3D));
  Model3D *other = (Model3D *)malloc(sizeof(Model3D));

  parseModel3D(thiss, "./ObjFiles/deer.obj");
  copyStartStateModel(thiss, other);
  if (thiss->_status.total_coords == other->_status.total_coords) {
    printf("\n--                  Sucsess                   --\n\n");
  }
  rotateModel3D(thiss, OX, 1);
  rotateModel3D(thiss, OY, 1);
  rotateModel3D(thiss, OZ, 1);
  scaleModel3D(thiss, 1.1);
  moveModel3D_OX(thiss, 1);
  moveModel3D_OY(thiss, 1);
  moveModel3D_OZ(thiss, 1);
  __removeVertexIndexArrays(thiss);
  __removeVertexIndexArrays(other);
  free(thiss);
  free(other);
  printf("----------------1-test-completed----------------\n\n");
}
END_TEST

START_TEST(result_test_2) {
  printf("------------------2-test-begin------------------\n");
  Model3D *thiss = (Model3D *)malloc(sizeof(Model3D));
  Model3D *other1 = (Model3D *)malloc(sizeof(Model3D));
  Model3D *other2 = (Model3D *)malloc(sizeof(Model3D));
  Model3D *other3 = (Model3D *)malloc(sizeof(Model3D));

  parseModel3D(thiss, "./ObjFiles/deer.obj");
  copyStartStateModel3m(thiss, other1, other2, other3);
  if (thiss->_status.total_coords == other1->_status.total_coords &&
      other2->_status.total_coords && other3->_status.total_coords) {
    printf("\n--                  Sucsess                   --\n\n");
  }
  __removeVertexIndexArrays(thiss);
  __removeVertexIndexArrays(other1);
  __removeVertexIndexArrays(other2);
  __removeVertexIndexArrays(other3);
  free(thiss);
  free(other1);
  free(other2);
  free(other3);
  printf("----------------2-test-completed----------------\n\n");
}
END_TEST

START_TEST(result_test_3) {
  printf("------------------3-test-begin------------------\n");
  Model3D *thiss = (Model3D *)malloc(sizeof(Model3D));
  parseModel3D(thiss, "./ObjFiles/my_cube.obj");

  if (thiss->_status.total_vertexes == 8 &&
      thiss->_status.total_poligons == 12) {
    printf("\n--                  Sucsess                   --\n\n");
  }

  __removeVertexIndexArrays(thiss);
  free(thiss);
  printf("----------------3-test-completed----------------\n\n");
}
END_TEST

int main(void) {
  Suite *s1 = suite_create("Core");
  TCase *tc1_1 = tcase_create("Core");
  SRunner *sr = srunner_create(s1);
  int nf;

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, result_test_1);
  tcase_add_test(tc1_1, result_test_2);
  tcase_add_test(tc1_1, result_test_3);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
