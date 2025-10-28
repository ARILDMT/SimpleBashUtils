#ifndef CAT_H
#define CAT_H

#include <stdio.h>

#define MAX_LINE_LEN 4096

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} Flags;

void print_file(FILE *fp, Flags *flags, int *line_num);
void process_line(char *line, Flags *flags, int *line_num, int *empty_count);
void process_char(char ch, Flags *flags);

#endif