/*
 * main.c
 *
 *  Created on: Feb 18, 2025
 *  Author    : AbdullahDarwish
 *
 *  Description: Part C — Demonstration.
 *               Exercises the full Message Encoder/Decoder (Part B) and
 *               Memory Scanner API (Part A) with concrete test scenarios.
 */

/******************************************************************************/
/*                              Includes                                      */
/******************************************************************************/
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "MemScanner.h"
#include "MsgCodec.h"
#include <stdio.h>
#include <inttypes.h>

/******************************************************************************/
/*                     Message Handler Callbacks                              */
/******************************************************************************/

/*
 * SensorReadingHandler
 * Prints: the 32-bit raw value, the two 16-bit values, and the four
 * individual bytes (as required by step 5).
 */
static void SensorReadingHandler(const DecodedMsg_t *msg)
{
    printf("  [Handler] SENSOR_READING decoded:\n");
    printf("    32-bit raw value : 0x%08" PRIX32 "\n",
           (u32)msg->payload.word);
    printf("    Temperature (lower 16-bit): %" PRIu16 " (= 25.5 C)\n",
           msg->payload.halfWord[0]);
    printf("    Humidity    (upper 16-bit): %" PRIu16 " (= 60.0 %%RH)\n",
           msg->payload.halfWord[1]);
    printf("    Bytes: [0]=0x%02X  [1]=0x%02X  [2]=0x%02X  [3]=0x%02X\n",
           msg->payload.bytes[0], msg->payload.bytes[1],
           msg->payload.bytes[2], msg->payload.bytes[3]);
}

/*
 * DeviceCommandHandler
 * Prints: command ID, parameter, and the two unused bytes.
 */
static void DeviceCommandHandler(const DecodedMsg_t *msg)
{
    printf("  [Handler] DEVICE_COMMAND decoded:\n");
    printf("    Command ID : 0x%02X\n", msg->payload.bytes[0]);
    printf("    Parameter  : 0x%02X\n", msg->payload.bytes[1]);
    printf("    Unused [2] : 0x%02X\n", msg->payload.bytes[2]);
    printf("    Unused [3] : 0x%02X\n", msg->payload.bytes[3]);
}

/******************************************************************************/
/*                              Main Function                                 */
/******************************************************************************/

