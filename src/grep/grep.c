#include "grep.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 4096

int add_pattern(const char *src, Flags *f);
void load_patterns(const char *fname, Flags *f, int *err);
void process_flag_chars(const char *arg, int argc, char **argv, int *idx,
                        Flags *f, int *err);
void parse_flags(int argc, char **argv, Flags *f, int *pat, int *file,
                 int *err);
void free_patterns(Flags *f);
char *ci_find(const char *hay, const char *nee);
void print_line(const char *fn, const char *ln, int n, Flags *f, int multi);
void print_only(const char *ln, Flags *f);
void handle_match(const char *fn, const char *ln, int n, Flags *f, int multi,
                  int *cnt, int *found);

int main(int argc, char **argv) {
  Flags f = {0};
  int pat = -1, file = -1, err = 0;
  parse_flags(argc, argv, &f, &pat, &file, &err);
  if (!err && f.pattern_count == 0 && pat != -1) add_pattern(argv[pat], &f);
  if (!err && file == -1) {
    fprintf(stderr, "grep: no input files\n");
    err = 1;
  }
  if (!err) {
    int multi = (argc - file > 1);
    for (int i = file; i < argc; ++i) grep_file(argv[i], &f, multi);
  }
  free_patterns(&f);
  return err;
}

int add_pattern(const char *src, Flags *f) {
  int rc = 0;
  size_t len = strlen(src) + 1;
  char *buf = (char *)malloc(len);
  if (buf) {
    strcpy(buf, src);
    f->patterns[f->pattern_count++] = buf;
  } else
    rc = 1;
  return rc;
}

void free_patterns(Flags *f) {
  for (int i = 0; i < f->pattern_count; ++i) free(f->patterns[i]);
  f->pattern_count = 0;
}

void load_patterns(const char *fname, Flags *f, int *err) {
  FILE *fp = fopen(fname, "r");
  *err = 0;
  if (fp) {
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp)) {
      size_t len = strlen(line);
      if (len && line[len - 1] == '\n') line[len - 1] = '\0';
      add_pattern(line, f);
    }
    fclose(fp);
  } else
    *err = 1;
}

void process_flag_chars(const char *arg, int argc, char **argv, int *idx,
                        Flags *f, int *err) {
  for (int j = 1; arg[j] && !*err; ++j) {
    char c = arg[j];
    if (c == 'e') {
      f->e = 1;
      if (arg[j + 1])
        add_pattern(&arg[j + 1], f);
      else if (*idx + 1 < argc) {
        ++*idx;
        add_pattern(argv[*idx], f);
      }
    } else if (c == 'i')
      f->i = 1;
    else if (c == 'v')
      f->v = 1;
    else if (c == 'c')
      f->c = 1;
    else if (c == 'l')
      f->l = 1;
    else if (c == 'n')
      f->n = 1;
    else if (c == 'h')
      f->h = 1;
    else if (c == 's')
      f->s = 1;
    else if (c == 'f') {
      f->f = 1;
      if (*idx + 1 < argc) {
        ++*idx;
        load_patterns(argv[*idx], f, err);
      }
    } else if (c == 'o')
      f->o = 1;
    else {
      fprintf(stderr, "grep: invalid option -- '%c'\n", c);
      *err = 1;
    }
  }
}

void parse_flags(int argc, char **argv, Flags *f, int *pat, int *file,
                 int *err) {
  int i = 1;
  *err = 0;
  *pat = -1;
  *file = -1;
  while (i < argc && !*err) {
    if (argv[i][0] == '-' && argv[i][1])
      process_flag_chars(argv[i], argc, argv, &i, f, err);
    else if (*file == -1 && (f->pattern_count || *pat != -1))
      *file = i;
    else if (*pat == -1)
      *pat = i;
    ++i;
  }
}

char *ci_find(const char *hay, const char *nee) {
  size_t n = strlen(nee);
  char *res = NULL;
  if (n == 0)
    res = (char *)hay;
  else {
    for (const char *h = hay; *h && res == NULL; ++h) {
      size_t i = 0;
      while (i < n &&
             tolower((unsigned char)h[i]) == tolower((unsigned char)nee[i]))
        ++i;
      if (i == n) res = (char *)h;
    }
  }
  return res;
}

int match_pattern(const char *ln, Flags *f) {
  int ok = 0;
  for (int i = 0; i < f->pattern_count && !ok; ++i)
    if ((f->i && ci_find(ln, f->patterns[i])) ||
        (!f->i && strstr(ln, f->patterns[i])))
      ok = 1;
  return ok;
}

void print_line(const char *fn, const char *ln, int n, Flags *f, int multi) {
  if (!f->h && multi) printf("%s:", fn);
  if (f->n) printf("%d:", n);
  printf("%s", ln);
  if (ln[strlen(ln) - 1] != '\n') putchar('\n');
}

void print_only(const char *ln, Flags *f) {
  for (int i = 0; i < f->pattern_count; ++i) {
    char *p = (char *)ln;
    while (
        (p = f->i ? ci_find(p, f->patterns[i]) : strstr(p, f->patterns[i]))) {
      int len = (int)strlen(f->patterns[i]);
      printf("%.*s\n", len, p);
      p += len;
    }
  }
}

void handle_match(const char *fn, const char *ln, int n, Flags *f, int multi,
                  int *cnt, int *found) {
  ++*cnt;
  *found = 1;
  if (!f->c && !f->l) {
    if (f->o)
      print_only(ln, f);
    else
      print_line(fn, ln, n, f, multi);
  }
}

void grep_file(const char *fn, Flags *f, int multi) {
  FILE *fp = fopen(fn, "r");
  int cnt = 0, found = 0;
  if (!fp) {
    if (!f->s) fprintf(stderr, "grep: %s: No such file or directory\n", fn);
  } else {
    char line[MAX_LINE];
    int num = 0;
    while (fgets(line, sizeof(line), fp)) {
      ++num;
      int m = match_pattern(line, f);
      if (f->v) m = !m;
      if (m) handle_match(fn, line, num, f, multi, &cnt, &found);
    }
    fclose(fp);
    if (f->c) {
      if (!f->h && multi) printf("%s:", fn);
      printf("%d\n", cnt);
    }
    if (f->l && found) printf("%s\n", fn);
  }
}