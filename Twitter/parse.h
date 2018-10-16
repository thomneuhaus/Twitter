#ifndef PARSELIB_H
#define PARSELIB_H

int parseCmdLine(int argc, char *argv[], int(*parseCallback)(char *key, char *value, void *UserData), void *UserData);

int str_cmp(const char * str1, const char * str2);

#endif /* PARSELIB_H */

