#pragma once
// MESSAGE CUSTOM_ATTITUDE PACKING

#define MAVLINK_MSG_ID_CUSTOM_ATTITUDE 150


typedef struct __mavlink_custom_attitude_t {
 uint32_t time_boot_ms; /*< [ms] Timestamp (milliseconds since system boot)*/
 int16_t accel_x_mg; /*< [mg] X acceleration (milli-G)*/
 int16_t accel_y_mg; /*< [mg] Y acceleration (milli-G)*/
 int16_t accel_z_mg; /*< [mg] Z acceleration (milli-G)*/
 int16_t gyro_x_mr; /*< [mrad/s] X angular rate (milli-radians/s)*/
 int16_t gyro_y_mr; /*< [mrad/s] Y angular rate (milli-radians/s)*/
 int16_t gyro_z_mr; /*< [mrad/s] Z angular rate (milli-radians/s)*/
} mavlink_custom_attitude_t;

#define MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN 16
#define MAVLINK_MSG_ID_CUSTOM_ATTITUDE_MIN_LEN 16
#define MAVLINK_MSG_ID_150_LEN 16
#define MAVLINK_MSG_ID_150_MIN_LEN 16

#define MAVLINK_MSG_ID_CUSTOM_ATTITUDE_CRC 0
#define MAVLINK_MSG_ID_150_CRC 0



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_CUSTOM_ATTITUDE { \
    150, \
    "CUSTOM_ATTITUDE", \
    7, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_custom_attitude_t, time_boot_ms) }, \
         { "accel_x_mg", NULL, MAVLINK_TYPE_INT16_T, 0, 4, offsetof(mavlink_custom_attitude_t, accel_x_mg) }, \
         { "accel_y_mg", NULL, MAVLINK_TYPE_INT16_T, 0, 6, offsetof(mavlink_custom_attitude_t, accel_y_mg) }, \
         { "accel_z_mg", NULL, MAVLINK_TYPE_INT16_T, 0, 8, offsetof(mavlink_custom_attitude_t, accel_z_mg) }, \
         { "gyro_x_mr", NULL, MAVLINK_TYPE_INT16_T, 0, 10, offsetof(mavlink_custom_attitude_t, gyro_x_mr) }, \
         { "gyro_y_mr", NULL, MAVLINK_TYPE_INT16_T, 0, 12, offsetof(mavlink_custom_attitude_t, gyro_y_mr) }, \
         { "gyro_z_mr", NULL, MAVLINK_TYPE_INT16_T, 0, 14, offsetof(mavlink_custom_attitude_t, gyro_z_mr) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_CUSTOM_ATTITUDE { \
    "CUSTOM_ATTITUDE", \
    7, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_custom_attitude_t, time_boot_ms) }, \
         { "accel_x_mg", NULL, MAVLINK_TYPE_INT16_T, 0, 4, offsetof(mavlink_custom_attitude_t, accel_x_mg) }, \
         { "accel_y_mg", NULL, MAVLINK_TYPE_INT16_T, 0, 6, offsetof(mavlink_custom_attitude_t, accel_y_mg) }, \
         { "accel_z_mg", NULL, MAVLINK_TYPE_INT16_T, 0, 8, offsetof(mavlink_custom_attitude_t, accel_z_mg) }, \
         { "gyro_x_mr", NULL, MAVLINK_TYPE_INT16_T, 0, 10, offsetof(mavlink_custom_attitude_t, gyro_x_mr) }, \
         { "gyro_y_mr", NULL, MAVLINK_TYPE_INT16_T, 0, 12, offsetof(mavlink_custom_attitude_t, gyro_y_mr) }, \
         { "gyro_z_mr", NULL, MAVLINK_TYPE_INT16_T, 0, 14, offsetof(mavlink_custom_attitude_t, gyro_z_mr) }, \
         } \
}
#endif

