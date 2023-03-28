#ifndef DISCOVER_H
#define DISCOVER_H

void my_discovery_recursive(int is_dir_temp, int flag_d, int flag_f, char *pop, char *dir,char* rel_path);
void my_discovery_print(int is_dir, int flag_d, int flag_f, char *root, char *dir);
void my_discover(char **args, char *root, char *curdir);
#endif