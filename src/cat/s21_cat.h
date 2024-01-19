#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <getopt.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  int n_flag;
  int b_flag;
  int s_flag;
  int t_flag;
  int e_flag;
  int v_flag;
  int countLines;
  int countEmptyLines;

} CatOptions;

int processFile(const char *filename, CatOptions options);
CatOptions parseOptions(int argc, char *argv[]);

#endif