/**
 * @brief Pack a custom_attitude message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp (milliseconds since system boot)
 * @param accel_x_mg [mg] X acceleration (milli-G)
 * @param accel_y_mg [mg] Y acceleration (milli-G)
 * @param accel_z_mg [mg] Z acceleration (milli-G)
 * @param gyro_x_mr [mrad/s] X angular rate (milli-radians/s)
 * @param gyro_y_mr [mrad/s] Y angular rate (milli-radians/s)
 * @param gyro_z_mr [mrad/s] Z angular rate (milli-radians/s)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_custom_attitude_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_boot_ms, int16_t accel_x_mg, int16_t accel_y_mg, int16_t accel_z_mg, int16_t gyro_x_mr, int16_t gyro_y_mr, int16_t gyro_z_mr)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int16_t(buf, 4, accel_x_mg);
    _mav_put_int16_t(buf, 6, accel_y_mg);
    _mav_put_int16_t(buf, 8, accel_z_mg);
    _mav_put_int16_t(buf, 10, gyro_x_mr);
    _mav_put_int16_t(buf, 12, gyro_y_mr);
    _mav_put_int16_t(buf, 14, gyro_z_mr);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN);
#else
    mavlink_custom_attitude_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.accel_x_mg = accel_x_mg;
    packet.accel_y_mg = accel_y_mg;
    packet.accel_z_mg = accel_z_mg;
    packet.gyro_x_mr = gyro_x_mr;
    packet.gyro_y_mr = gyro_y_mr;
    packet.gyro_z_mr = gyro_z_mr;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CUSTOM_ATTITUDE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_CRC);
}

/**
 * @brief Pack a custom_attitude message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp (milliseconds since system boot)
 * @param accel_x_mg [mg] X acceleration (milli-G)
 * @param accel_y_mg [mg] Y acceleration (milli-G)
 * @param accel_z_mg [mg] Z acceleration (milli-G)
 * @param gyro_x_mr [mrad/s] X angular rate (milli-radians/s)
 * @param gyro_y_mr [mrad/s] Y angular rate (milli-radians/s)
 * @param gyro_z_mr [mrad/s] Z angular rate (milli-radians/s)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_custom_attitude_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_boot_ms, int16_t accel_x_mg, int16_t accel_y_mg, int16_t accel_z_mg, int16_t gyro_x_mr, int16_t gyro_y_mr, int16_t gyro_z_mr)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int16_t(buf, 4, accel_x_mg);
    _mav_put_int16_t(buf, 6, accel_y_mg);
    _mav_put_int16_t(buf, 8, accel_z_mg);
    _mav_put_int16_t(buf, 10, gyro_x_mr);
    _mav_put_int16_t(buf, 12, gyro_y_mr);
    _mav_put_int16_t(buf, 14, gyro_z_mr);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN);
#else
    mavlink_custom_attitude_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.accel_x_mg = accel_x_mg;
    packet.accel_y_mg = accel_y_mg;
    packet.accel_z_mg = accel_z_mg;
    packet.gyro_x_mr = gyro_x_mr;
    packet.gyro_y_mr = gyro_y_mr;
    packet.gyro_z_mr = gyro_z_mr;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CUSTOM_ATTITUDE;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN);
#endif
}

/**
 * @brief Pack a custom_attitude message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_boot_ms [ms] Timestamp (milliseconds since system boot)
 * @param accel_x_mg [mg] X acceleration (milli-G)
 * @param accel_y_mg [mg] Y acceleration (milli-G)
 * @param accel_z_mg [mg] Z acceleration (milli-G)
 * @param gyro_x_mr [mrad/s] X angular rate (milli-radians/s)
 * @param gyro_y_mr [mrad/s] Y angular rate (milli-radians/s)
 * @param gyro_z_mr [mrad/s] Z angular rate (milli-radians/s)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_custom_attitude_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_boot_ms,int16_t accel_x_mg,int16_t accel_y_mg,int16_t accel_z_mg,int16_t gyro_x_mr,int16_t gyro_y_mr,int16_t gyro_z_mr)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int16_t(buf, 4, accel_x_mg);
    _mav_put_int16_t(buf, 6, accel_y_mg);
    _mav_put_int16_t(buf, 8, accel_z_mg);
    _mav_put_int16_t(buf, 10, gyro_x_mr);
    _mav_put_int16_t(buf, 12, gyro_y_mr);
    _mav_put_int16_t(buf, 14, gyro_z_mr);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN);
#else
    mavlink_custom_attitude_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.accel_x_mg = accel_x_mg;
    packet.accel_y_mg = accel_y_mg;
    packet.accel_z_mg = accel_z_mg;
    packet.gyro_x_mr = gyro_x_mr;
    packet.gyro_y_mr = gyro_y_mr;
    packet.gyro_z_mr = gyro_z_mr;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CUSTOM_ATTITUDE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_CRC);
}

/**
 * @brief Encode a custom_attitude struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param custom_attitude C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_custom_attitude_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_custom_attitude_t* custom_attitude)
{
    return mavlink_msg_custom_attitude_pack(system_id, component_id, msg, custom_attitude->time_boot_ms, custom_attitude->accel_x_mg, custom_attitude->accel_y_mg, custom_attitude->accel_z_mg, custom_attitude->gyro_x_mr, custom_attitude->gyro_y_mr, custom_attitude->gyro_z_mr);
}

/**
 * @brief Encode a custom_attitude struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param custom_attitude C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_custom_attitude_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_custom_attitude_t* custom_attitude)
{
    return mavlink_msg_custom_attitude_pack_chan(system_id, component_id, chan, msg, custom_attitude->time_boot_ms, custom_attitude->accel_x_mg, custom_attitude->accel_y_mg, custom_attitude->accel_z_mg, custom_attitude->gyro_x_mr, custom_attitude->gyro_y_mr, custom_attitude->gyro_z_mr);
}

/**
 * @brief Encode a custom_attitude struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param custom_attitude C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_custom_attitude_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_custom_attitude_t* custom_attitude)
{
    return mavlink_msg_custom_attitude_pack_status(system_id, component_id, _status, msg,  custom_attitude->time_boot_ms, custom_attitude->accel_x_mg, custom_attitude->accel_y_mg, custom_attitude->accel_z_mg, custom_attitude->gyro_x_mr, custom_attitude->gyro_y_mr, custom_attitude->gyro_z_mr);
}

/**
 * @brief Send a custom_attitude message
 * @param chan MAVLink channel to send the message
 *
 * @param time_boot_ms [ms] Timestamp (milliseconds since system boot)
 * @param accel_x_mg [mg] X acceleration (milli-G)
 * @param accel_y_mg [mg] Y acceleration (milli-G)
 * @param accel_z_mg [mg] Z acceleration (milli-G)
 * @param gyro_x_mr [mrad/s] X angular rate (milli-radians/s)
 * @param gyro_y_mr [mrad/s] Y angular rate (milli-radians/s)
 * @param gyro_z_mr [mrad/s] Z angular rate (milli-radians/s)
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_custom_attitude_send(mavlink_channel_t chan, uint32_t time_boot_ms, int16_t accel_x_mg, int16_t accel_y_mg, int16_t accel_z_mg, int16_t gyro_x_mr, int16_t gyro_y_mr, int16_t gyro_z_mr)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int16_t(buf, 4, accel_x_mg);
    _mav_put_int16_t(buf, 6, accel_y_mg);
    _mav_put_int16_t(buf, 8, accel_z_mg);
    _mav_put_int16_t(buf, 10, gyro_x_mr);
    _mav_put_int16_t(buf, 12, gyro_y_mr);
    _mav_put_int16_t(buf, 14, gyro_z_mr);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUSTOM_ATTITUDE, buf, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_CRC);
#else
    mavlink_custom_attitude_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.accel_x_mg = accel_x_mg;
    packet.accel_y_mg = accel_y_mg;
    packet.accel_z_mg = accel_z_mg;
    packet.gyro_x_mr = gyro_x_mr;
    packet.gyro_y_mr = gyro_y_mr;
    packet.gyro_z_mr = gyro_z_mr;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUSTOM_ATTITUDE, (const char *)&packet, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_CRC);
#endif
}

/**
 * @brief Send a custom_attitude message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_custom_attitude_send_struct(mavlink_channel_t chan, const mavlink_custom_attitude_t* custom_attitude)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_custom_attitude_send(chan, custom_attitude->time_boot_ms, custom_attitude->accel_x_mg, custom_attitude->accel_y_mg, custom_attitude->accel_z_mg, custom_attitude->gyro_x_mr, custom_attitude->gyro_y_mr, custom_attitude->gyro_z_mr);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUSTOM_ATTITUDE, (const char *)custom_attitude, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_CRC);
#endif
}

#if MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_custom_attitude_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_boot_ms, int16_t accel_x_mg, int16_t accel_y_mg, int16_t accel_z_mg, int16_t gyro_x_mr, int16_t gyro_y_mr, int16_t gyro_z_mr)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int16_t(buf, 4, accel_x_mg);
    _mav_put_int16_t(buf, 6, accel_y_mg);
    _mav_put_int16_t(buf, 8, accel_z_mg);
    _mav_put_int16_t(buf, 10, gyro_x_mr);
    _mav_put_int16_t(buf, 12, gyro_y_mr);
    _mav_put_int16_t(buf, 14, gyro_z_mr);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUSTOM_ATTITUDE, buf, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_CRC);
#else
    mavlink_custom_attitude_t *packet = (mavlink_custom_attitude_t *)msgbuf;
    packet->time_boot_ms = time_boot_ms;
    packet->accel_x_mg = accel_x_mg;
    packet->accel_y_mg = accel_y_mg;
    packet->accel_z_mg = accel_z_mg;
    packet->gyro_x_mr = gyro_x_mr;
    packet->gyro_y_mr = gyro_y_mr;
    packet->gyro_z_mr = gyro_z_mr;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CUSTOM_ATTITUDE, (const char *)packet, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_MIN_LEN, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_CRC);
#endif
}
#endif

#endif

// MESSAGE CUSTOM_ATTITUDE UNPACKING


/**
 * @brief Get field time_boot_ms from custom_attitude message
 *
 * @return [ms] Timestamp (milliseconds since system boot)
 */
