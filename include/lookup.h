#ifndef LOOKUP_H
#define LOOKUP_H

typedef struct {
    void *dl;
    char *module;
    char *libname;
    char *errmsg;
} pkg_info;

void *lookup_function(pkg_info *info, const char *name);

#endif
