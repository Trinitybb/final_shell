#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

void *xmalloc(size_t sz)
{
    void *p = malloc(sz);
    if (!p) {
        fprintf(stderr, "xmalloc: out of memory (size=%zu)\n", sz);
        exit(1);
    }
    return p;
}

int split_line(char *line, char **argv, int max_args)
{
    int argc = 0;
    char *p = line;

    while (*p && argc < max_args - 1) {
        // skip leading whitespace
        while (isspace((unsigned char)*p)) p++;
        if (!*p) break;

        if (*p == '"') {
            // quoted argument
            p++; // skip opening quote
            char *start = p;
            char *out = p;

            while (*p && *p != '"') {
                *out++ = *p++;
            }
            *out = '\0';
            if (*p == '"') p++;  // skip closing quote
            argv[argc++] = start;
        } else {
            // normal word
            argv[argc++] = p;
            while (*p && !isspace((unsigned char)*p)) p++;
            if (*p) {
                *p = '\0';
                p++;
            }
        }
    }

    argv[argc] = NULL;
    return argc;
}
