#pragma once
// MESSAGE CUSTOM_HEALTH PACKING

#define MAVLINK_MSG_ID_CUSTOM_HEALTH 152


typedef struct __mavlink_custom_health_t {
 uint32_t time_boot_ms; /*< [ms] Timestamp (milliseconds since system boot)*/
 uint16_t vbat_mv; /*< [mV] Battery voltage (millivolts)*/
 uint8_t status_mask; /*<  Bitmask of system statuses*/
} mavlink_custom_health_t;

#define MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN 7
#define MAVLINK_MSG_ID_CUSTOM_HEALTH_MIN_LEN 7
#define MAVLINK_MSG_ID_152_LEN 7
#define MAVLINK_MSG_ID_152_MIN_LEN 7

#define MAVLINK_MSG_ID_CUSTOM_HEALTH_CRC 128
#define MAVLINK_MSG_ID_152_CRC 128



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_CUSTOM_HEALTH { \
    152, \
    "CUSTOM_HEALTH", \
    3, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_custom_health_t, time_boot_ms) }, \
         { "vbat_mv", NULL, MAVLINK_TYPE_UINT16_T, 0, 4, offsetof(mavlink_custom_health_t, vbat_mv) }, \
         { "status_mask", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_custom_health_t, status_mask) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_CUSTOM_HEALTH { \
    "CUSTOM_HEALTH", \
    3, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_custom_health_t, time_boot_ms) }, \
         { "vbat_mv", NULL, MAVLINK_TYPE_UINT16_T, 0, 4, offsetof(mavlink_custom_health_t, vbat_mv) }, \
         { "status_mask", NULL, MAVLINK_TYPE_UINT8_T, 0, 6, offsetof(mavlink_custom_health_t, status_mask) }, \
         } \
}
#endif