s32 main(void)
{
    /* ---- Message buffers ---- */
    u8 sensorBuffer[MSG_TOTAL_SIZE];
    u8 commandBuffer[MSG_TOTAL_SIZE];

    /* Sensor payload: temperature = 25.5 C  => 255 in 0.1 C units  = 0x00FF
     *                 humidity    = 60.0 %  => 600 in 0.1 %RH units = 0x0258
     * Little-Endian byte layout:
     *   bytes[0]=0xFF, bytes[1]=0x00  =>  halfWord[0] = 0x00FF (temperature)
     *   bytes[2]=0x58, bytes[3]=0x02  =>  halfWord[1] = 0x0258 (humidity)
     */
    u8 sensorPayload[MSG_PAYLOAD_SIZE] = { 0xFFU, 0x00U, 0x58U, 0x02U };

    /* Command payload: cmdID=0x01, param=0xFF, 2 unused bytes (zero-filled) */
    u8 commandPayload[MSG_PAYLOAD_SIZE] = { 0x01U, 0xFFU, 0x00U, 0x00U };

    /* Decoded message result */
    DecodedMsg_t decoded;

    /* Variables for MemScanner demonstrations */
    u8  headerByte;
    u8  lengthByte;
    u16 tempValue;
    u32 fullPayload;
    u8  sensorCopy[MSG_TOTAL_SIZE];
    u8  fillBuffer[MSG_TOTAL_SIZE];
    u8  cmpBuffer1[MSG_TOTAL_SIZE];
    u8  cmpBuffer2[MSG_TOTAL_SIZE];
    u32 cmpResult;
    s32 findResult;
    u32 i;

    printf("========================================\n");
    printf("  Project 1 — Part C Demonstration\n");
    printf("========================================\n\n");

    /* ================================================================ */
    /*  C1: Message Encode / Decode                                     */
    /* ================================================================ */
    printf("--- C1: Message Encode / Decode ---\n\n");

    /* Step 1: Register a handler for SENSOR_READING and DEVICE_COMMAND */
    printf("[Step 1] Registering handlers...\n");
    MsgCodec_RegisterHandler(MSG_TYPE_SENSOR_READING, SensorReadingHandler);
    MsgCodec_RegisterHandler(MSG_TYPE_DEVICE_COMMAND, DeviceCommandHandler);
    printf("  Handlers registered for SENSOR_READING and DEVICE_COMMAND.\n\n");

    /* Step 2: Encode a SENSOR_READING message (temp=25.5C, humidity=60.0%) */
    printf("[Step 2] Encoding SENSOR_READING (temp=25.5C, humidity=60.0%%)...\n");
    MsgCodec_Encode(MSG_TYPE_SENSOR_READING, PRIORITY_NORMAL,
                    sensorPayload, sensorBuffer);
    printf("  Encoded successfully.\n\n");

    /* Step 3: Encode a DEVICE_COMMAND message (cmdID=0x01, param=0xFF) */
    printf("[Step 3] Encoding DEVICE_COMMAND (cmdID=0x01, param=0xFF)...\n");
    MsgCodec_Encode(MSG_TYPE_DEVICE_COMMAND, PRIORITY_NORMAL,
                    commandPayload, commandBuffer);
    printf("  Encoded successfully.\n\n");

    /* Step 4 & 5: Decode each message — handlers automatically print contents */
    printf("[Step 4] Decoding SENSOR_READING message...\n");
    MsgCodec_Decode((ptr_t)sensorBuffer, &decoded);
    printf("  MsgType=%" PRIu8 "  Priority=%" PRIu8
           "  SeqNum=%" PRIu8 "  Length=%" PRIu8 "\n\n",
           decoded.msgType, decoded.priority,
           decoded.seqNum, decoded.length);

    printf("[Step 4] Decoding DEVICE_COMMAND message...\n");
    MsgCodec_Decode((ptr_t)commandBuffer, &decoded);
    printf("  MsgType=%" PRIu8 "  Priority=%" PRIu8
           "  SeqNum=%" PRIu8 "  Length=%" PRIu8 "\n\n",
           decoded.msgType, decoded.priority,
           decoded.seqNum, decoded.length);

    /* ================================================================ */
    /*  C2: Memory Scanner Demonstration                                */
    /* ================================================================ */
    printf("--- C2: Memory Scanner Demonstration ---\n\n");

    /* Step 6: HexDump — display raw bytes of each encoded message buffer */
    printf("[Step 6] HexDump of encoded message buffers:\n");
    printf("  Sensor  : ");
    MemScanner_HexDump((ptr_t)sensorBuffer, MSG_TOTAL_SIZE);
    printf("  Command : ");
    MemScanner_HexDump((ptr_t)commandBuffer, MSG_TOTAL_SIZE);
    printf("\n");

    /* Step 7: ReadByte — read HEADER (offset 0) and LENGTH (offset 1) */
    printf("[Step 7] ReadByte from sensor buffer:\n");
    headerByte = MemScanner_ReadByte((ptr_t)sensorBuffer, 0U);
    lengthByte = MemScanner_ReadByte((ptr_t)sensorBuffer, 1U);
    printf("  HEADER (offset 0) = 0x%02X\n", headerByte);
    printf("  LENGTH (offset 1) = 0x%02X (%" PRIu8 " bytes)\n\n",
           lengthByte, lengthByte);

    /* Step 8: ReadHalfWord — temperature (lower 16 bits of payload at offset 2) */
    printf("[Step 8] ReadHalfWord — temperature from sensor at offset 2:\n");
    tempValue = MemScanner_ReadHalfWord((ptr_t)sensorBuffer, 2U);
    printf("  Temperature = %" PRIu16 " (0x%04X) => 25.5 C\n\n",
           tempValue, tempValue);

    /* Step 9: ReadWord — full 32-bit payload from sensor at offset 2 */
    printf("[Step 9] ReadWord — full 32-bit payload from sensor at offset 2:\n");
    fullPayload = MemScanner_ReadWord((ptr_t)sensorBuffer, 2U);
    printf("  Full payload = 0x%08" PRIX32 "\n\n", fullPayload);

    /* Step 10: WriteByte — copy sensor buffer, change PRIORITY bit in HEADER */
    printf("[Step 10] WriteByte — modify PRIORITY bit in sensor copy:\n");
    /* Create a byte-for-byte copy of the sensor buffer */
    for (i = 0U; i < MSG_TOTAL_SIZE; i++)
    {
        sensorCopy[i] = sensorBuffer[i];
    }
    /* Read header from copy, set PRIORITY bit (bit 3), write it back */
    headerByte = MemScanner_ReadByte((ptr_t)sensorCopy, 0U);
    SET_BIT(headerByte, PRIORITY_POS);
    MemScanner_WriteByte((ptr_t)sensorCopy, 0U, headerByte);
    /* Print both buffers to show the difference */
    printf("  Original : ");
    MemScanner_HexDump((ptr_t)sensorBuffer, MSG_TOTAL_SIZE);
    printf("  Modified : ");
    MemScanner_HexDump((ptr_t)sensorCopy, MSG_TOTAL_SIZE);
    printf("\n");

    /* Step 11: MemFill — declare 6-byte buffer, fill with 0xAA, then HexDump */
    printf("[Step 11] MemFill — fill 6-byte buffer with 0xAA:\n");
    MemScanner_MemFill((ptr_t)fillBuffer, 0xAAU, MSG_TOTAL_SIZE);
    printf("  Filled   : ");
    MemScanner_HexDump((ptr_t)fillBuffer, MSG_TOTAL_SIZE);
    printf("\n");

    /* Step 12: MemCompare — compare identical buffers, then after modification */
    printf("[Step 12] MemCompare:\n");
    /* Copy the sensor buffer into two separate compare buffers */
    for (i = 0U; i < MSG_TOTAL_SIZE; i++)
    {
        cmpBuffer1[i] = sensorBuffer[i];
        cmpBuffer2[i] = sensorBuffer[i];
    }
    /* Compare identical buffers — should return 0 */
    cmpResult = MemScanner_MemCompare((ptr_t)cmpBuffer1, (ptr_t)cmpBuffer2,
                                      MSG_TOTAL_SIZE);
    printf("  Identical buffers  => MemCompare = %" PRIu32 " (expected 0)\n",
           cmpResult);
    /* Modify one byte in cmpBuffer2, then compare again */
    cmpBuffer2[3] = 0xBBU;
    cmpResult = MemScanner_MemCompare((ptr_t)cmpBuffer1, (ptr_t)cmpBuffer2,
                                      MSG_TOTAL_SIZE);
    printf("  Modified byte [3]  => MemCompare = %" PRIu32 " (expected 4)\n\n",
           cmpResult);

    /* Step 13: FindByte — search for known and unknown bytes */
    printf("[Step 13] FindByte:\n");
    /* Search for 0xFF — should be at offset 2 in sensorBuffer */
    findResult = MemScanner_FindByte((ptr_t)sensorBuffer, MSG_TOTAL_SIZE, 0xFFU);
    printf("  Search 0xFF => offset = %" PRId32 " (expected 2)\n",
           findResult);
    /* Search for 0xAB — does not exist, should return -1 */
    findResult = MemScanner_FindByte((ptr_t)sensorBuffer, MSG_TOTAL_SIZE, 0xABU);
    printf("  Search 0xAB => offset = %" PRId32 " (expected -1)\n\n",
           findResult);

    printf("========================================\n");
    printf("  Demonstration Complete.\n");
    printf("========================================\n");

    return 0;
}
