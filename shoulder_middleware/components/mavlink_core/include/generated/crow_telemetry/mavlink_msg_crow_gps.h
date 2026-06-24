#pragma once
// MESSAGE CROW_GPS PACKING

#define MAVLINK_MSG_ID_CROW_GPS 203


typedef struct __mavlink_crow_gps_t {
 uint32_t time_boot_ms; /*<  Timestamp (ms)*/
 int32_t latitude; /*<  Latitude (deg * 1E7)*/
 int32_t longitude; /*<  Longitude (deg * 1E7)*/
 uint16_t altitude_msl; /*<  Altitude MSL (m * 10)*/
 uint16_t ground_speed; /*<  Ground speed (m/s * 10)*/
 uint8_t satellites; /*<  Number of satellites visible*/
} mavlink_crow_gps_t;

#define MAVLINK_MSG_ID_CROW_GPS_LEN 17
#define MAVLINK_MSG_ID_CROW_GPS_MIN_LEN 17
#define MAVLINK_MSG_ID_203_LEN 17
#define MAVLINK_MSG_ID_203_MIN_LEN 17

#define MAVLINK_MSG_ID_CROW_GPS_CRC 134
#define MAVLINK_MSG_ID_203_CRC 134



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_CROW_GPS { \
    203, \
    "CROW_GPS", \
    6, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_crow_gps_t, time_boot_ms) }, \
         { "latitude", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_crow_gps_t, latitude) }, \
         { "longitude", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_crow_gps_t, longitude) }, \
         { "altitude_msl", NULL, MAVLINK_TYPE_UINT16_T, 0, 12, offsetof(mavlink_crow_gps_t, altitude_msl) }, \
         { "ground_speed", NULL, MAVLINK_TYPE_UINT16_T, 0, 14, offsetof(mavlink_crow_gps_t, ground_speed) }, \
         { "satellites", NULL, MAVLINK_TYPE_UINT8_T, 0, 16, offsetof(mavlink_crow_gps_t, satellites) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_CROW_GPS { \
    "CROW_GPS", \
    6, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_crow_gps_t, time_boot_ms) }, \
         { "latitude", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_crow_gps_t, latitude) }, \
         { "longitude", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_crow_gps_t, longitude) }, \
         { "altitude_msl", NULL, MAVLINK_TYPE_UINT16_T, 0, 12, offsetof(mavlink_crow_gps_t, altitude_msl) }, \
         { "ground_speed", NULL, MAVLINK_TYPE_UINT16_T, 0, 14, offsetof(mavlink_crow_gps_t, ground_speed) }, \
         { "satellites", NULL, MAVLINK_TYPE_UINT8_T, 0, 16, offsetof(mavlink_crow_gps_t, satellites) }, \
         } \
}
#endif

