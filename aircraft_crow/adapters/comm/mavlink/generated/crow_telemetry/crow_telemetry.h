/** @file
 *  @brief MAVLink comm protocol generated from crow_telemetry.xml
 *  @see http://mavlink.org
 */
#pragma once
#ifndef MAVLINK_CROW_TELEMETRY_H
#define MAVLINK_CROW_TELEMETRY_H

#ifndef MAVLINK_H
    #error Wrong include order: MAVLINK_CROW_TELEMETRY.H MUST NOT BE DIRECTLY USED. Include mavlink.h from the same directory instead or set ALL AND EVERY defines from MAVLINK.H manually accordingly, including the #define MAVLINK_H call.
#endif

#define MAVLINK_CROW_TELEMETRY_XML_HASH -1893639384748692318

#ifdef __cplusplus
extern "C" {
#endif

// MESSAGE LENGTHS AND CRCS

#ifndef MAVLINK_MESSAGE_LENGTHS
#define MAVLINK_MESSAGE_LENGTHS {}
#endif

#ifndef MAVLINK_MESSAGE_CRCS
#define MAVLINK_MESSAGE_CRCS {{0, 50, 9, 9, 0, 0, 0}, {201, 103, 22, 22, 0, 0, 0}, {202, 89, 8, 8, 0, 0, 0}, {203, 134, 17, 17, 0, 0, 0}, {204, 116, 5, 5, 0, 0, 0}}
#endif

#include "../protocol.h"

#define MAVLINK_ENABLED_CROW_TELEMETRY

// ENUM DEFINITIONS


/** @brief Comandos do piloto via PWM e estado do sistema */
#ifndef HAVE_ENUM_CROW_COMMAND_TYPE
#define HAVE_ENUM_CROW_COMMAND_TYPE
typedef enum CROW_COMMAND_TYPE
{
   CROW_CMD_IDLE=0, /*  | */
   CROW_CMD_START_RECORDING=1, /*  | */
   CROW_CMD_STOP_RECORDING=2, /*  | */
   CROW_COMMAND_TYPE_ENUM_END=3, /*  | */
} CROW_COMMAND_TYPE;
#endif

// MAVLINK VERSION

#ifndef MAVLINK_VERSION
#define MAVLINK_VERSION 2
#endif

#if (MAVLINK_VERSION == 0)
#undef MAVLINK_VERSION
#define MAVLINK_VERSION 2
#endif

// MESSAGE DEFINITIONS
#include "./mavlink_msg_crow_imu.h"
#include "./mavlink_msg_crow_baro.h"
#include "./mavlink_msg_crow_gps.h"
#include "./mavlink_msg_crow_command.h"

// base include
#include "../minimal/minimal.h"


#if MAVLINK_CROW_TELEMETRY_XML_HASH == MAVLINK_PRIMARY_XML_HASH
# define MAVLINK_MESSAGE_INFO {MAVLINK_MESSAGE_INFO_HEARTBEAT, MAVLINK_MESSAGE_INFO_CROW_IMU, MAVLINK_MESSAGE_INFO_CROW_BARO, MAVLINK_MESSAGE_INFO_CROW_GPS, MAVLINK_MESSAGE_INFO_CROW_COMMAND}
# define MAVLINK_MESSAGE_NAMES {{ "CROW_BARO", 202 }, { "CROW_COMMAND", 204 }, { "CROW_GPS", 203 }, { "CROW_IMU", 201 }, { "HEARTBEAT", 0 }}
# if MAVLINK_COMMAND_24BIT
#  include "../mavlink_get_info.h"
# endif
#endif

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // MAVLINK_CROW_TELEMETRY_H
