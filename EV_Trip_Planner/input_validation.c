#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input_validation.h"

int read_integer(const char *prompt, int *value)
{
    char buffer[100];
    char extra;

    printf("%s", prompt);

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        return 0;

    if (sscanf(buffer, " %d %c", value, &extra) != 1)
        return 0;

    return 1;
}

int read_double(const char *prompt, double *value)
{
    char buffer[100];
    char extra;

    printf("%s", prompt);

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        return 0;

    if (sscanf(buffer, " %lf %c", value, &extra) != 1)
        return 0;

    return 1;
}

int read_string(const char *prompt, char *buffer, int size)
{
    size_t length;

    if (buffer == NULL || size <= 1)
        return 0;

    printf("%s", prompt);

    if (fgets(buffer, size, stdin) == NULL)
        return 0;

    length = strlen(buffer);

    if (length > 0 && buffer[length - 1] == '\n')
        buffer[length - 1] = '\0';
    else
        buffer[size - 1] = '\0';

    return buffer[0] != '\0';
}