/**
 * @brief Pack a crow_gps message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms  Timestamp (ms)
 * @param latitude  Latitude (deg * 1E7)
 * @param longitude  Longitude (deg * 1E7)
 * @param altitude_msl  Altitude MSL (m * 10)
 * @param ground_speed  Ground speed (m/s * 10)
 * @param satellites  Number of satellites visible
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_crow_gps_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_boot_ms, int32_t latitude, int32_t longitude, uint16_t altitude_msl, uint16_t ground_speed, uint8_t satellites)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CROW_GPS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, latitude);
    _mav_put_int32_t(buf, 8, longitude);
    _mav_put_uint16_t(buf, 12, altitude_msl);
    _mav_put_uint16_t(buf, 14, ground_speed);
    _mav_put_uint8_t(buf, 16, satellites);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CROW_GPS_LEN);
#else
    mavlink_crow_gps_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.latitude = latitude;
    packet.longitude = longitude;
    packet.altitude_msl = altitude_msl;
    packet.ground_speed = ground_speed;
    packet.satellites = satellites;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CROW_GPS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CROW_GPS;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CROW_GPS_MIN_LEN, MAVLINK_MSG_ID_CROW_GPS_LEN, MAVLINK_MSG_ID_CROW_GPS_CRC);
}

/**
 * @brief Pack a crow_gps message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms  Timestamp (ms)
 * @param latitude  Latitude (deg * 1E7)
 * @param longitude  Longitude (deg * 1E7)
 * @param altitude_msl  Altitude MSL (m * 10)
 * @param ground_speed  Ground speed (m/s * 10)
 * @param satellites  Number of satellites visible
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_crow_gps_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_boot_ms, int32_t latitude, int32_t longitude, uint16_t altitude_msl, uint16_t ground_speed, uint8_t satellites)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CROW_GPS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, latitude);
    _mav_put_int32_t(buf, 8, longitude);
    _mav_put_uint16_t(buf, 12, altitude_msl);
    _mav_put_uint16_t(buf, 14, ground_speed);
    _mav_put_uint8_t(buf, 16, satellites);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CROW_GPS_LEN);
#else
    mavlink_crow_gps_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.latitude = latitude;
    packet.longitude = longitude;
    packet.altitude_msl = altitude_msl;
    packet.ground_speed = ground_speed;
    packet.satellites = satellites;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CROW_GPS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CROW_GPS;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_CROW_GPS_MIN_LEN, MAVLINK_MSG_ID_CROW_GPS_LEN, MAVLINK_MSG_ID_CROW_GPS_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_CROW_GPS_MIN_LEN, MAVLINK_MSG_ID_CROW_GPS_LEN);
#endif
}

/**
 * @brief Pack a crow_gps message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_boot_ms  Timestamp (ms)
 * @param latitude  Latitude (deg * 1E7)
 * @param longitude  Longitude (deg * 1E7)
 * @param altitude_msl  Altitude MSL (m * 10)
 * @param ground_speed  Ground speed (m/s * 10)
 * @param satellites  Number of satellites visible
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_crow_gps_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_boot_ms,int32_t latitude,int32_t longitude,uint16_t altitude_msl,uint16_t ground_speed,uint8_t satellites)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CROW_GPS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, latitude);
    _mav_put_int32_t(buf, 8, longitude);
    _mav_put_uint16_t(buf, 12, altitude_msl);
    _mav_put_uint16_t(buf, 14, ground_speed);
    _mav_put_uint8_t(buf, 16, satellites);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CROW_GPS_LEN);
#else
    mavlink_crow_gps_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.latitude = latitude;
    packet.longitude = longitude;
    packet.altitude_msl = altitude_msl;
    packet.ground_speed = ground_speed;
    packet.satellites = satellites;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CROW_GPS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CROW_GPS;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CROW_GPS_MIN_LEN, MAVLINK_MSG_ID_CROW_GPS_LEN, MAVLINK_MSG_ID_CROW_GPS_CRC);
}

/**
 * @brief Encode a crow_gps struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param crow_gps C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_crow_gps_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_crow_gps_t* crow_gps)
{
    return mavlink_msg_crow_gps_pack(system_id, component_id, msg, crow_gps->time_boot_ms, crow_gps->latitude, crow_gps->longitude, crow_gps->altitude_msl, crow_gps->ground_speed, crow_gps->satellites);
}

/**
 * @brief Encode a crow_gps struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param crow_gps C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_crow_gps_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_crow_gps_t* crow_gps)
{
    return mavlink_msg_crow_gps_pack_chan(system_id, component_id, chan, msg, crow_gps->time_boot_ms, crow_gps->latitude, crow_gps->longitude, crow_gps->altitude_msl, crow_gps->ground_speed, crow_gps->satellites);
}

/**
 * @brief Encode a crow_gps struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param crow_gps C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_crow_gps_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_crow_gps_t* crow_gps)
{
    return mavlink_msg_crow_gps_pack_status(system_id, component_id, _status, msg,  crow_gps->time_boot_ms, crow_gps->latitude, crow_gps->longitude, crow_gps->altitude_msl, crow_gps->ground_speed, crow_gps->satellites);
}

/**
 * @brief Send a crow_gps message
 * @param chan MAVLink channel to send the message
 *
 * @param time_boot_ms  Timestamp (ms)
 * @param latitude  Latitude (deg * 1E7)
 * @param longitude  Longitude (deg * 1E7)
 * @param altitude_msl  Altitude MSL (m * 10)
 * @param ground_speed  Ground speed (m/s * 10)
 * @param satellites  Number of satellites visible
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_crow_gps_send(mavlink_channel_t chan, uint32_t time_boot_ms, int32_t latitude, int32_t longitude, uint16_t altitude_msl, uint16_t ground_speed, uint8_t satellites)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CROW_GPS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, latitude);
    _mav_put_int32_t(buf, 8, longitude);
    _mav_put_uint16_t(buf, 12, altitude_msl);
    _mav_put_uint16_t(buf, 14, ground_speed);
    _mav_put_uint8_t(buf, 16, satellites);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CROW_GPS, buf, MAVLINK_MSG_ID_CROW_GPS_MIN_LEN, MAVLINK_MSG_ID_CROW_GPS_LEN, MAVLINK_MSG_ID_CROW_GPS_CRC);
#else
    mavlink_crow_gps_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.latitude = latitude;
    packet.longitude = longitude;
    packet.altitude_msl = altitude_msl;
    packet.ground_speed = ground_speed;
    packet.satellites = satellites;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CROW_GPS, (const char *)&packet, MAVLINK_MSG_ID_CROW_GPS_MIN_LEN, MAVLINK_MSG_ID_CROW_GPS_LEN, MAVLINK_MSG_ID_CROW_GPS_CRC);
#endif
}

/**
 * @brief Send a crow_gps message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_crow_gps_send_struct(mavlink_channel_t chan, const mavlink_crow_gps_t* crow_gps)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_crow_gps_send(chan, crow_gps->time_boot_ms, crow_gps->latitude, crow_gps->longitude, crow_gps->altitude_msl, crow_gps->ground_speed, crow_gps->satellites);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CROW_GPS, (const char *)crow_gps, MAVLINK_MSG_ID_CROW_GPS_MIN_LEN, MAVLINK_MSG_ID_CROW_GPS_LEN, MAVLINK_MSG_ID_CROW_GPS_CRC);
#endif
}

#if MAVLINK_MSG_ID_CROW_GPS_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_crow_gps_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_boot_ms, int32_t latitude, int32_t longitude, uint16_t altitude_msl, uint16_t ground_speed, uint8_t satellites)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, latitude);
    _mav_put_int32_t(buf, 8, longitude);
    _mav_put_uint16_t(buf, 12, altitude_msl);
    _mav_put_uint16_t(buf, 14, ground_speed);
    _mav_put_uint8_t(buf, 16, satellites);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CROW_GPS, buf, MAVLINK_MSG_ID_CROW_GPS_MIN_LEN, MAVLINK_MSG_ID_CROW_GPS_LEN, MAVLINK_MSG_ID_CROW_GPS_CRC);
#else
    mavlink_crow_gps_t *packet = (mavlink_crow_gps_t *)msgbuf;
    packet->time_boot_ms = time_boot_ms;
    packet->latitude = latitude;
    packet->longitude = longitude;
    packet->altitude_msl = altitude_msl;
    packet->ground_speed = ground_speed;
    packet->satellites = satellites;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CROW_GPS, (const char *)packet, MAVLINK_MSG_ID_CROW_GPS_MIN_LEN, MAVLINK_MSG_ID_CROW_GPS_LEN, MAVLINK_MSG_ID_CROW_GPS_CRC);
#endif
}
#endif

#endif

// MESSAGE CROW_GPS UNPACKING


/**
 * @brief Get field time_boot_ms from crow_gps message
 *
 * @return  Timestamp (ms)
 */
