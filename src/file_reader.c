#include "file_reader.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef SWITCH_TO_MMAP
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#ifndef SWITCH_TO_MMAP

char **read_lines(const char *filename, int num_lines) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error: no file\n");
    return NULL;
  }

  int actual_lines = 0;
  char *line = NULL;
  size_t len = 0;

  while (getline(&line, &len, fp) != -1) {
    actual_lines++;
  }
  free(line);

  if (actual_lines < num_lines) {
    fprintf(stderr, "File has only %d lines, but you entered %d\n",
            actual_lines, num_lines);
    fclose(fp);
    return NULL;
  }

  rewind(fp);

  char **lines = malloc(sizeof(char *) * num_lines);
  if (lines == NULL) {
    fprintf(stderr, "Error while memory allocation\n");
    fclose(fp);
    return NULL;
  }

  line = NULL;
  len = 0;

  for (int i = 0; i < num_lines; i++) {
    if (getline(&line, &len, fp) == -1) {
      fprintf(stderr, "Error while reading line %d\n", i);
      for (int j = 0; j < i; j++) {
        free(lines[j]);
      }
      free(lines);
      fclose(fp);
      free(line);
      return NULL;
    }

    line[strcspn(line, "\n")] = '\0';

    lines[i] = strdup(line);
    if (lines[i] == NULL) {
      fprintf(stderr, "Error while memory allocation\n");
      for (int j = 0; j < i; j++) {
        free(lines[j]);
      }
      free(lines);
      fclose(fp);
      free(line);
      return NULL;
    }
  }

  free(line);
  fclose(fp);

  return lines;
}

#else

char **read_lines(const char *filename, int num_lines) {
  int fd = open(filename, O_RDONLY);

  if (fd == -1) {
    fprintf(stderr, "Error while opening file for reading\n");
    return NULL;
  }

  struct stat fileInfo;

  if (fstat(fd, &fileInfo) == -1) {
    fprintf(stderr, "Error while getting the file size\n");
    close(fd);
    return NULL;
  }

  if (fileInfo.st_size == 0) {
    fprintf(stderr, "Error: File is empty, nothing to do\n");
    close(fd);
    return NULL;
  }

  char *map = mmap(NULL, fileInfo.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

  if (map == MAP_FAILED) {
    fprintf(stderr, "Error while memory mapping\n");
    close(fd);
    return NULL;
  }

  int actual_lines = 0;
  for (off_t i = 0; i < fileInfo.st_size; i++) {
    if (map[i] == '\n') {
      actual_lines++;
    }
  }

  if (fileInfo.st_size > 0 && map[fileInfo.st_size - 1] != '\n') {
    actual_lines++;
  }

  if (num_lines > actual_lines) {
    fprintf(stderr, "File has only %d lines, but you entered %d\n",
            actual_lines, num_lines);
    munmap(map, fileInfo.st_size);
    close(fd);
    return NULL;
  }

  char **lines = malloc(sizeof(char *) * num_lines);

  if (lines == NULL) {
    fprintf(stderr, "Error while memory allocation\n");
    munmap(map, fileInfo.st_size);
    close(fd);
    return NULL;
  }

  char *ptr = map;
  for (int i = 0; i < num_lines; i++) {
    char *start = ptr;

    while (*ptr != '\n' && *ptr != '\0')
      ptr++;

    size_t len = ptr - start;
    if (len == 0) {
      lines[i] = strdup("");

      if (lines[i] == NULL) {
        fprintf(stderr, "Error while memory allocation\n");
        munmap(map, fileInfo.st_size);
        close(fd);
        return NULL;
      }
    } else {
      lines[i] = strndup(start, len);

      if (lines[i] == NULL) {
        fprintf(stderr, "Error while memory allocation\n");
        munmap(map, fileInfo.st_size);
        close(fd);
        return NULL;
      }
    }

    ptr++;
  }

  munmap(map, fileInfo.st_size);
  close(fd);
  return lines;
}
#endif