static inline uint32_t mavlink_msg_custom_attitude_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field accel_x_mg from custom_attitude message
 *
 * @return [mg] X acceleration (milli-G)
 */
static inline int16_t mavlink_msg_custom_attitude_get_accel_x_mg(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  4);
}

/**
 * @brief Get field accel_y_mg from custom_attitude message
 *
 * @return [mg] Y acceleration (milli-G)
 */
static inline int16_t mavlink_msg_custom_attitude_get_accel_y_mg(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  6);
}

/**
 * @brief Get field accel_z_mg from custom_attitude message
 *
 * @return [mg] Z acceleration (milli-G)
 */
static inline int16_t mavlink_msg_custom_attitude_get_accel_z_mg(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  8);
}

/**
 * @brief Get field gyro_x_mr from custom_attitude message
 *
 * @return [mrad/s] X angular rate (milli-radians/s)
 */
static inline int16_t mavlink_msg_custom_attitude_get_gyro_x_mr(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  10);
}

/**
 * @brief Get field gyro_y_mr from custom_attitude message
 *
 * @return [mrad/s] Y angular rate (milli-radians/s)
 */
static inline int16_t mavlink_msg_custom_attitude_get_gyro_y_mr(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  12);
}

/**
 * @brief Get field gyro_z_mr from custom_attitude message
 *
 * @return [mrad/s] Z angular rate (milli-radians/s)
 */
