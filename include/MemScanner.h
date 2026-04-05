/*
 * MemScanner.h
 *
 *  Created on: Apr 05, 2025
 *  Author    : AbdullahDarwish
 *
 *  Description: Memory inspection and manipulation module header.
 *               Inspired by embedded debugger tools (OpenOCD, pyOCD).
 */

#ifndef MEMSCANNER_H
#define MEMSCANNER_H

/******************************************************************************/
/*                              Includes                                      */
/******************************************************************************/
#include "STD_TYPES.h"

/******************************************************************************/
/*                          Function Prototypes                               */
/******************************************************************************/

/*
 * MemScanner_ReadByte
 * Description: Read a single byte from a memory region at a given byte offset.
 * Parameters:
 *   base   - Generic pointer (void*) to the start of the memory region.
 *   offset - Byte offset from the base address.
 * Returns:   The byte value at (base + offset).
 */
u8 MemScanner_ReadByte(ptr_t base, u32 offset);

/*
 * MemScanner_ReadHalfWord
 * Description: Read a 16-bit value from a memory region at a given byte offset.
 *              Assumes Little-Endian byte order using pointer casting.
 * Parameters:
 *   base   - Generic pointer to the start of the memory region.
 *   offset - Byte offset from the base address.
 * Returns:   The 16-bit value at (base + offset).
 */
u16 MemScanner_ReadHalfWord(ptr_t base, u32 offset);

/*
 * MemScanner_ReadWord
 * Description: Read a 32-bit value from a memory region at a given byte offset.
 *              Assumes Little-Endian byte order using pointer casting.
 * Parameters:
 *   base   - Generic pointer to the start of the memory region.
 *   offset - Byte offset from the base address.
 * Returns:   The 32-bit value at (base + offset).
 */
u32 MemScanner_ReadWord(ptr_t base, u32 offset);

/*
 * MemScanner_WriteByte
 * Description: Write a single byte value into a memory region at a given offset.
 * Parameters:
 *   base   - Generic pointer to the start of the memory region.
 *   offset - Byte offset from the base address.
 *   value  - The byte value to write.
 */
void MemScanner_WriteByte(ptr_t base, u32 offset, u8 value);

/*
 * MemScanner_HexDump
 * Description: Print a region of memory as formatted hex bytes.
 *              Output format: [HexDump] 0A 1B 2C 3D 4E 5F
 * Parameters:
 *   base   - Generic pointer to the start of the memory region.
 *   size   - Number of bytes to dump.
 */
void MemScanner_HexDump(ptr_t base, u32 size);

/*
 * MemScanner_MemFill
 * Description: Fill a memory region of a given size with a specified byte value.
 * Parameters:
 *   base   - Generic pointer to the start of the memory region.
 *   value  - The byte value to fill with.
 *   size   - Number of bytes to fill.
 */
void MemScanner_MemFill(ptr_t base, u8 value, u32 size);

/*
 * MemScanner_MemCompare
 * Description: Compare two memory regions of a given size byte-by-byte.
 * Parameters:
 *   ptr1   - Generic pointer to the first memory region.
 *   ptr2   - Generic pointer to the second memory region.
 *   size   - Number of bytes to compare.
 * Returns:   0 if identical, or the offset of the first differing byte
 *            (1-indexed) if they differ.
 */
u32 MemScanner_MemCompare(ptr_t ptr1, ptr_t ptr2, u32 size);

/*
 * MemScanner_FindByte
 * Description: Search a memory region for the first occurrence of a specified
 *              byte value.
 * Parameters:
 *   base   - Generic pointer to the start of the memory region.
 *   size   - Number of bytes to search.
 *   value  - The byte value to search for.
 * Returns:   The offset if found, or -1 (cast to s32) if not found.
 */
s32 MemScanner_FindByte(ptr_t base, u32 size, u8 value);

#endif /* MEMSCANNER_H */
