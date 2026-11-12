
#ifndef INPUT_VALIDATION_H
#define INPUT_VALIDATION_H

int read_integer(const char *prompt, int *value);
int read_double(const char *prompt, double *value);
int read_string(const char *prompt, char *buffer, int size);

#endif
