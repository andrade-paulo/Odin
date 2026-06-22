#pragma once
// MESSAGE CROW_BARO PACKING

#define MAVLINK_MSG_ID_CROW_BARO 202


typedef struct __mavlink_crow_baro_t {
 uint32_t time_boot_ms; /*<  Timestamp (ms)*/
 int16_t pressure_delta; /*<  Pressure offset (Pa)*/
 int16_t temperature; /*<  Temperature (C * 100)*/
} mavlink_crow_baro_t;

#define MAVLINK_MSG_ID_CROW_BARO_LEN 8
#define MAVLINK_MSG_ID_CROW_BARO_MIN_LEN 8
#define MAVLINK_MSG_ID_202_LEN 8
#define MAVLINK_MSG_ID_202_MIN_LEN 8

#define MAVLINK_MSG_ID_CROW_BARO_CRC 89
#define MAVLINK_MSG_ID_202_CRC 89



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_CROW_BARO { \
    202, \
    "CROW_BARO", \
    3, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_crow_baro_t, time_boot_ms) }, \
         { "pressure_delta", NULL, MAVLINK_TYPE_INT16_T, 0, 4, offsetof(mavlink_crow_baro_t, pressure_delta) }, \
         { "temperature", NULL, MAVLINK_TYPE_INT16_T, 0, 6, offsetof(mavlink_crow_baro_t, temperature) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_CROW_BARO { \
    "CROW_BARO", \
    3, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_crow_baro_t, time_boot_ms) }, \
         { "pressure_delta", NULL, MAVLINK_TYPE_INT16_T, 0, 4, offsetof(mavlink_crow_baro_t, pressure_delta) }, \
         { "temperature", NULL, MAVLINK_TYPE_INT16_T, 0, 6, offsetof(mavlink_crow_baro_t, temperature) }, \
         } \
}
#endif

