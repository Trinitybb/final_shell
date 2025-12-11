CC      = gcc
CFLAGS  = -Wall -Wextra -g -fPIC -Iinclude
LDFLAGS = -ldl

SRC_DIR     = src
PLUGIN_DIR  = plugins
OBJ_DIR     = build

SHELL_OBJS = $(OBJ_DIR)/shellmain.o $(OBJ_DIR)/builtins.o \
             $(OBJ_DIR)/util.o $(OBJ_DIR)/cmdiface.o $(OBJ_DIR)/lookup.o

STATIC_OBJS = $(OBJ_DIR)/builtins.o $(OBJ_DIR)/util.o

PLUGINS = libtext.so libmath.so

all: myshell $(PLUGINS)

# ---- main shell program ----
myshell: libshellcore.a $(SHELL_OBJS)
	$(CC) -o $@ $(SHELL_OBJS) -L. -lshellcore $(LDFLAGS)

# ---- static library ----
libshellcore.a: $(STATIC_OBJS)
	ar rcs $@ $^

# ---- plugin shared libs ----
libtext.so: $(OBJ_DIR)/text.o
	$(CC) -shared -o $@ $^

libmath.so: $(OBJ_DIR)/math.o
	$(CC) -shared -o $@ $^

# ---- generic object build rules ----
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(PLUGIN_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) myshell libshellcore.a $(PLUGINS)

.PHONY: all clean
