#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct arguments {
  bool e;
  int i;
  bool v;
  bool c;
  bool l;
  bool n;
  bool h;
  bool s;
  bool f;
  bool o;

  char pattern[4096];
  int len_pattern;
} arguments;

void add_pattern(arguments* arg, char* pattern);
size_t getline(char**, size_t*, FILE*);

void add_file(arguments* f, char* filepath) {
  FILE* file = fopen(filepath, "r");
  if (file == NULL) {
    if (!f->s) perror(filepath);
    exit(1);
  }

  char* line = NULL;
  size_t memline = 0;
  int read = getline(&line, &memline, file);

  while (read != -1) {
    if (line[read - 1] == '\n') line[read - 1] = '\0';
    add_pattern(f, line);
    read = getline(&line, &memline, file);
  }
  free(line);
  fclose(file);
}

void add_pattern(arguments* arg, char* pattern) {
  if (arg->len_pattern != 0) {
    strcat(arg->pattern + arg->len_pattern, "|");
    arg->len_pattern += 1;
  }

  arg->len_pattern += sprintf(arg->pattern + arg->len_pattern, "(%s)", pattern);
}

arguments arguments_parser(int argc, char* argv[]) {
  arguments arg = {0};
  int currentFlag;

  while ((currentFlag = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (currentFlag) {
      case 'e':
        arg.e = true;
        add_pattern(&arg, optarg);

        break;
      case 'i':
        arg.i = REG_ICASE;

        break;
      case 'v':
        arg.v = true;

        break;
      case 'c':
        arg.c = true;

        break;
      case 'l':
        arg.l = true;

        break;
      case 'n':
        arg.n = true;

        break;
      case 'h':
        arg.h = true;

        break;
      case 's':
        arg.s = true;

        break;
      case 'f':
        arg.f = true;
        add_file(&arg, optarg);

        break;
      case 'o':
        arg.o = true;
        break;
    }
  }

  if (arg.len_pattern == 0) {
    add_pattern(&arg, argv[optind]);
    optind += 1;
  }

  if (argc - optind == 1) arg.h = 1;

  return arg;
};

void output_line(char* line, int n) {
  for (int i = 0; i < n; i++) {
    putchar(line[i]);
  }
  if (line[n - 1] != '\n') putchar('\n');
}

void print_match(regex_t* re, char* line) {
  regmatch_t math;
  int offset = 0;

  while (1) {
    int result = regexec(re, line + offset, 1, &math, 0);

    if (result != 0) break;

    for (int i = math.rm_so; i < math.rm_eo; i++) {
      putchar(line[i]);
    }
    putchar('\n');
    offset += math.rm_eo;
  }
}

void processFile(arguments f, char* path, regex_t* reg) {
  FILE* file = fopen(path, "r");
  if (file == NULL) {
    if (!f.s) perror(path);
    exit(1);
  }

  int line_count = 1;
  int c_count = 0;
  char* line = NULL;
  size_t memline = 0;
  int read = getline(&line, &memline, file);

  while (read != -1) {
    int result = regexec(reg, line, 0, NULL, 0);

    if ((result == 0 && !f.v) || (result != 0 && f.v)) {
      if (!f.c && !f.l) {
        if (!f.h) printf("%s:", path);

        if (f.n) printf("%d:", line_count);

        if (f.o)
          print_match(reg, line);

        else
          output_line(line, read);
      }
      c_count += 1;
    }

    read = getline(&line, &memline, file);

    line_count += 1;
  }

  free(line);

  if (f.c && !f.l) {
    if (!f.h) printf("%s:", path);
    printf("%d\n", c_count);
  }

  if (f.l && c_count > 0) printf("%s\n", path);
  fclose(file);
}

void output(arguments f, int argc, char** argv) {
  regex_t re;

  int error = regcomp(&re, f.pattern, REG_EXTENDED | f.i);

  if (error) perror("ERROR");

  for (int i = optind; i < argc; i++) {
    processFile(f, argv[i], &re);
  }

  regfree(&re);
}

int main(int argc, char* argv[]) {
  arguments f = arguments_parser(argc, argv);
  output(f, argc, argv);

  return 0;
}
