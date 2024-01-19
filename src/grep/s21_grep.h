
#ifndef C3_SIMPLEBASHUTILS_1_S21_GREP_H
#define C3_SIMPLEBASHUTILS_1_S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int e_flag;
  int i_flag;
  int v_flag;
  int c_flag;
  int l_flag;
  int n_flag;
  int h_flag;
  int s_flag;
  int f_flag;
  int o_flag;
  int countFiles;
  int numberLine;
  int countMatches;
} GrepOptions;

int parseOptions(int argc, char **argv, GrepOptions *options, char **pattern);
void mainProcess(char *str, GrepOptions *GrepOptions, char *pattern,
                 char *name);
void processFile(FILE *file, GrepOptions *options, char *pattern, char *path);
void optionF(GrepOptions *options, char **pattern, char *optarg);
void optionE(GrepOptions *options, char **pattern, char *optarg);
void outOptionsO(regex_t my_regex, char *str, GrepOptions options);
void auxProcess(GrepOptions options, char *path);
void *reallocString(void *str, int size);
int setupPattern(char **str, char *optarg);
void mainOptions(GrepOptions *options, int argc);
void addPattern(char **str, char *optarg);

#endif  // C3_SIMPLEBASHUTILS_1_S21_GREP_H
