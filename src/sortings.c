#include "sortings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RADIX_BUCKETS 256

int error = 0;

void sort_bubble(char **arr, size_t size,
                 int (*comparator)(const char *, const char *)) {
  if (size < 2) {
    return;
  }

  for (size_t i = 0; i < size; i++) {
    for (size_t j = 0; j < size - i - 1; j++) {
      if (comparator(arr[j], arr[j + 1]) > 0) {
        char *tmp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = tmp;
      }
    }
  }
}

void sort_insertion(char **arr, size_t size,
                    int (*comparator)(const char *, const char *)) {
  if (size < 2) {
    return;
  }

  for (size_t i = 1; i < size; i++) {
    char *tmp = arr[i];
    int j = i - 1;
    while (j >= 0 && comparator(arr[j], tmp) > 0) {
      arr[j + 1] = arr[j];
      j--;
    }
    arr[j + 1] = tmp;
  }
}

void sort_merge(char **arr, size_t size,
                int (*comparator)(const char *, const char *)) {
  if (size < 2) {
    return;
  }

  // Recursive split on left and right parts
  size_t mid = size / 2;
  sort_merge(arr, mid, comparator);
  sort_merge(arr + mid, size - mid, comparator);

  // Merge two sorted halves [0..mid-1] and [mid..size-1]
  char **tmp = (char **)malloc(size * sizeof(char *));
  if (tmp == NULL) {
    fprintf(stderr, "Error while memory allocation\n");
    error = 1;
    return;
  }

  // Indexes for left part, right part, and temporary buffer
  size_t leftPos = 0;
  size_t rightPos = mid;
  size_t outPos = 0;

  // Merge elements until one of the halves runs out
  while (leftPos < mid && rightPos < size) {
    if (comparator(arr[leftPos], arr[rightPos]) <= 0) {
      tmp[outPos++] = arr[leftPos++];
    } else {
      tmp[outPos++] = arr[rightPos++];
    }
  }

  // Append remaining elements from left (if any)
  while (leftPos < mid) {
    tmp[outPos++] = arr[leftPos++];
  }

  // Append remaining elements from right (if any)
  while (rightPos < size) {
    tmp[outPos++] = arr[rightPos++];
  }

  // Copy merged results from temporary array back to original
  for (size_t i = 0; i < size; i++) {
    arr[i] = tmp[i];
  }

  free(tmp);
}

void sort_quick(char **arr, size_t size,
                int (*comparator)(const char *, const char *)) {
  if (size < 2) {
    return;
  }

  // Using last element as pivot
  char *pivot = arr[size - 1];

  // Boundary index for elements <= pivot
  size_t i = 0;

  // Partition array around pivot
  for (size_t j = 0; j < size - 1; j++) {
    if (comparator(arr[j], pivot) <= 0) {
      char *tmp = arr[i];
      arr[i] = arr[j];
      arr[j] = tmp;
      i++;
    }
  }

  // Place pivot at its correct position
  char *tmp = arr[i];
  arr[i] = arr[size - 1];
  arr[size - 1] = tmp;

  // Recursively sort left and right
  if (i > 0) {
    sort_quick(arr, i, comparator);
  }

  sort_quick(arr + i + 1, size - (i + 1), comparator);
}

static unsigned char get_char(const char *str, size_t pos) {
  if (!str[pos]) {
    return 0; // The "\0" symbol
  }
  return (unsigned char)str[pos];
}

// Recursive MSD radix sort function
static void msd_sort(char **arr, size_t left, size_t right, size_t depth,
                     int direction) {
  if (right - left < 2) {
    return;
  }

  // Count occurrences of each character [0..255] at the given depth
  int count[RADIX_BUCKETS] = {0};
  for (size_t i = left; i < right; i++) {
    unsigned char c = get_char(arr[i], depth);
    count[c]++;
  }

  // If all strings in a given context have the same character at position
  // depth, stop recursion
  if (count[get_char(arr[left], depth)] == (int)(right - left)) {
    return;
  }

  // Turn count into prefix sums. boundary[i] tells how many elements <= i
  int boundary[RADIX_BUCKETS];
  if (direction == 1) { // asc
    boundary[0] = count[0];
    for (int i = 1; i < RADIX_BUCKETS; i++) {
      boundary[i] = boundary[i - 1] + count[i];
    }
  } else { // des
    boundary[RADIX_BUCKETS - 1] = count[RADIX_BUCKETS - 1];
    for (int i = RADIX_BUCKETS - 2; i >= 0; i--) {
      boundary[i] = boundary[i + 1] + count[i];
    }
  }

  // Allocate temporary array for rearranging elements
  size_t sizeRange = right - left;
  char **tmp = (char **)malloc(sizeRange * sizeof(char *));
  if (tmp == NULL) {
    fprintf(stderr, "Error while memory allocation\n");
    error = 1;
    return;
  }

  // Place elements into tmp
  int bucketPos[RADIX_BUCKETS];
  if (direction == 1) { // asc
    bucketPos[0] = boundary[0] - 1;
    for (int i = 1; i < RADIX_BUCKETS; i++) {
      bucketPos[i] = boundary[i] - 1;
    }
  } else { // des
    bucketPos[RADIX_BUCKETS - 1] = 0;
    for (int i = RADIX_BUCKETS - 2; i >= 0; i--) {
      bucketPos[i] = bucketPos[i + 1] + count[i + 1];
    }
  }

  for (size_t i = right; i > left;) {
    i--;
    unsigned char c = get_char(arr[i], depth);
    size_t pos = (direction == 1) ? bucketPos[c]-- : bucketPos[c]++;
    tmp[pos] = arr[i];
  }

  // Copy from tmp to arr
  for (size_t i = 0; i < sizeRange; i++) {
    arr[left + i] = tmp[i];
  }
  free(tmp);

  // Recursively sort each segment by the next character depth
  if (direction == 1) {
    int prev = 0;
    for (int c = 0; c < RADIX_BUCKETS; c++) {
      int curr = boundary[c];
      if (curr > prev) {
        msd_sort(arr, left + prev, left + curr, depth + 1, direction);
      }
      prev = curr;
    }
  } else {
    int prev = 0;
    for (int c = RADIX_BUCKETS - 1; c >= 0; c--) {
      int curr = boundary[c];
      if (curr > prev) {
        msd_sort(arr, left + prev, left + curr, depth + 1, direction);
      }
      prev = curr;
    }
  }
}

void sort_radix(char **arr, size_t size,
                int (*comparator)(const char *, const char *)) {
  if (size < 2) {
    return;
  }

  int direction = (comparator == des) ? -1 : 1;

  msd_sort(arr, 0, size, 0, direction);
}

// Comparators

int asc(const char *a, const char *b) { return strcmp(a, b); }

int des(const char *a, const char *b) { return strcmp(b, a); }