#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "cmdiface.h"
#include "lookup.h"

CmdPlugin *cmdiface_bind(char *module, char *errmsg)
{
    char libname[128];
    snprintf(libname, sizeof(libname), "lib%s.so", module);

    void *dl = dlopen(libname, RTLD_NOW);
    if (!dl) {
        snprintf(errmsg, 256, "dlopen failed for %s", libname);
        return NULL;
    }

    pkg_info info;
    info.dl = dl;
    info.module = module;
    info.libname = libname;
    info.errmsg = errmsg;

    /* Required metadata: plugin_name, plugin_version, iface_version */
    char **name_ptr  = (char **)lookup_function(&info, "plugin_name");
    char **ver_ptr   = (char **)lookup_function(&info, "plugin_version");
    char **iface_ptr = (char **)lookup_function(&info, "iface_version");
    if (!name_ptr || !ver_ptr || !iface_ptr) {
        snprintf(errmsg, 256, "Metadata (name/version/interface) missing in %s", libname);
        return NULL;
    }

    /* Require interface version 1.0 for this project */
    if (strcmp(*iface_ptr, "1.0") != 0) {
        snprintf(errmsg, 256,
                 "Incompatible interface version: %s (expected 1.0)",
                 *iface_ptr);
        return NULL;
    }

    /* Command table symbols: num_cmds + cmds */
    size_t *num_cmds = (size_t *)lookup_function(&info, "num_cmds");
    if (!num_cmds) return NULL;

    CmdIface *cmds = (CmdIface *)lookup_function(&info, "cmds");
    if (!cmds) return NULL;

    CmdPlugin *plugin = malloc(sizeof(*plugin));
    if (!plugin) {
        snprintf(errmsg, 256, "cmdiface_bind: malloc failed");
        return NULL;
    }

    plugin->plugin_name    = *name_ptr;
    plugin->plugin_version = *ver_ptr;
    plugin->iface_version  = *iface_ptr;
    plugin->num_cmds       = *num_cmds;
    plugin->cmds           = cmds;

    return plugin;
}
