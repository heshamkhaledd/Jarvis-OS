#include "common_funs.h"

/*******************************************************************************
 *                          Basic C String Functions
 ******************************************************************************/
int8_t strcmp(const uint8_t *Str_1, const uint8_t *Str_2)
{
    while (*Str_1)
    {
        if (*Str_1 != *Str_2)
            return -1;

        Str_1++;
        Str_2++;
    }
    return 0;
}
void strcpy (uint8_t *destination, const uint8_t *source)
{
    if (destination == NULL)
            return;

        while (*source != '\0')
        {
            *destination = *source;
            destination++;
            source++;
        }
        *destination = '\0';

        return;
}