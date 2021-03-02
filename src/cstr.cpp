#include "cstr.h"

char uintTo_StringOutput[128];
const char *to_string(uint64_t value)
{
    uint8_t size = 0;
    uint64_t sizeTest = value;
    while (sizeTest / 10 > 0)
    {
        sizeTest /= 10;
        size++;
    }

    uint8_t index = 0;
    while (value / 10 > 0)
    {
        uint8_t remainder = value % 10;
        value /= 10;
        uintTo_StringOutput[size - index] = remainder + '0';
        index++;
    }
    uint8_t remainder = value % 10;
    uintTo_StringOutput[size - index] = remainder + '0';
    uintTo_StringOutput[size + 1] = 0;
    return uintTo_StringOutput;
}

char intTo_StringOutput[128];
const char *to_string(int64_t value)
{
    uint8_t isNegative = 0;

    if (value < 0)
    {
        isNegative = 1;
        value *= -1;
        intTo_StringOutput[0] = '-';
    }

    uint8_t size = 0;
    uint64_t sizeTest = value;
    while (sizeTest / 10 > 0)
    {
        sizeTest /= 10;
        size++;
    }

    uint8_t index = 0;
    while (value / 10 > 0)
    {
        uint8_t remainder = value % 10;
        value /= 10;
        intTo_StringOutput[isNegative + size - index] = remainder + '0';
        index++;
    }
    uint8_t remainder = value % 10;
    intTo_StringOutput[isNegative + size - index] = remainder + '0';
    intTo_StringOutput[isNegative + size + 1] = 0;
    return intTo_StringOutput;
}

char doubleTo_StringOutput[128];
const char *to_string(double value, uint8_t decimal_places)
{
    // KNOWN BUG: if we give more decimal places than the number has, like decimal_places=2 for 0.1, there are some magled chars;
    char *intPtr = (char *)to_string((int64_t)value);
    char *doublePtr = doubleTo_StringOutput;

    if (value < 0)
    {
        value *= -1;
    }

    while (*intPtr != 0)
    {
        *doublePtr = *intPtr;
        intPtr++;
        doublePtr++;
    }

    if (decimal_places > 0)
    {
        *doublePtr = '.';
        doublePtr++;
    }

    double decimal = value - (int)value;

    for (uint8_t i = 0; i < decimal_places; i++)
    {
        decimal *= 10;
        *doublePtr = (int)decimal + '0';
        decimal -= (int)decimal;
        doublePtr++;
    }

    *doublePtr = 0;
    return doubleTo_StringOutput;
}

const char *to_string(double value)
{
    return to_string(value, 2);
}

char hexTo_StringOutput[128];
const char *to_hstring(uint64_t value)
{
    uint64_t *valPtr = &value;
    uint8_t *ptr;
    uint8_t tmp;
    uint8_t size = 8 * 2 - 1;

    for (uint8_t i = 0; i < size; i++)
    {
        ptr = ((uint8_t *)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StringOutput[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        hexTo_StringOutput[size - (i * 2 + 0)] = tmp + (tmp > 9 ? 55 : '0');
    }

    hexTo_StringOutput[size + 1] = 0;
    return hexTo_StringOutput;
}

char hexTo_StringOutput_32[128];
const char *to_hstring(uint32_t value)
{
    uint32_t *valPtr = &value;
    uint8_t *ptr;
    uint8_t tmp;
    uint8_t size = 4 * 2 - 1;

    for (uint8_t i = 0; i < size; i++)
    {
        ptr = ((uint8_t *)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StringOutput_32[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        hexTo_StringOutput_32[size - (i * 2 + 0)] = tmp + (tmp > 9 ? 55 : '0');
    }

    hexTo_StringOutput_32[size + 1] = 0;
    return hexTo_StringOutput_32;
}

char hexTo_StringOutput_16[128];
const char *to_hstring(uint16_t value)
{
    uint16_t *valPtr = &value;
    uint8_t *ptr;
    uint8_t tmp;
    uint8_t size = 2 * 2 - 1;

    for (uint8_t i = 0; i < size; i++)
    {
        ptr = ((uint8_t *)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StringOutput_16[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        hexTo_StringOutput_16[size - (i * 2 + 0)] = tmp + (tmp > 9 ? 55 : '0');
    }

    hexTo_StringOutput_16[size + 1] = 0;
    return hexTo_StringOutput_16;
}

char hexTo_StringOutput_8[128];
const char *to_hstring(uint8_t value)
{
    uint8_t *valPtr = &value;
    uint8_t *ptr;
    uint8_t tmp;
    uint8_t size = 1 * 2 - 1;

    for (uint8_t i = 0; i < size; i++)
    {
        ptr = ((uint8_t *)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StringOutput_8[size - (i * 2 + 1)] = tmp + (tmp > 9 ? 55 : '0');
        tmp = ((*ptr & 0x0F));
        hexTo_StringOutput_8[size - (i * 2 + 0)] = tmp + (tmp > 9 ? 55 : '0');
    }

    hexTo_StringOutput_8[size + 1] = 0;
    return hexTo_StringOutput_8;
}
