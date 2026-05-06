#pragma once
// MESSAGE CUSTOM_NAVIGATION PACKING

#define MAVLINK_MSG_ID_CUSTOM_NAVIGATION 151


typedef struct __mavlink_custom_navigation_t {
 int32_t lat; /*<  Latitude, expressed as degrees * 1E7*/
 int32_t lon; /*<  Longitude, expressed as degrees * 1E7*/
 int32_t alt_mm; /*< [mm] Altitude above MSL (millimeters)*/
 uint32_t time_boot_ms; /*< [ms] Timestamp (milliseconds since system boot)*/
 uint16_t vel_cm_s; /*< [cm/s] Ground speed (centimeters/second)*/
 uint16_t heading_cdeg; /*< [cdeg] Heading (centi-degrees, 0.00 to 359.99)*/
} mavlink_custom_navigation_t;

#define MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN 20
#define MAVLINK_MSG_ID_CUSTOM_NAVIGATION_MIN_LEN 20
#define MAVLINK_MSG_ID_151_LEN 20
#define MAVLINK_MSG_ID_151_MIN_LEN 20

#define MAVLINK_MSG_ID_CUSTOM_NAVIGATION_CRC 172
#define MAVLINK_MSG_ID_151_CRC 172



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_CUSTOM_NAVIGATION { \
    151, \
    "CUSTOM_NAVIGATION", \
    6, \
    {  { "lat", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_custom_navigation_t, lat) }, \
         { "lon", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_custom_navigation_t, lon) }, \
         { "alt_mm", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_custom_navigation_t, alt_mm) }, \
         { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 12, offsetof(mavlink_custom_navigation_t, time_boot_ms) }, \
         { "vel_cm_s", NULL, MAVLINK_TYPE_UINT16_T, 0, 16, offsetof(mavlink_custom_navigation_t, vel_cm_s) }, \
         { "heading_cdeg", NULL, MAVLINK_TYPE_UINT16_T, 0, 18, offsetof(mavlink_custom_navigation_t, heading_cdeg) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_CUSTOM_NAVIGATION { \
    "CUSTOM_NAVIGATION", \
    6, \
    {  { "lat", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_custom_navigation_t, lat) }, \
         { "lon", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_custom_navigation_t, lon) }, \
         { "alt_mm", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_custom_navigation_t, alt_mm) }, \
         { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 12, offsetof(mavlink_custom_navigation_t, time_boot_ms) }, \
         { "vel_cm_s", NULL, MAVLINK_TYPE_UINT16_T, 0, 16, offsetof(mavlink_custom_navigation_t, vel_cm_s) }, \
         { "heading_cdeg", NULL, MAVLINK_TYPE_UINT16_T, 0, 18, offsetof(mavlink_custom_navigation_t, heading_cdeg) }, \
         } \
}
#endif