static inline uint32_t mavlink_msg_crow_gps_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field latitude from crow_gps message
 *
 * @return  Latitude (deg * 1E7)
 */
static inline int32_t mavlink_msg_crow_gps_get_latitude(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  4);
}

/**
 * @brief Get field longitude from crow_gps message
 *
 * @return  Longitude (deg * 1E7)
 */
static inline int32_t mavlink_msg_crow_gps_get_longitude(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  8);
}

/**
 * @brief Get field altitude_msl from crow_gps message
 *
 * @return  Altitude MSL (m * 10)
 */
static inline uint16_t mavlink_msg_crow_gps_get_altitude_msl(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  12);
}

/**
 * @brief Get field ground_speed from crow_gps message
 *
 * @return  Ground speed (m/s * 10)
 */
static inline uint16_t mavlink_msg_crow_gps_get_ground_speed(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  14);
}

/**
 * @brief Get field satellites from crow_gps message
 *
 * @return  Number of satellites visible
 */
static inline uint8_t mavlink_msg_crow_gps_get_satellites(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  16);
}

/**
 * @brief Decode a crow_gps message into a struct
 *
 * @param msg The message to decode
 * @param crow_gps C-struct to decode the message contents into
 */
static inline void mavlink_msg_crow_gps_decode(const mavlink_message_t* msg, mavlink_crow_gps_t* crow_gps)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    crow_gps->time_boot_ms = mavlink_msg_crow_gps_get_time_boot_ms(msg);
    crow_gps->latitude = mavlink_msg_crow_gps_get_latitude(msg);
    crow_gps->longitude = mavlink_msg_crow_gps_get_longitude(msg);
    crow_gps->altitude_msl = mavlink_msg_crow_gps_get_altitude_msl(msg);
    crow_gps->ground_speed = mavlink_msg_crow_gps_get_ground_speed(msg);
    crow_gps->satellites = mavlink_msg_crow_gps_get_satellites(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_CROW_GPS_LEN? msg->len : MAVLINK_MSG_ID_CROW_GPS_LEN;
        memset(crow_gps, 0, MAVLINK_MSG_ID_CROW_GPS_LEN);
    memcpy(crow_gps, _MAV_PAYLOAD(msg), len);
#endif
}
