#ifndef GREP_H
#define GREP_H

#define MAX_PATTERNS 128

typedef struct {
  int e, i, v, c, l, n, h, s, f, o;
  char *patterns[MAX_PATTERNS];
  int pattern_count;
} Flags;

int match_pattern(const char *line, Flags *flags);
void grep_file(const char *filename, Flags *flags, int multiple_files);
void free_patterns(Flags *flags);

#endif