/**
 * @brief Pack a custom_navigation message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param lat  Latitude, expressed as degrees * 1E7
 * @param lon  Longitude, expressed as degrees * 1E7
 * @param alt_mm [mm] Altitude above MSL (millimeters)
 * @param time_boot_ms [ms] Timestamp (milliseconds since system boot)
 * @param vel_cm_s [cm/s] Ground speed (centimeters/second)
 * @param heading_cdeg [cdeg] Heading (centi-degrees, 0.00 to 359.99)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_custom_navigation_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               int32_t lat, int32_t lon, int32_t alt_mm, uint32_t time_boot_ms, uint16_t vel_cm_s, uint16_t heading_cdeg)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN];
    _mav_put_int32_t(buf, 0, lat);
    _mav_put_int32_t(buf, 4, lon);
    _mav_put_int32_t(buf, 8, alt_mm);
    _mav_put_uint32_t(buf, 12, time_boot_ms);
    _mav_put_uint16_t(buf, 16, vel_cm_s);
    _mav_put_uint16_t(buf, 18, heading_cdeg);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN);
#else
    mavlink_custom_navigation_t packet;
    packet.lat = lat;
    packet.lon = lon;
    packet.alt_mm = alt_mm;
    packet.time_boot_ms = time_boot_ms;
    packet.vel_cm_s = vel_cm_s;
    packet.heading_cdeg = heading_cdeg;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CUSTOM_NAVIGATION;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_CRC);
}

/**
 * @brief Pack a custom_navigation message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param lat  Latitude, expressed as degrees * 1E7
 * @param lon  Longitude, expressed as degrees * 1E7
 * @param alt_mm [mm] Altitude above MSL (millimeters)
 * @param time_boot_ms [ms] Timestamp (milliseconds since system boot)
 * @param vel_cm_s [cm/s] Ground speed (centimeters/second)
 * @param heading_cdeg [cdeg] Heading (centi-degrees, 0.00 to 359.99)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_custom_navigation_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               int32_t lat, int32_t lon, int32_t alt_mm, uint32_t time_boot_ms, uint16_t vel_cm_s, uint16_t heading_cdeg)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN];
    _mav_put_int32_t(buf, 0, lat);
    _mav_put_int32_t(buf, 4, lon);
    _mav_put_int32_t(buf, 8, alt_mm);
    _mav_put_uint32_t(buf, 12, time_boot_ms);
    _mav_put_uint16_t(buf, 16, vel_cm_s);
    _mav_put_uint16_t(buf, 18, heading_cdeg);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN);
#else
    mavlink_custom_navigation_t packet;
    packet.lat = lat;
    packet.lon = lon;
    packet.alt_mm = alt_mm;
    packet.time_boot_ms = time_boot_ms;
    packet.vel_cm_s = vel_cm_s;
    packet.heading_cdeg = heading_cdeg;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CUSTOM_NAVIGATION;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN);
#endif
}

/**
 * @brief Pack a custom_navigation message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param lat  Latitude, expressed as degrees * 1E7
 * @param lon  Longitude, expressed as degrees * 1E7
 * @param alt_mm [mm] Altitude above MSL (millimeters)
 * @param time_boot_ms [ms] Timestamp (milliseconds since system boot)
 * @param vel_cm_s [cm/s] Ground speed (centimeters/second)
 * @param heading_cdeg [cdeg] Heading (centi-degrees, 0.00 to 359.99)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_custom_navigation_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   int32_t lat,int32_t lon,int32_t alt_mm,uint32_t time_boot_ms,uint16_t vel_cm_s,uint16_t heading_cdeg)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN];
    _mav_put_int32_t(buf, 0, lat);
    _mav_put_int32_t(buf, 4, lon);
    _mav_put_int32_t(buf, 8, alt_mm);
    _mav_put_uint32_t(buf, 12, time_boot_ms);
    _mav_put_uint16_t(buf, 16, vel_cm_s);
    _mav_put_uint16_t(buf, 18, heading_cdeg);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN);
#else
    mavlink_custom_navigation_t packet;
    packet.lat = lat;
    packet.lon = lon;
    packet.alt_mm = alt_mm;
    packet.time_boot_ms = time_boot_ms;
    packet.vel_cm_s = vel_cm_s;
    packet.heading_cdeg = heading_cdeg;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CUSTOM_NAVIGATION;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_CRC);
}

/**
 * @brief Encode a custom_navigation struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param custom_navigation C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_custom_navigation_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_custom_navigation_t* custom_navigation)
{
    return mavlink_msg_custom_navigation_pack(system_id, component_id, msg, custom_navigation->lat, custom_navigation->lon, custom_navigation->alt_mm, custom_navigation->time_boot_ms, custom_navigation->vel_cm_s, custom_navigation->heading_cdeg);
}

/**
 * @brief Encode a custom_navigation struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param custom_navigation C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_custom_navigation_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_custom_navigation_t* custom_navigation)
{
    return mavlink_msg_custom_navigation_pack_chan(system_id, component_id, chan, msg, custom_navigation->lat, custom_navigation->lon, custom_navigation->alt_mm, custom_navigation->time_boot_ms, custom_navigation->vel_cm_s, custom_navigation->heading_cdeg);
}

/**
 * @brief Encode a custom_navigation struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param custom_navigation C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_custom_navigation_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_custom_navigation_t* custom_navigation)
{
    return mavlink_msg_custom_navigation_pack_status(system_id, component_id, _status, msg,  custom_navigation->lat, custom_navigation->lon, custom_navigation->alt_mm, custom_navigation->time_boot_ms, custom_navigation->vel_cm_s, custom_navigation->heading_cdeg);
}

/**
 * @brief Send a custom_navigation message
 * @param chan MAVLink channel to send the message
 *
 * @param lat  Latitude, expressed as degrees * 1E7
 * @param lon  Longitude, expressed as degrees * 1E7
 * @param alt_mm [mm] Altitude above MSL (millimeters)
 * @param time_boot_ms [ms] Timestamp (milliseconds since system boot)
 * @param vel_cm_s [cm/s] Ground speed (centimeters/second)
 * @param heading_cdeg [cdeg] Heading (centi-degrees, 0.00 to 359.99)
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_custom_navigation_send(mavlink_channel_t chan, int32_t lat, int32_t lon, int32_t alt_mm, uint32_t time_boot_ms, uint16_t vel_cm_s, uint16_t heading_cdeg)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN];
    _mav_put_int32_t(buf, 0, lat);
    _mav_put_int32_t(buf, 4, lon);
    _mav_put_int32_t(buf, 8, alt_mm);
    _mav_put_uint32_t(buf, 12, time_boot_ms);
    _mav_put_uint16_t(buf, 16, vel_cm_s);
    _mav_put_uint16_t(buf, 18, heading_cdeg);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUSTOM_NAVIGATION, buf, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_CRC);
#else
    mavlink_custom_navigation_t packet;
    packet.lat = lat;
    packet.lon = lon;
    packet.alt_mm = alt_mm;
    packet.time_boot_ms = time_boot_ms;
    packet.vel_cm_s = vel_cm_s;
    packet.heading_cdeg = heading_cdeg;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUSTOM_NAVIGATION, (const char *)&packet, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_CRC);
#endif
}

/**
 * @brief Send a custom_navigation message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_custom_navigation_send_struct(mavlink_channel_t chan, const mavlink_custom_navigation_t* custom_navigation)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_custom_navigation_send(chan, custom_navigation->lat, custom_navigation->lon, custom_navigation->alt_mm, custom_navigation->time_boot_ms, custom_navigation->vel_cm_s, custom_navigation->heading_cdeg);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUSTOM_NAVIGATION, (const char *)custom_navigation, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_CRC);
#endif
}

#if MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_custom_navigation_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  int32_t lat, int32_t lon, int32_t alt_mm, uint32_t time_boot_ms, uint16_t vel_cm_s, uint16_t heading_cdeg)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_int32_t(buf, 0, lat);
    _mav_put_int32_t(buf, 4, lon);
    _mav_put_int32_t(buf, 8, alt_mm);
    _mav_put_uint32_t(buf, 12, time_boot_ms);
    _mav_put_uint16_t(buf, 16, vel_cm_s);
    _mav_put_uint16_t(buf, 18, heading_cdeg);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUSTOM_NAVIGATION, buf, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_CRC);
#else
    mavlink_custom_navigation_t *packet = (mavlink_custom_navigation_t *)msgbuf;
    packet->lat = lat;
    packet->lon = lon;
    packet->alt_mm = alt_mm;
    packet->time_boot_ms = time_boot_ms;
    packet->vel_cm_s = vel_cm_s;
    packet->heading_cdeg = heading_cdeg;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUSTOM_NAVIGATION, (const char *)packet, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_CRC);
#endif
}
#endif

#endif

// MESSAGE CUSTOM_NAVIGATION UNPACKING


/**
 * @brief Get field lat from custom_navigation message
 *
 * @return  Latitude, expressed as degrees * 1E7
 */
