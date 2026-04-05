/*
 * MsgCodec.h
 *
 *  Created on: Apr 05, 2025
 *  Author    : AbdullahDarwish
 *
 *  Description: Message Encoder / Decoder module header.
 *               Encodes structured messages into raw 6-byte buffers and
 *               decodes raw buffers back into structured messages for a
 *               simulated embedded communication link.
 */

#ifndef MSGCODEC_H
#define MSGCODEC_H

/******************************************************************************/
/*                              Includes                                      */
/******************************************************************************/
#include "STD_TYPES.h"

/******************************************************************************/
/*                          Message Constants                                 */
/******************************************************************************/

/* Total message size (bytes) */
#define MSG_TOTAL_SIZE          (6U)

/* Payload size (bytes) */
#define MSG_PAYLOAD_SIZE        (4U)

/* Maximum number of message types (MSG_TYPE is 3 bits: 0-7) */
#define MSG_TYPE_MAX            (8U)

/******************************************************************************/
/*                          Message Types                                     */
/******************************************************************************/

#define MSG_TYPE_SENSOR_READING (0U)
#define MSG_TYPE_DEVICE_COMMAND (1U)

/******************************************************************************/
/*                   HEADER Byte Bit-Field Definitions                        */
/******************************************************************************/

/* MSG_TYPE: bits 2:0 */
#define MSG_TYPE_POS            (0U)
#define MSG_TYPE_MSK            (0x07U)

/* PRIORITY: bit 3 */
#define PRIORITY_POS            (3U)
#define PRIORITY_MSK            (0x08U)

/* SEQ_NUM: bits 7:4 */
#define SEQ_NUM_POS             (4U)
#define SEQ_NUM_MSK             (0xF0U)

/******************************************************************************/
/*                          Priority Values                                   */
/******************************************************************************/

#define PRIORITY_NORMAL         (0U)
#define PRIORITY_HIGH           (1U)

/******************************************************************************/
/*                          Type Definitions                                  */
/******************************************************************************/

/*
 * Payload_t
 * Union allowing the 4-byte payload to be accessed as:
 *   - A single 32-bit value  (word)
 *   - Two 16-bit half-words  (halfWord[0] = lower, halfWord[1] = upper)
 *   - Four individual bytes  (bytes[0..3])
 */
typedef union {
    u32 word;
    u16 halfWord[2];
    u8  bytes[4];
} Payload_t;

/*
 * Message_t
 * Struct that maps directly to the 6-byte message layout:
 *   Byte 0:   header  (packed MSG_TYPE + PRIORITY + SEQ_NUM)
 *   Byte 1:   length  (payload size in bytes)
 *   Bytes 2-5: payload (fixed 4-byte data field)
 *
 * Packed attribute ensures no padding — size must be exactly 6 bytes.
 */
typedef struct {
    u8        header;
    u8        length;
    Payload_t payload;
} __attribute__((packed)) Message_t;

/* Compile-time verification: Message_t must be exactly 6 bytes */
_Static_assert(sizeof(Message_t) == MSG_TOTAL_SIZE,
               "Message_t must be exactly 6 bytes");

/*
 * DecodedMsg_t
 * Structured result produced by the decoder — all header fields are
 * extracted into individual members for easy access.
 */
typedef struct {
    u8        msgType;
    u8        priority;
    u8        seqNum;
    u8        length;
    Payload_t payload;
} DecodedMsg_t;

/*
 * MsgHandler_t
 * Function pointer type for message handler callbacks.
 * Receives a pointer to the decoded message data.
 */
typedef void (*MsgHandler_t)(const DecodedMsg_t *msg);

/******************************************************************************/
/*                          Function Prototypes                               */
/******************************************************************************/

/*
 * MsgCodec_Encode
 * Description: Encode message parameters into a raw 6-byte buffer.
 *              The sequence number is managed internally (auto-increment)
 *              and is not visible outside the source file.
 * Parameters:
 *   msgType     - Message type (0-7).
 *   priority    - Priority flag (PRIORITY_NORMAL or PRIORITY_HIGH).
 *   payloadData - Pointer to 4 bytes of payload data to encode.
 *   outBuffer   - Pointer to a 6-byte output buffer.
 * Returns:   E_OK on success, E_NOT_OK on failure.
 */
Std_ReturnType MsgCodec_Encode(u8 msgType, u8 priority,
                               const u8 *payloadData, u8 *outBuffer);

/*
 * MsgCodec_Decode
 * Description: Decode a raw 6-byte buffer (as void*) into a structured
 *              DecodedMsg_t result. For SENSOR_READING messages, the payload
 *              is interpretable as the full 32-bit value, two 16-bit halves,
 *              and four individual bytes via the Payload_t union.
 *              If a handler is registered for the decoded message type, it
 *              is called automatically after successful decoding.
 * Parameters:
 *   rawBuffer - Generic pointer (void*) to a 6-byte raw message buffer.
 *   result    - Pointer to a DecodedMsg_t to fill with decoded fields.
 * Returns:   E_OK on success, E_NOT_OK on failure.
 */
Std_ReturnType MsgCodec_Decode(ptr_t rawBuffer, DecodedMsg_t *result);

/*
 * MsgCodec_RegisterHandler
 * Description: Register a handler function for a specific message type.
 *              After decoding a message, the appropriate handler is called
 *              automatically. If no handler is registered, the decoder still
 *              succeeds but skips the handler call.
 * Parameters:
 *   msgType - Message type (0-7) to register the handler for.
 *   handler - Function pointer to the handler callback.
 */
void MsgCodec_RegisterHandler(u8 msgType, MsgHandler_t handler);

#endif /* MSGCODEC_H */
