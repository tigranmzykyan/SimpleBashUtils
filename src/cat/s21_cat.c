#include "s21_cat.h"

int main(int argc, char *argv[]) {
  CatOptions options = parseOptions(argc, argv);

  if (optind == argc) {
    processFile(NULL, options);
  } else {
    while (optind < argc) {
      if (processFile(argv[optind], options) != 0) {
        return 1;
      }
      optind++;
    }
  }

  return 0;
}

CatOptions parseOptions(int argc, char *argv[]) {
  CatOptions options = {0};

  struct option long_options[] = {{"number-nonblank", no_argument, 0, 'b'},
                                  {"number", no_argument, 0, 'n'},
                                  {"squeeze-blank", no_argument, 0, 's'},
                                  {0, 0, 0, 0}};

  int option;
  while ((option = getopt_long(argc, argv, "bEnsTvet", long_options, NULL)) !=
         -1) {
    switch (option) {
      case 'b':
        options.b_flag = 1;
        break;
      case 'n':
        options.n_flag = 1;
        break;
      case 's':
        options.s_flag = 1;
        break;
      case 'e':
        options.e_flag = 1;
        options.v_flag = 1;
        break;
      case 't':
        options.t_flag = 1;
        options.v_flag = 1;
        break;
      case 'v':
        options.v_flag = 1;
        break;
      case 'T':
        options.t_flag = 1;
        break;
      case 'E':
        options.e_flag = 1;
        break;
      default:
        break;
    }
  }
  return options;
}

int processFile(const char *filename, CatOptions options) {
  FILE *file;
  if (filename == NULL) {
    file = stdin;
  } else {
    file = fopen(filename, "r");
    if (file == NULL) {
      printf("Error opening file: %s\n", filename);
      return 1;
    }
  }

  options.countLines = 1;
  options.countEmptyLines = 0;

  int last_char = '\n';

  while (1) {
    int current_char = fgetc(file);

    if (current_char == EOF) {
      break;
    }

    if (options.s_flag && current_char == '\n' && last_char == '\n') {
      options.countEmptyLines++;
      if (options.countEmptyLines > 1) {
        continue;
      }
    } else {
      options.countEmptyLines = 0;
    }

    if (last_char == '\n' &&
        ((options.b_flag && current_char != '\n') || options.n_flag)) {
      printf("%6d\t", options.countLines++);
    }

    if (options.t_flag && current_char == '\t') {
      printf("^");
      current_char = 'I';
    }

    if (options.e_flag && current_char == '\n') {
      printf("$");
    }

    if (options.v_flag) {
      if ((current_char >= 0 && current_char < 9) ||
          (current_char > 10 && current_char < 32)) {
        printf("^");
        current_char += 64;
      } else if (current_char == 127) {
        printf("^");
        current_char = '?';
      }
    }
    printf("%c", current_char);
    last_char = current_char;
  }

  if (file != stdin) {
    fclose(file);
  }
  return 0;
}