static inline int32_t mavlink_msg_custom_navigation_get_lat(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  0);
}

/**
 * @brief Get field lon from custom_navigation message
 *
 * @return  Longitude, expressed as degrees * 1E7
 */
static inline int32_t mavlink_msg_custom_navigation_get_lon(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  4);
}

/**
 * @brief Get field alt_mm from custom_navigation message
 *
 * @return [mm] Altitude above MSL (millimeters)
 */
static inline int32_t mavlink_msg_custom_navigation_get_alt_mm(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  8);
}

/**
 * @brief Get field time_boot_ms from custom_navigation message
 *
 * @return [ms] Timestamp (milliseconds since system boot)
 */
static inline uint32_t mavlink_msg_custom_navigation_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  12);
}

/**
 * @brief Get field vel_cm_s from custom_navigation message
 *
 * @return [cm/s] Ground speed (centimeters/second)
 */
static inline uint16_t mavlink_msg_custom_navigation_get_vel_cm_s(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  16);
}

/**
 * @brief Get field heading_cdeg from custom_navigation message
 *
 * @return [cdeg] Heading (centi-degrees, 0.00 to 359.99)
 */
static inline uint16_t mavlink_msg_custom_navigation_get_heading_cdeg(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  18);
}

/**
 * @brief Decode a custom_navigation message into a struct
 *
 * @param msg The message to decode
 * @param custom_navigation C-struct to decode the message contents into
 */
static inline void mavlink_msg_custom_navigation_decode(const mavlink_message_t* msg, mavlink_custom_navigation_t* custom_navigation)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    custom_navigation->lat = mavlink_msg_custom_navigation_get_lat(msg);
    custom_navigation->lon = mavlink_msg_custom_navigation_get_lon(msg);
    custom_navigation->alt_mm = mavlink_msg_custom_navigation_get_alt_mm(msg);
    custom_navigation->time_boot_ms = mavlink_msg_custom_navigation_get_time_boot_ms(msg);
    custom_navigation->vel_cm_s = mavlink_msg_custom_navigation_get_vel_cm_s(msg);
    custom_navigation->heading_cdeg = mavlink_msg_custom_navigation_get_heading_cdeg(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN? msg->len : MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN;
        memset(custom_navigation, 0, MAVLINK_MSG_ID_CUSTOM_NAVIGATION_LEN);
    memcpy(custom_navigation, _MAV_PAYLOAD(msg), len);
#endif
}
