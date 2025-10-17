#include "trap.h"

#define N 32
char *test0 = "dasdabucbzau231897das;dla'";
char *test1 = "dsaudbiazug81318y0dpczoicnpllsal";
char *test2 = "dasdabucbzau231897das;dla'";
char *test3 = "dasdabucbzau231897das;dla'pllsal";

int ans[] = { 0, 0, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, 
              0, 0, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, -18, 
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -112, -112, -112, -112, 
              0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -112, -112, -112, -112, -112, -112, -112, -112, -112, -112,  };

void test_strncmp() {
  int ans_index = 0;
  for (int i = 0; i < strlen(test0) + 5; i++) {
    check(strncmp(test0, test1, i) == ans[ans_index++]);
  }
  for (int i = 0; i < strlen(test1) + 5; i++) {
    check(strncmp(test0, test1, i) == ans[ans_index++]);
  }
  for (int i = 0; i < strlen(test2) + 5; i++) {
    check(strncmp(test2, test3, i) == ans[ans_index++]);
  }
  for (int i = 0; i < strlen(test3) + 5; i++) {
    check(strncmp(test2, test3, i) == ans[ans_index++]);
  }
}

int main(){
  test_strncmp();

  return 0;
}
