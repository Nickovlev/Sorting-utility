#pragma once

#include <stdio.h>

void sort_bubble(char **arr, size_t size,
                 int (*comparator)(const char *, const char *));
void sort_insertion(char **arr, size_t size,
                    int (*comparator)(const char *, const char *));
void sort_merge(char **arr, size_t size,
                int (*comparator)(const char *, const char *));
void sort_quick(char **arr, size_t size,
                int (*comparator)(const char *, const char *));
void sort_radix(char **arr, size_t size,
                int (*comparator)(const char *, const char *));

// Comparators
int asc(const char *a, const char *b);
int des(const char *a, const char *b);

// Error flag for sorting functions
extern int error;