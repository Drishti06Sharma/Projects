#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include "input_validation.h"

/* ---------- CLEAR INPUT BUFFER ---------- */

static void clear_input_buffer(void)
{
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF)
    {
    }
}

/* ---------- READ INTEGER ---------- */

int read_integer(const char *prompt, int *value)
{
    char buffer[100];
    char *end;
    long result;

    if (value == NULL)
    {
        return 0;
    }

    printf("%s", prompt);

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        return 0;
    }

    /* Clear remaining characters if input is too long */
    if (strchr(buffer, '\n') == NULL)
    {
        clear_input_buffer();
    }

    result = strtol(buffer, &end, 10);

    /* Check invalid input */
    if (end == buffer)
    {
        return 0;
    }

    /* Check extra characters */
    while (*end == ' ' || *end == '\t' ||
           *end == '\n' || *end == '\r')
    {
        end++;
    }

    if (*end != '\0')
    {
        return 0;
    }

    /* Check integer range */
    if (result < INT_MIN || result > INT_MAX)
    {
        return 0;
    }

    *value = (int)result;

    return 1;
}

/* ---------- READ DOUBLE ---------- */

int read_double(const char *prompt, double *value)
{
    char buffer[100];
    char *end;
    double result;

    if (value == NULL)
    {
        return 0;
    }

    printf("%s", prompt);

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        return 0;
    }

    /* Clear remaining characters if input is too long */
    if (strchr(buffer, '\n') == NULL)
    {
        clear_input_buffer();
    }

    result = strtod(buffer, &end);

    /* Check invalid input */
    if (end == buffer)
    {
        return 0;
    }

    /* Check extra characters */
    while (*end == ' ' || *end == '\t' ||
           *end == '\n' || *end == '\r')
    {
        end++;
    }

    if (*end != '\0')
    {
        return 0;
    }

    /* Reject NaN and Infinity */
    if (!isfinite(result))
    {
        return 0;
    }

    *value = result;

    return 1;
}

/* ---------- READ STRING ---------- */

int read_string(const char *prompt, char *buffer, int size)
{
    size_t length;

    if (buffer == NULL || size <= 1)
    {
        return 0;
    }

    printf("%s", prompt);

    if (fgets(buffer, size, stdin) == NULL)
    {
        return 0;
    }

    length = strlen(buffer);

    /* Handle newline */
    if (length > 0 && buffer[length - 1] == '\n')
    {
        buffer[length - 1] = '\0';
    }
    else
    {
        /* Clear remaining input if string is too long */
        clear_input_buffer();
    }

    /* Remove trailing carriage return */
    length = strlen(buffer);

    if (length > 0 && buffer[length - 1] == '\r')
    {
        buffer[length - 1] = '\0';
    }

    /* Reject empty string */
    return buffer[0] != '\0';
}