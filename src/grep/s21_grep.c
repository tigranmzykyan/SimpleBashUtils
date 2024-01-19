#include "s21_grep.h"

int main(int argc, char **argv) {
  if (argc > 1) {
    char *temp = NULL;
    GrepOptions options = {0};

    int x = parseOptions(argc, argv, &options, &temp);
    while (x < argc) {
      FILE *file = fopen(argv[x], "r");
      if (file != NULL) {
        processFile(file, &options, temp, argv[x]);
        fclose(file);
      } else if (options.s_flag == 0) {
        fprintf(stderr, "s21_grep: %s: No such file or directory\n", argv[x]);
      }
      x++;
    }

    if (temp != NULL) free(temp);
  }
  return 0;
}

void processFile(FILE *file, GrepOptions *options, char *pattern, char *path) {
  for (char ch = '0'; (ch = getc(file)) != EOF;) {
    char *line = calloc(256, sizeof(char));
    options->numberLine += 1;
    int length = 0;

    for (line[length] = '\0'; ch != EOF && ch != '\n'; ch = getc(file)) {
      line[length] = ch;
      line[length += 1] = '\0';
      if (length % 255 == 0) {
        line = reallocString(line, length + 256);
      }
    }

    mainProcess(line, options, pattern, path);
    free(line);
  }
  auxProcess(*options, path);
}

int parseOptions(int argc, char **argv, GrepOptions *options, char **pattern) {
  int option;
  while ((option = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (option) {
      case 'i':
        options->i_flag = 1;
        break;
      case 'v':
        options->v_flag = 1;
        break;
      case 'c':
        options->c_flag = 1;
        break;
      case 'l':
        options->l_flag = 1;
        break;
      case 'n':
        options->n_flag = 1;
        break;
      case 'h':
        options->h_flag = 1;
        break;
      case 's':
        options->s_flag = 1;
        break;
      case 'o':
        options->o_flag = 1;
        break;
      case 'e':
        optionE(options, pattern, optarg);
        break;
      case 'f':
        optionF(options, pattern, optarg);
        break;
    }
  }

  if ((options->e_flag || options->f_flag) == 0) {
    setupPattern(pattern, argv[optind]);
    optind += 1;
  }

  mainOptions(options, argc);
  return optind;
}

void optionF(GrepOptions *options, char **pattern, char *optarg) {
  FILE *file = fopen(optarg, "r");
  if (file != NULL) {
    for (char ch = '0'; (ch = getc(file)) != EOF;) {
      int length = 0;
      char *line = calloc(256, sizeof(char));

      for (line[length] = '\0'; ch != EOF && ch != '\n'; ch = getc(file)) {
        line[length] = ch;
        line[length += 1] = '\0';
        if (length % 255 == 0) line = (char *)reallocString(line, length + 256);
      }

      if (!(options->e_flag || options->f_flag))
        options->f_flag = setupPattern(pattern, line);
      else
        addPattern(pattern, line);
      free(line);
    }
    fclose(file);
  } else if (options->s_flag == 0) {
    fprintf(stderr, "s21_grep: %s: No such file or directory\n", optarg);
  }
}

void optionE(GrepOptions *options, char **pattern, char *optarg) {
  if (!(options->e_flag || options->f_flag))
    options->e_flag = setupPattern(pattern, optarg);
  else
    addPattern(pattern, optarg);
}

void auxProcess(GrepOptions options, char *path) {
  if (options.c_flag) {
    if (options.l_flag) {
      options.countFiles > 1 ? printf("%s:1\n", path) : printf("1\n");
    } else {
      if (options.countFiles > 1) printf("%s:", path);
      printf("%i\n", options.countMatches);
    }
  }
  if (options.l_flag && options.countMatches) printf("%s\n", path);
}

void mainProcess(char *line, GrepOptions *options, char *pattern, char *path) {
  regex_t regex;
  if (regcomp(&regex, pattern, options->i_flag ? REG_ICASE : REG_EXTENDED) ==
      0) {
    if (regexec(&regex, line, 0, NULL, 0) == options->v_flag) {
      options->countMatches += 1;
      if ((options->c_flag || options->l_flag) == 0) {
        if (options->countFiles > 1 && !(options->h_flag)) printf("%s:", path);
        if (options->n_flag) printf("%i:", options->numberLine);
        if (!options->o_flag)
          printf("%s\n", line);
        else
          outOptionsO(regex, line, *options);
      }
    }
    regfree(&regex);
  }
}

void outOptionsO(regex_t regex, char *line, GrepOptions options) {
  while (regexec(&regex, line, 0, NULL, 0) == options.v_flag) {
    char *aux = (char *)calloc(strlen(line) + 1, sizeof(char));
    strcpy(aux, line);
    // int end = strlen(line);
    while (regexec(&regex, aux, 0, NULL, 0) == options.v_flag) {
      // end--;
      aux[strlen(aux) - 1] = 0;
    }
    aux[strlen(aux)] = line[strlen(aux)];
    int start = 0;
    while (regexec(&regex, aux, 0, NULL, 0) == options.v_flag &&
           strlen(aux) > 0) {
      start++;
      int j = 0;
      while (aux[j] != 0) {
        aux[j] = aux[j + 1];
        j++;
      }
    }
    start--;
    int i = strlen(aux);
    while (i != 0) {
      aux[i] = aux[i - 1];
      i--;
    }
    aux[0] = line[start];
    printf("%s\n", aux);
    free(aux);
    i = start + 1;
    while (line[i] != 0) {
      line[i - start - 1] = line[i];
      i++;
    }
    line[i - start - 1] = 0;
  }
}

void mainOptions(GrepOptions *options, int argc) {
  if (options->o_flag &&
      (options->l_flag || options->v_flag || options->c_flag))
    options->o_flag = 0;
  options->countFiles = argc - optind;
}

int setupPattern(char **str, char *optarg) {
  *str = calloc(strlen(optarg) + 1, sizeof(char));
  if (*str) strcpy(*str, optarg);
  return str ? 1 : 0;
}

void addPattern(char **str, char *optarg) {
  *str = reallocString(*str, strlen(*str) + strlen(optarg) + 2);
  if (*str) strcat(strcat(*str, "|"), optarg);
}

void *reallocString(void *str, int size) {
  char *aux = realloc(str, size);
  return aux;
}
