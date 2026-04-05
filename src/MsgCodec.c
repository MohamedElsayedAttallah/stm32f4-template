/*
 * MsgCodec.c
 *
 *  Created on: Apr 05, 2025
 *  Author    : AbdullahDarwish
 *
 *  Description: Message Encoder / Decoder module implementation.
 *               Uses MemScanner for safe memory access as required by
 *               the project objective.
 */

/******************************************************************************/
/*                              Includes                                      */
/******************************************************************************/
#include "MsgCodec.h"
#include "BIT_MATH.h"
#include "MemScanner.h"

/******************************************************************************/
/*                     Static (File-Scope) Variables                          */
/******************************************************************************/

/*
 * Sequence number state — auto-increments per message, wraps 0-15.
 * Not visible outside this source file.
 */
static u8 seqNumber = 0U;

/*
 * Handler table — stores one handler per message type (up to 8 types).
 * All registered handlers and internal state are encapsulated here.
 */
static MsgHandler_t handlerTable[MSG_TYPE_MAX] = {
    NULL_PTR, NULL_PTR, NULL_PTR, NULL_PTR,
    NULL_PTR, NULL_PTR, NULL_PTR, NULL_PTR
};

/******************************************************************************/
/*                       Function Implementations                             */
/******************************************************************************/

/*
 * MsgCodec_Encode
 * Encode message parameters into a raw 6-byte buffer.
 */
Std_ReturnType MsgCodec_Encode(u8 msgType, u8 priority,
                               const u8 *payloadData, u8 *outBuffer)
{
    u8 header;
    u32 i;

    /* Validate input pointers */
    if ((payloadData == NULL_PTR) || (outBuffer == NULL_PTR))
    {
        return E_NOT_OK;
    }

    /* Validate message type range (3 bits: 0-7) */
    if (msgType >= MSG_TYPE_MAX)
    {
        return E_NOT_OK;
    }

    /* Build HEADER byte using bit-field packing:
     *   Bits 2:0 = MSG_TYPE
     *   Bit  3   = PRIORITY
     *   Bits 7:4 = SEQ_NUM
     */
    header = 0U;
    header |= ((msgType  << MSG_TYPE_POS)  & MSG_TYPE_MSK);
    header |= ((priority << PRIORITY_POS)  & PRIORITY_MSK);
    header |= ((seqNumber << SEQ_NUM_POS)  & SEQ_NUM_MSK);

    /* Write header byte at offset 0 */
    MemScanner_WriteByte((ptr_t)outBuffer, 0U, header);

    /* Write length byte at offset 1 */
    MemScanner_WriteByte((ptr_t)outBuffer, 1U, MSG_PAYLOAD_SIZE);

    /* Write payload bytes at offsets 2-5 */
    for (i = 0U; i < MSG_PAYLOAD_SIZE; i++)
    {
        MemScanner_WriteByte((ptr_t)outBuffer, 2U + i, payloadData[i]);
    }

    /* Auto-increment sequence number, wrap at 16 (4-bit field: 0-15) */
    seqNumber = (seqNumber + 1U) & 0x0FU;

    return E_OK;
}

/*
 * MsgCodec_Decode
 * Decode a raw 6-byte buffer into a structured DecodedMsg_t.
 * Calls the registered handler (if any) after successful decoding.
 */
Std_ReturnType MsgCodec_Decode(ptr_t rawBuffer, DecodedMsg_t *result)
{
    u8 header;
    u32 i;

    /* Validate input pointers */
    if ((rawBuffer == NULL_PTR) || (result == NULL_PTR))
    {
        return E_NOT_OK;
    }

    /* Read header byte from offset 0 */
    header = MemScanner_ReadByte(rawBuffer, 0U);

    /* Extract header bit-fields */
    result->msgType  = (header & MSG_TYPE_MSK)  >> MSG_TYPE_POS;
    result->priority = (header & PRIORITY_MSK)  >> PRIORITY_POS;
    result->seqNum   = (header & SEQ_NUM_MSK)   >> SEQ_NUM_POS;

    /* Read length byte from offset 1 */
    result->length = MemScanner_ReadByte(rawBuffer, 1U);

    /* Read payload bytes from offsets 2-5 into the union.
     * By copying into payload.bytes[], the data is also accessible as:
     *   payload.word       — full 32-bit value
     *   payload.halfWord[] — two 16-bit halves (Little-Endian)
     *   payload.bytes[]    — four individual bytes
     */
    for (i = 0U; i < MSG_PAYLOAD_SIZE; i++)
    {
        result->payload.bytes[i] = MemScanner_ReadByte(rawBuffer, 2U + i);
    }

    /* Invoke registered handler for this message type, if one exists */
    if (result->msgType < MSG_TYPE_MAX)
    {
        if (handlerTable[result->msgType] != NULL_PTR)
        {
            handlerTable[result->msgType](result);
        }
    }

    return E_OK;
}

/*
 * MsgCodec_RegisterHandler
 * Register a callback handler for a specific message type.
 */
void MsgCodec_RegisterHandler(u8 msgType, MsgHandler_t handler)
{
    if (msgType < MSG_TYPE_MAX)
    {
        handlerTable[msgType] = handler;
    }
}
