#ifndef CMDIFACE_H
#define CMDIFACE_H

#include <stddef.h>

/* F-functions (formal interface) for a command */

typedef int (*cmd_run_f)(int argc, char **argv);
typedef const char *(*cmd_name_f)(void);
typedef const char *(*cmd_help_f)(void);

typedef struct {
    cmd_run_f  run;
    cmd_name_f name;
    cmd_help_f help;
} CmdIface;

/* Plugin-level interface */

typedef struct {
    const char *plugin_name;
    const char *plugin_version;
    const char *iface_version;  /* expected to be "1.0" in this project */

    size_t      num_cmds;
    CmdIface   *cmds;
} CmdPlugin;

/*
 * CmdPlugin *cmdiface_bind(char *module, char *errmsg);
 *   Dynamically binds lib<module>.so to this interface using dlopen/dlsym.
 *   On success: returns allocated CmdPlugin struct.
 *   On failure: returns NULL and writes an error string into errmsg.
 */
CmdPlugin *cmdiface_bind(char *module, char *errmsg);

#endif
