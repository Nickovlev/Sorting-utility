#include "file_reader.h"
#include "sortings.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc != 5) {
    fprintf(stderr,
            "Error. Try: <lines number> <file> <algo name> <comparator>\n");
    return -1;
  }

  if (argv[1][0] == '-') {
    fprintf(stderr, "Error: lines amount can't be negative number\n");
    return -1;
  }

  char *endptr;
  errno = 0;
  size_t num_lines = strtoul(argv[1], &endptr, 10);
  if (errno != 0 || *endptr != '\0' || num_lines == 0) {
    fprintf(stderr, "Error: incorrect amount of lines: %s\n", argv[1]);
    return -1;
  }

  int (*comparator)(const char *, const char *);
  if (strcmp(argv[4], "asc") == 0) {
    comparator = asc;
  } else if (strcmp(argv[4], "des") == 0) {
    comparator = des;
  } else {
    fprintf(stderr, "Error. Unknown comparator: %s. Use asc or des\n", argv[4]);
    return -1;
  }

  char **buffer = read_lines(argv[2], num_lines);

  if (buffer == NULL) {
    return -1;
  }

  error = 0;

  if (strcmp(argv[3], "bubble") == 0) {
    sort_bubble(buffer, num_lines, comparator);
  } else if (strcmp(argv[3], "insertion") == 0) {
    sort_insertion(buffer, num_lines, comparator);
  } else if (strcmp(argv[3], "merge") == 0) {
    sort_merge(buffer, num_lines, comparator);
  } else if (strcmp(argv[3], "quick") == 0) {
    sort_quick(buffer, num_lines, comparator);
  } else if (strcmp(argv[3], "radix") == 0) {
    sort_radix(buffer, num_lines, comparator);
  } else {
    fprintf(stderr,
            "Error. Unknown sorting: %s. "
            "Try: bubble, insertion, merge, quick, radix\n",
            argv[3]);
    return -1;
  }

  if (error != 0) {
    for (size_t i = 0; i < num_lines; i++) {
      if (buffer[i] != NULL) {
        free(buffer[i]);
      }
    }
    free(buffer);
    return -1;
  }

  for (size_t i = 0; i < num_lines; i++) {
    if (buffer[i] != NULL) {
      printf("%s\n", buffer[i]);
      free(buffer[i]);
    }
  }

  free(buffer);

  return 0;
}