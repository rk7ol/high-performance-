#include <string.h>

#include "utils.h"

int string2int(char *value)
{

    int length = strlen(value);
    int result = 0;

    if (length > 0)
    {
        result += (value[length - 1] - '0');
    }

    for (int i = 1; i < length; i++)
    {
        result += (10 * i) * (value[length - 1 - i] - '0');
    }

    return result;
}

char *int2string(int value)
{

    //process number, int to string
    int count = 0;
    int process_number_temp = value;

    while (process_number_temp != 0)
    {
        count++;
        process_number_temp = process_number_temp / 10;
    }

    char *buf = malloc(sizeof(char) * (count + 1));

    for (size_t i = 0; i < count; i++)
    {
        buf[count - i - 1] = (value % 10) + '0';
        value = value / 10;
    }

    buf[count] = '\0';

    return buf;
}