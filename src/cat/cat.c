#include "cat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void process_flag_option(char option, Flags *flags, int *status);
static void process_input_files(char *argv[], int start, int argc, Flags *flags,
                                int *line_num);

int main(int argc, char *argv[]) {
  Flags flags = {0};
  int status = 0;
  int i = 1;
  int line_num = 1;

  while (i < argc && argv[i][0] == '-' && strcmp(argv[i], "--") != 0 &&
         status == 0) {
    for (int j = 1; argv[i][j] && status == 0; ++j) {
      process_flag_option(argv[i][j], &flags, &status);
    }
    if (status == 0) ++i;
  }
  if (i < argc && strcmp(argv[i], "--") == 0) ++i;

  if (i == argc && status == 0) {
    print_file(stdin, &flags, &line_num);
  } else if (status == 0) {
    process_input_files(argv, i, argc, &flags, &line_num);
  }

  return status;
}

static void process_flag_option(char option, Flags *flags, int *status) {
  switch (option) {
    case 'b':
      flags->b = 1;
      break;
    case 'e':
      flags->e = 1;
      flags->v = 1;
      break;
    case 'E':
      flags->e = 1;
      break;
    case 'n':
      flags->n = 1;
      break;
    case 's':
      flags->s = 1;
      break;
    case 't':
      flags->t = 1;
      flags->v = 1;
      break;
    case 'T':
      flags->t = 1;
      break;
    case 'v':
      flags->v = 1;
      break;
    default:
      fprintf(stderr, "cat: invalid option -- '%c'\n", option);
      *status = 1;
  }
}

static void process_input_files(char *argv[], int start, int argc, Flags *flags,
                                int *line_num) {
  for (int k = start; k < argc; ++k) {
    FILE *fp = fopen(argv[k], "r");
    if (fp) {
      print_file(fp, flags, line_num);
      fclose(fp);
    } else {
      fprintf(stderr, "cat: %s: No such file or directory\n", argv[k]);
    }
  }
}

void print_file(FILE *fp, Flags *flags, int *line_num) {
  char line[MAX_LINE_LEN];
  int empty_count = 0;
  while (fgets(line, MAX_LINE_LEN, fp)) {
    process_line(line, flags, line_num, &empty_count);
  }
}

void process_line(char *line, Flags *flags, int *line_num, int *empty_count) {
  int is_empty = (strcmp(line, "\n") == 0);
  int should_process = 1;

  if (flags->s && is_empty) {
    if (*empty_count >= 1) {
      should_process = 0;
    } else {
      (*empty_count)++;
    }
  } else {
    *empty_count = 0;
  }

  if (should_process) {
    if (flags->b && !is_empty) {
      printf("%6d\t", (*line_num)++);
    } else if (!flags->b && flags->n) {
      printf("%6d\t", (*line_num)++);
    }
    for (int i = 0; line[i]; ++i) {
      char ch = line[i];
      if (ch == '\t' && flags->t) {
        printf("^I");
      } else if (ch == '\n') {
        if (flags->e) putchar('$');
        putchar('\n');
      } else {
        process_char(ch, flags);
      }
    }
  }
}

void process_char(char ch, Flags *flags) {
  if (flags->v) {
    if (ch < 32 && ch != '\n' && ch != '\t') {
      printf("^%c", ch + 64);
    } else if (ch == 127) {
      printf("^?");
    } else {
      putchar(ch);
    }
  } else {
    putchar(ch);
  }
}