static inline int16_t mavlink_msg_custom_attitude_get_gyro_z_mr(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  14);
}

/**
 * @brief Decode a custom_attitude message into a struct
 *
 * @param msg The message to decode
 * @param custom_attitude C-struct to decode the message contents into
 */
static inline void mavlink_msg_custom_attitude_decode(const mavlink_message_t* msg, mavlink_custom_attitude_t* custom_attitude)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    custom_attitude->time_boot_ms = mavlink_msg_custom_attitude_get_time_boot_ms(msg);
    custom_attitude->accel_x_mg = mavlink_msg_custom_attitude_get_accel_x_mg(msg);
    custom_attitude->accel_y_mg = mavlink_msg_custom_attitude_get_accel_y_mg(msg);
    custom_attitude->accel_z_mg = mavlink_msg_custom_attitude_get_accel_z_mg(msg);
    custom_attitude->gyro_x_mr = mavlink_msg_custom_attitude_get_gyro_x_mr(msg);
    custom_attitude->gyro_y_mr = mavlink_msg_custom_attitude_get_gyro_y_mr(msg);
    custom_attitude->gyro_z_mr = mavlink_msg_custom_attitude_get_gyro_z_mr(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN? msg->len : MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN;
        memset(custom_attitude, 0, MAVLINK_MSG_ID_CUSTOM_ATTITUDE_LEN);
    memcpy(custom_attitude, _MAV_PAYLOAD(msg), len);
#endif
}
