/*
 * MemScanner.c
 *
 *  Created on: Apr 05, 2025
 *  Author    : AbdullahDarwish
 *
 *  Description: Memory inspection and manipulation module implementation.
 *               Inspired by embedded debugger tools (OpenOCD, pyOCD).
 */

/******************************************************************************/
/*                              Includes                                      */
/******************************************************************************/
#include "MemScanner.h"
#include "STD_TYPES.h"
#include <stdio.h>

/******************************************************************************/
/*                       Function Implementations                             */
/******************************************************************************/

/*
 * MemScanner_ReadByte
 * Read a single byte from base + offset.
 */
u8 MemScanner_ReadByte(ptr_t base, u32 offset)
{
    u8 *bytePtr = (u8 *)base;
    return bytePtr[offset];
}

/*
 * MemScanner_ReadHalfWord
 * Read a 16-bit value from base + offset (Little-Endian, pointer casting).
 */
u16 MemScanner_ReadHalfWord(ptr_t base, u32 offset)
{
    u8 *bytePtr = (u8 *)base;
    u16 *halfWordPtr = (u16 *)&bytePtr[offset];
    return *halfWordPtr;
}

/*
 * MemScanner_ReadWord
 * Read a 32-bit value from base + offset (Little-Endian, pointer casting).
 */
u32 MemScanner_ReadWord(ptr_t base, u32 offset)
{
    u8 *bytePtr = (u8 *)base;
    u32 *wordPtr = (u32 *)&bytePtr[offset];
    return *wordPtr;
}

/*
 * MemScanner_WriteByte
 * Write a single byte at base + offset.
 */
void MemScanner_WriteByte(ptr_t base, u32 offset, u8 value)
{
    u8 *bytePtr = (u8 *)base;
    bytePtr[offset] = value;
}

/*
 * MemScanner_HexDump
 * Print memory as formatted hex bytes: [HexDump] 0A 1B 2C 3D 4E 5F
 */
void MemScanner_HexDump(ptr_t base, u32 size)
{
    u8 *bytePtr = (u8 *)base;
    u32 i;

    printf("[HexDump] ");
    for (i = 0U; i < size; i++)
    {
        printf("%02X", bytePtr[i]);
        if (i < (size - 1U))
        {
            printf(" ");
        }
    }
    printf("\n");
}

/*
 * MemScanner_MemFill
 * Fill 'size' bytes starting at base with the specified value.
 */
void MemScanner_MemFill(ptr_t base, u8 value, u32 size)
{
    u8 *bytePtr = (u8 *)base;
    u32 i;

    for (i = 0U; i < size; i++)
    {
        bytePtr[i] = value;
    }
}

/*
 * MemScanner_MemCompare
 * Compare two regions byte-by-byte.
 * Returns 0 if identical, or 1-indexed offset of the first difference.
 */
u32 MemScanner_MemCompare(ptr_t ptr1, ptr_t ptr2, u32 size)
{
    u8 *p1 = (u8 *)ptr1;
    u8 *p2 = (u8 *)ptr2;
    u32 i;

    for (i = 0U; i < size; i++)
    {
        if (p1[i] != p2[i])
        {
            return (i + 1U);   /* 1-indexed offset of first difference */
        }
    }

    return 0U;   /* Regions are identical */
}

/*
 * MemScanner_FindByte
 * Search for the first occurrence of 'value' in the memory region.
 * Returns the 0-indexed offset if found, or -1 if not found.
 */
s32 MemScanner_FindByte(ptr_t base, u32 size, u8 value)
{
    u8 *bytePtr = (u8 *)base;
    u32 i;

    for (i = 0U; i < size; i++)
    {
        if (bytePtr[i] == value)
        {
            return (s32)i;
        }
    }

    return (s32)(-1);   /* Not found */
}