/**
 * @brief Pack a crow_baro message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms  Timestamp (ms)
 * @param pressure_delta  Pressure offset (Pa)
 * @param temperature  Temperature (C * 100)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_crow_baro_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_boot_ms, int16_t pressure_delta, int16_t temperature)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CROW_BARO_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int16_t(buf, 4, pressure_delta);
    _mav_put_int16_t(buf, 6, temperature);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CROW_BARO_LEN);
#else
    mavlink_crow_baro_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.pressure_delta = pressure_delta;
    packet.temperature = temperature;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CROW_BARO_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CROW_BARO;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CROW_BARO_MIN_LEN, MAVLINK_MSG_ID_CROW_BARO_LEN, MAVLINK_MSG_ID_CROW_BARO_CRC);
}

/**
 * @brief Pack a crow_baro message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms  Timestamp (ms)
 * @param pressure_delta  Pressure offset (Pa)
 * @param temperature  Temperature (C * 100)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_crow_baro_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_boot_ms, int16_t pressure_delta, int16_t temperature)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CROW_BARO_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int16_t(buf, 4, pressure_delta);
    _mav_put_int16_t(buf, 6, temperature);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CROW_BARO_LEN);
#else
    mavlink_crow_baro_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.pressure_delta = pressure_delta;
    packet.temperature = temperature;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CROW_BARO_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CROW_BARO;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_CROW_BARO_MIN_LEN, MAVLINK_MSG_ID_CROW_BARO_LEN, MAVLINK_MSG_ID_CROW_BARO_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_CROW_BARO_MIN_LEN, MAVLINK_MSG_ID_CROW_BARO_LEN);
#endif
}

/**
 * @brief Pack a crow_baro message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_boot_ms  Timestamp (ms)
 * @param pressure_delta  Pressure offset (Pa)
 * @param temperature  Temperature (C * 100)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_crow_baro_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_boot_ms,int16_t pressure_delta,int16_t temperature)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CROW_BARO_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int16_t(buf, 4, pressure_delta);
    _mav_put_int16_t(buf, 6, temperature);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CROW_BARO_LEN);
#else
    mavlink_crow_baro_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.pressure_delta = pressure_delta;
    packet.temperature = temperature;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CROW_BARO_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CROW_BARO;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CROW_BARO_MIN_LEN, MAVLINK_MSG_ID_CROW_BARO_LEN, MAVLINK_MSG_ID_CROW_BARO_CRC);
}

/**
 * @brief Encode a crow_baro struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param crow_baro C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_crow_baro_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_crow_baro_t* crow_baro)
{
    return mavlink_msg_crow_baro_pack(system_id, component_id, msg, crow_baro->time_boot_ms, crow_baro->pressure_delta, crow_baro->temperature);
}

/**
 * @brief Encode a crow_baro struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param crow_baro C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_crow_baro_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_crow_baro_t* crow_baro)
{
    return mavlink_msg_crow_baro_pack_chan(system_id, component_id, chan, msg, crow_baro->time_boot_ms, crow_baro->pressure_delta, crow_baro->temperature);
}

/**
 * @brief Encode a crow_baro struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param crow_baro C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_crow_baro_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_crow_baro_t* crow_baro)
{
    return mavlink_msg_crow_baro_pack_status(system_id, component_id, _status, msg,  crow_baro->time_boot_ms, crow_baro->pressure_delta, crow_baro->temperature);
}

/**
 * @brief Send a crow_baro message
 * @param chan MAVLink channel to send the message
 *
 * @param time_boot_ms  Timestamp (ms)
 * @param pressure_delta  Pressure offset (Pa)
 * @param temperature  Temperature (C * 100)
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_crow_baro_send(mavlink_channel_t chan, uint32_t time_boot_ms, int16_t pressure_delta, int16_t temperature)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CROW_BARO_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int16_t(buf, 4, pressure_delta);
    _mav_put_int16_t(buf, 6, temperature);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CROW_BARO, buf, MAVLINK_MSG_ID_CROW_BARO_MIN_LEN, MAVLINK_MSG_ID_CROW_BARO_LEN, MAVLINK_MSG_ID_CROW_BARO_CRC);
#else
    mavlink_crow_baro_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.pressure_delta = pressure_delta;
    packet.temperature = temperature;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CROW_BARO, (const char *)&packet, MAVLINK_MSG_ID_CROW_BARO_MIN_LEN, MAVLINK_MSG_ID_CROW_BARO_LEN, MAVLINK_MSG_ID_CROW_BARO_CRC);
#endif
}

/**
 * @brief Send a crow_baro message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_crow_baro_send_struct(mavlink_channel_t chan, const mavlink_crow_baro_t* crow_baro)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_crow_baro_send(chan, crow_baro->time_boot_ms, crow_baro->pressure_delta, crow_baro->temperature);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CROW_BARO, (const char *)crow_baro, MAVLINK_MSG_ID_CROW_BARO_MIN_LEN, MAVLINK_MSG_ID_CROW_BARO_LEN, MAVLINK_MSG_ID_CROW_BARO_CRC);
#endif
}

#if MAVLINK_MSG_ID_CROW_BARO_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_crow_baro_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_boot_ms, int16_t pressure_delta, int16_t temperature)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int16_t(buf, 4, pressure_delta);
    _mav_put_int16_t(buf, 6, temperature);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CROW_BARO, buf, MAVLINK_MSG_ID_CROW_BARO_MIN_LEN, MAVLINK_MSG_ID_CROW_BARO_LEN, MAVLINK_MSG_ID_CROW_BARO_CRC);
#else
    mavlink_crow_baro_t *packet = (mavlink_crow_baro_t *)msgbuf;
    packet->time_boot_ms = time_boot_ms;
    packet->pressure_delta = pressure_delta;
    packet->temperature = temperature;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CROW_BARO, (const char *)packet, MAVLINK_MSG_ID_CROW_BARO_MIN_LEN, MAVLINK_MSG_ID_CROW_BARO_LEN, MAVLINK_MSG_ID_CROW_BARO_CRC);
#endif
}
#endif

#endif

// MESSAGE CROW_BARO UNPACKING


/**
 * @brief Get field time_boot_ms from crow_baro message
 *
 * @return  Timestamp (ms)
 */
static inline uint32_t mavlink_msg_crow_baro_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field pressure_delta from crow_baro message
 *
 * @return  Pressure offset (Pa)
 */
static inline int16_t mavlink_msg_crow_baro_get_pressure_delta(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  4);
}

/**
 * @brief Get field temperature from crow_baro message
 *
 * @return  Temperature (C * 100)
 */
static inline int16_t mavlink_msg_crow_baro_get_temperature(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  6);
}

/**
 * @brief Decode a crow_baro message into a struct
 *
 * @param msg The message to decode
 * @param crow_baro C-struct to decode the message contents into
 */
static inline void mavlink_msg_crow_baro_decode(const mavlink_message_t* msg, mavlink_crow_baro_t* crow_baro)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    crow_baro->time_boot_ms = mavlink_msg_crow_baro_get_time_boot_ms(msg);
    crow_baro->pressure_delta = mavlink_msg_crow_baro_get_pressure_delta(msg);
    crow_baro->temperature = mavlink_msg_crow_baro_get_temperature(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_CROW_BARO_LEN? msg->len : MAVLINK_MSG_ID_CROW_BARO_LEN;
        memset(crow_baro, 0, MAVLINK_MSG_ID_CROW_BARO_LEN);
    memcpy(crow_baro, _MAV_PAYLOAD(msg), len);
#endif
}
