#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include "lookup.h"

void *lookup_function(pkg_info *info, const char *name)
{
    /* Try plain name first */
    void *p = dlsym(info->dl, name);
    if (p != NULL) return p;

    /* Try module_name style: <module>_<name> */
    char fullname[256];
    snprintf(fullname, sizeof(fullname), "%s_%s", info->module, name);
    p = dlsym(info->dl, fullname);
    if (p != NULL) return p;

    snprintf(info->errmsg, 256,
             "Symbol '%s' or '%s' not found in %s",
             name, fullname, info->libname);
    return NULL;
}
