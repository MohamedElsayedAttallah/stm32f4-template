/*
 * BIT_MATH.h
 *
 *  Created on: Apr 05, 2025
 *  Author    : AbdullahDarwish
 *
 *  Description: MISRA-C compliant bit manipulation macro library.
 *               All macros use unsigned literals, full parenthesization,
 *               and do not produce signed-shift operations.
 */

#ifndef BIT_MATH_H
#define BIT_MATH_H

/******************************************************************************/
/*                     Single Bit Operations                                  */
/******************************************************************************/

/* Set a single bit at position BIT in register REG */
#define SET_BIT(REG, BIT)       ((REG) |=  (1UL << (BIT)))

/* Clear a single bit at position BIT in register REG */
#define CLR_BIT(REG, BIT)       ((REG) &= ~(1UL << (BIT)))

/* Toggle a single bit at position BIT in register REG */
#define TGL_BIT(REG, BIT)      ((REG) ^=  (1UL << (BIT)))

/* Read a single bit at position BIT in register REG (returns 0 or 1) */
#define GET_BIT(REG, BIT)      (((REG) >> (BIT)) & 1UL)

/******************************************************************************/
/*                     Multi-Bit Mask Operations                              */
/******************************************************************************/

/* Set multiple bits specified by MASK in register REG */
#define SET_MASK(REG, MASK)     ((REG) |=  (MASK))

/* Clear multiple bits specified by MASK in register REG */
#define CLR_MASK(REG, MASK)     ((REG) &= ~(MASK))

/******************************************************************************/
/*                     Bit-Field Operations                                   */
/******************************************************************************/

/*
 * WRITE_BF: Write VALUE into a bit-field region of REG.
 *   REG  - the target register / variable
 *   POS  - the start-bit position of the field (LSB of the field)
 *   MSK  - the field mask already shifted to position
 *          (e.g. for a 3-bit field starting at bit 4: 0x70UL = 0b01110000)
 *   VAL  - the value to write (unshifted, will be shifted by POS)
 *
 * Steps: 1) Clear the field bits,  2) Write the new value.
 */
#define WRITE_BF(REG, POS, MSK, VAL)    \
    do {                                 \
        (REG) &= ~(MSK);                \
        (REG) |= (((u32)(VAL) << (POS)) & (MSK)); \
    } while (0U)

/*
 * READ_BF: Read / extract a value from a bit-field region of REG.
 *   REG  - the source register / variable
 *   POS  - the start-bit position of the field
 *   MSK  - the field mask already shifted to position
 *
 * Returns the field value right-aligned.
 */
#define READ_BF(REG, POS, MSK)  (((REG) & (MSK)) >> (POS))

#endif /* BIT_MATH_H */
