#ifndef HISTORY_H
#define HISTORY_H
int readhistory(char *root, char **commands);
void my_history(char *root);
void addhistory(char *root, char *input);
#endif