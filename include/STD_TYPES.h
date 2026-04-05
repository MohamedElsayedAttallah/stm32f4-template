/*
 * STD_TYPES.h
 *
 *  Created on: Apr 05, 2025
 *  Author    : AbdullahDarwish
 *
 *  Description: Portable types header that maps all project types to <stdint.h>.
 *               No raw C types (int, char, long, unsigned) should appear anywhere
 *               in the project outside this file.
 */

#ifndef STD_TYPES_H
#define STD_TYPES_H

/******************************************************************************/
/*                              Includes                                      */
/******************************************************************************/
#include <stdint.h>

/******************************************************************************/
/*                     Fixed-Width Unsigned Integer Types                      */
/******************************************************************************/
typedef uint8_t         u8;
typedef uint16_t        u16;
typedef uint32_t        u32;

/******************************************************************************/
/*                     Fixed-Width Signed Integer Types                        */
/******************************************************************************/
typedef int8_t          s8;
typedef int16_t         s16;
typedef int32_t         s32;

/******************************************************************************/
/*                              Boolean Type                                  */
/******************************************************************************/
typedef uint8_t         bool_t;

#define TRUE            ((bool_t)1U)
#define FALSE           ((bool_t)0U)

/******************************************************************************/
/*                     Standard Function Return Type                          */
/******************************************************************************/
typedef uint8_t         Std_ReturnType;

#define E_OK            ((Std_ReturnType)0U)
#define E_NOT_OK        ((Std_ReturnType)1U)

/******************************************************************************/
/*                          Generic Pointer Type                              */
/******************************************************************************/
typedef void*           ptr_t;

/******************************************************************************/
/*                           NULL Pointer                                     */
/******************************************************************************/
#ifndef NULL_PTR
#define NULL_PTR        ((void*)0)
#endif

#endif /* STD_TYPES_H */
