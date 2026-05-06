/** @file
 *  @brief MAVLink comm protocol generated from custom_telemetry.xml
 *  @see http://mavlink.org
 */
#pragma once
#ifndef MAVLINK_CUSTOM_TELEMETRY_H
#define MAVLINK_CUSTOM_TELEMETRY_H

#ifndef MAVLINK_H
    #error Wrong include order: MAVLINK_CUSTOM_TELEMETRY.H MUST NOT BE DIRECTLY USED. Include mavlink.h from the same directory instead or set ALL AND EVERY defines from MAVLINK.H manually accordingly, including the #define MAVLINK_H call.
#endif

#define MAVLINK_CUSTOM_TELEMETRY_XML_HASH -6039139953589558525

#ifdef __cplusplus
extern "C" {
#endif

// MESSAGE LENGTHS AND CRCS

#ifndef MAVLINK_MESSAGE_LENGTHS
#define MAVLINK_MESSAGE_LENGTHS {}
#endif

#ifndef MAVLINK_MESSAGE_CRCS
#define MAVLINK_MESSAGE_CRCS {{0, 50, 9, 9, 0, 0, 0}, {150, 0, 16, 16, 0, 0, 0}, {151, 172, 20, 20, 0, 0, 0}, {152, 128, 7, 7, 0, 0, 0}}
#endif

#include "../protocol.h"

#define MAVLINK_ENABLED_CUSTOM_TELEMETRY

// ENUM DEFINITIONS


/** @brief Bitmask representing the health of the UAV subsystems. */
#ifndef HAVE_ENUM_CUSTOM_SYS_STATUS_FLAGS
#define HAVE_ENUM_CUSTOM_SYS_STATUS_FLAGS
typedef enum CUSTOM_SYS_STATUS_FLAGS
{
   CUSTOM_SYS_IMU_HEALTHY=1, /* MPU6050 is responding and data is valid. | */
   CUSTOM_SYS_SD_LOGGING=2, /* SD Card is mounted and actively writing CSVs. | */
   CUSTOM_SYS_RADIO_LOCKED=4, /* LoRa PLL is locked and ready to transmit. | */
   CUSTOM_SYS_GPS_FIX=8, /* GPS has a 3D fix (Reserved for future use). | */
   CUSTOM_SYS_STATUS_FLAGS_ENUM_END=9, /*  | */
} CUSTOM_SYS_STATUS_FLAGS;
#endif

// MAVLINK VERSION

#ifndef MAVLINK_VERSION
#define MAVLINK_VERSION 1
#endif

#if (MAVLINK_VERSION == 0)
#undef MAVLINK_VERSION
#define MAVLINK_VERSION 1
#endif

// MESSAGE DEFINITIONS
#include "./mavlink_msg_custom_attitude.h"
#include "./mavlink_msg_custom_navigation.h"
#include "./mavlink_msg_custom_health.h"

// base include
#include "../minimal/minimal.h"


#if MAVLINK_CUSTOM_TELEMETRY_XML_HASH == MAVLINK_PRIMARY_XML_HASH
# define MAVLINK_MESSAGE_INFO {MAVLINK_MESSAGE_INFO_HEARTBEAT, MAVLINK_MESSAGE_INFO_CUSTOM_ATTITUDE, MAVLINK_MESSAGE_INFO_CUSTOM_NAVIGATION, MAVLINK_MESSAGE_INFO_CUSTOM_HEALTH}
# define MAVLINK_MESSAGE_NAMES {{ "CUSTOM_ATTITUDE", 150 }, { "CUSTOM_HEALTH", 152 }, { "CUSTOM_NAVIGATION", 151 }, { "HEARTBEAT", 0 }}
# if MAVLINK_COMMAND_24BIT
#  include "../mavlink_get_info.h"
# endif
#endif

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // MAVLINK_CUSTOM_TELEMETRY_H
