#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>

/* safe malloc that exits on error (for static lib demo) */
void *xmalloc(size_t sz);

/* simple tokenizer: splits line into argv[], returns argc */
int split_line(char *line, char **argv, int max_args);

#endif