/**
 * @brief Pack a custom_health message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp (milliseconds since system boot)
 * @param vbat_mv [mV] Battery voltage (millivolts)
 * @param status_mask  Bitmask of system statuses
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_custom_health_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_boot_ms, uint16_t vbat_mv, uint8_t status_mask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint16_t(buf, 4, vbat_mv);
    _mav_put_uint8_t(buf, 6, status_mask);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN);
#else
    mavlink_custom_health_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.vbat_mv = vbat_mv;
    packet.status_mask = status_mask;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CUSTOM_HEALTH;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CUSTOM_HEALTH_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN, MAVLINK_MSG_ID_CUSTOM_HEALTH_CRC);
}

/**
 * @brief Pack a custom_health message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp (milliseconds since system boot)
 * @param vbat_mv [mV] Battery voltage (millivolts)
 * @param status_mask  Bitmask of system statuses
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_custom_health_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_boot_ms, uint16_t vbat_mv, uint8_t status_mask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint16_t(buf, 4, vbat_mv);
    _mav_put_uint8_t(buf, 6, status_mask);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN);
#else
    mavlink_custom_health_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.vbat_mv = vbat_mv;
    packet.status_mask = status_mask;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CUSTOM_HEALTH;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_CUSTOM_HEALTH_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN, MAVLINK_MSG_ID_CUSTOM_HEALTH_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_CUSTOM_HEALTH_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN);
#endif
}

/**
 * @brief Pack a custom_health message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_boot_ms [ms] Timestamp (milliseconds since system boot)
 * @param vbat_mv [mV] Battery voltage (millivolts)
 * @param status_mask  Bitmask of system statuses
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_custom_health_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_boot_ms,uint16_t vbat_mv,uint8_t status_mask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint16_t(buf, 4, vbat_mv);
    _mav_put_uint8_t(buf, 6, status_mask);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN);
#else
    mavlink_custom_health_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.vbat_mv = vbat_mv;
    packet.status_mask = status_mask;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CUSTOM_HEALTH;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CUSTOM_HEALTH_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN, MAVLINK_MSG_ID_CUSTOM_HEALTH_CRC);
}

/**
 * @brief Encode a custom_health struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param custom_health C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_custom_health_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_custom_health_t* custom_health)
{
    return mavlink_msg_custom_health_pack(system_id, component_id, msg, custom_health->time_boot_ms, custom_health->vbat_mv, custom_health->status_mask);
}

/**
 * @brief Encode a custom_health struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param custom_health C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_custom_health_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_custom_health_t* custom_health)
{
    return mavlink_msg_custom_health_pack_chan(system_id, component_id, chan, msg, custom_health->time_boot_ms, custom_health->vbat_mv, custom_health->status_mask);
}

/**
 * @brief Encode a custom_health struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param custom_health C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_custom_health_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_custom_health_t* custom_health)
{
    return mavlink_msg_custom_health_pack_status(system_id, component_id, _status, msg,  custom_health->time_boot_ms, custom_health->vbat_mv, custom_health->status_mask);
}

/**
 * @brief Send a custom_health message
 * @param chan MAVLink channel to send the message
 *
 * @param time_boot_ms [ms] Timestamp (milliseconds since system boot)
 * @param vbat_mv [mV] Battery voltage (millivolts)
 * @param status_mask  Bitmask of system statuses
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_custom_health_send(mavlink_channel_t chan, uint32_t time_boot_ms, uint16_t vbat_mv, uint8_t status_mask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint16_t(buf, 4, vbat_mv);
    _mav_put_uint8_t(buf, 6, status_mask);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUSTOM_HEALTH, buf, MAVLINK_MSG_ID_CUSTOM_HEALTH_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN, MAVLINK_MSG_ID_CUSTOM_HEALTH_CRC);
#else
    mavlink_custom_health_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.vbat_mv = vbat_mv;
    packet.status_mask = status_mask;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUSTOM_HEALTH, (const char *)&packet, MAVLINK_MSG_ID_CUSTOM_HEALTH_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN, MAVLINK_MSG_ID_CUSTOM_HEALTH_CRC);
#endif
}

/**
 * @brief Send a custom_health message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_custom_health_send_struct(mavlink_channel_t chan, const mavlink_custom_health_t* custom_health)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_custom_health_send(chan, custom_health->time_boot_ms, custom_health->vbat_mv, custom_health->status_mask);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUSTOM_HEALTH, (const char *)custom_health, MAVLINK_MSG_ID_CUSTOM_HEALTH_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN, MAVLINK_MSG_ID_CUSTOM_HEALTH_CRC);
#endif
}

#if MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_custom_health_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_boot_ms, uint16_t vbat_mv, uint8_t status_mask)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint16_t(buf, 4, vbat_mv);
    _mav_put_uint8_t(buf, 6, status_mask);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUSTOM_HEALTH, buf, MAVLINK_MSG_ID_CUSTOM_HEALTH_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN, MAVLINK_MSG_ID_CUSTOM_HEALTH_CRC);
#else
    mavlink_custom_health_t *packet = (mavlink_custom_health_t *)msgbuf;
    packet->time_boot_ms = time_boot_ms;
    packet->vbat_mv = vbat_mv;
    packet->status_mask = status_mask;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUSTOM_HEALTH, (const char *)packet, MAVLINK_MSG_ID_CUSTOM_HEALTH_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN, MAVLINK_MSG_ID_CUSTOM_HEALTH_CRC);
#endif
}
#endif

#endif

// MESSAGE CUSTOM_HEALTH UNPACKING


/**
 * @brief Get field time_boot_ms from custom_health message
 *
 * @return [ms] Timestamp (milliseconds since system boot)
 */
static inline uint32_t mavlink_msg_custom_health_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field vbat_mv from custom_health message
 *
 * @return [mV] Battery voltage (millivolts)
 */
static inline uint16_t mavlink_msg_custom_health_get_vbat_mv(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  4);
}

/**
 * @brief Get field status_mask from custom_health message
 *
 * @return  Bitmask of system statuses
 */
static inline uint8_t mavlink_msg_custom_health_get_status_mask(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  6);
}

/**
 * @brief Decode a custom_health message into a struct
 *
 * @param msg The message to decode
 * @param custom_health C-struct to decode the message contents into
 */
static inline void mavlink_msg_custom_health_decode(const mavlink_message_t* msg, mavlink_custom_health_t* custom_health)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    custom_health->time_boot_ms = mavlink_msg_custom_health_get_time_boot_ms(msg);
    custom_health->vbat_mv = mavlink_msg_custom_health_get_vbat_mv(msg);
    custom_health->status_mask = mavlink_msg_custom_health_get_status_mask(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN? msg->len : MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN;
        memset(custom_health, 0, MAVLINK_MSG_ID_CUSTOM_HEALTH_LEN);
    memcpy(custom_health, _MAV_PAYLOAD(msg), len);
#endif
}
