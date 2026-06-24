#pragma once
// MESSAGE CROW_IMU PACKING

#define MAVLINK_MSG_ID_CROW_IMU 201


typedef struct __mavlink_crow_imu_t {
 uint32_t time_boot_ms; /*<  Timestamp (ms)*/
 int16_t accel_x; /*<  X accel (m/s^2 * 100)*/
 int16_t accel_y; /*<  Y accel (m/s^2 * 100)*/
 int16_t accel_z; /*<  Z accel (m/s^2 * 100)*/
 int16_t gyro_x; /*<  X gyro (deg/s * 10)*/
 int16_t gyro_y; /*<  Y gyro (deg/s * 10)*/
 int16_t gyro_z; /*<  Z gyro (deg/s * 10)*/
 int16_t mag_x; /*<  X mag (raw LSB)*/
 int16_t mag_y; /*<  Y mag (raw LSB)*/
 int16_t mag_z; /*<  Z mag (raw LSB)*/
} mavlink_crow_imu_t;

#define MAVLINK_MSG_ID_CROW_IMU_LEN 22
#define MAVLINK_MSG_ID_CROW_IMU_MIN_LEN 22
#define MAVLINK_MSG_ID_201_LEN 22
#define MAVLINK_MSG_ID_201_MIN_LEN 22

#define MAVLINK_MSG_ID_CROW_IMU_CRC 103
#define MAVLINK_MSG_ID_201_CRC 103



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_CROW_IMU { \
    201, \
    "CROW_IMU", \
    10, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_crow_imu_t, time_boot_ms) }, \
         { "accel_x", NULL, MAVLINK_TYPE_INT16_T, 0, 4, offsetof(mavlink_crow_imu_t, accel_x) }, \
         { "accel_y", NULL, MAVLINK_TYPE_INT16_T, 0, 6, offsetof(mavlink_crow_imu_t, accel_y) }, \
         { "accel_z", NULL, MAVLINK_TYPE_INT16_T, 0, 8, offsetof(mavlink_crow_imu_t, accel_z) }, \
         { "gyro_x", NULL, MAVLINK_TYPE_INT16_T, 0, 10, offsetof(mavlink_crow_imu_t, gyro_x) }, \
         { "gyro_y", NULL, MAVLINK_TYPE_INT16_T, 0, 12, offsetof(mavlink_crow_imu_t, gyro_y) }, \
         { "gyro_z", NULL, MAVLINK_TYPE_INT16_T, 0, 14, offsetof(mavlink_crow_imu_t, gyro_z) }, \
         { "mag_x", NULL, MAVLINK_TYPE_INT16_T, 0, 16, offsetof(mavlink_crow_imu_t, mag_x) }, \
         { "mag_y", NULL, MAVLINK_TYPE_INT16_T, 0, 18, offsetof(mavlink_crow_imu_t, mag_y) }, \
         { "mag_z", NULL, MAVLINK_TYPE_INT16_T, 0, 20, offsetof(mavlink_crow_imu_t, mag_z) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_CROW_IMU { \
    "CROW_IMU", \
    10, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_crow_imu_t, time_boot_ms) }, \
         { "accel_x", NULL, MAVLINK_TYPE_INT16_T, 0, 4, offsetof(mavlink_crow_imu_t, accel_x) }, \
         { "accel_y", NULL, MAVLINK_TYPE_INT16_T, 0, 6, offsetof(mavlink_crow_imu_t, accel_y) }, \
         { "accel_z", NULL, MAVLINK_TYPE_INT16_T, 0, 8, offsetof(mavlink_crow_imu_t, accel_z) }, \
         { "gyro_x", NULL, MAVLINK_TYPE_INT16_T, 0, 10, offsetof(mavlink_crow_imu_t, gyro_x) }, \
         { "gyro_y", NULL, MAVLINK_TYPE_INT16_T, 0, 12, offsetof(mavlink_crow_imu_t, gyro_y) }, \
         { "gyro_z", NULL, MAVLINK_TYPE_INT16_T, 0, 14, offsetof(mavlink_crow_imu_t, gyro_z) }, \
         { "mag_x", NULL, MAVLINK_TYPE_INT16_T, 0, 16, offsetof(mavlink_crow_imu_t, mag_x) }, \
         { "mag_y", NULL, MAVLINK_TYPE_INT16_T, 0, 18, offsetof(mavlink_crow_imu_t, mag_y) }, \
         { "mag_z", NULL, MAVLINK_TYPE_INT16_T, 0, 20, offsetof(mavlink_crow_imu_t, mag_z) }, \
         } \
}
#endif

/**
 * @brief Pack a crow_imu message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms  Timestamp (ms)
 * @param accel_x  X accel (m/s^2 * 100)
 * @param accel_y  Y accel (m/s^2 * 100)
 * @param accel_z  Z accel (m/s^2 * 100)
 * @param gyro_x  X gyro (deg/s * 10)
 * @param gyro_y  Y gyro (deg/s * 10)
 * @param gyro_z  Z gyro (deg/s * 10)
 * @param mag_x  X mag (raw LSB)
 * @param mag_y  Y mag (raw LSB)
 * @param mag_z  Z mag (raw LSB)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_crow_imu_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_boot_ms, int16_t accel_x, int16_t accel_y, int16_t accel_z, int16_t gyro_x, int16_t gyro_y, int16_t gyro_z, int16_t mag_x, int16_t mag_y, int16_t mag_z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CROW_IMU_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int16_t(buf, 4, accel_x);
    _mav_put_int16_t(buf, 6, accel_y);
    _mav_put_int16_t(buf, 8, accel_z);
    _mav_put_int16_t(buf, 10, gyro_x);
    _mav_put_int16_t(buf, 12, gyro_y);
    _mav_put_int16_t(buf, 14, gyro_z);
    _mav_put_int16_t(buf, 16, mag_x);
    _mav_put_int16_t(buf, 18, mag_y);
    _mav_put_int16_t(buf, 20, mag_z);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CROW_IMU_LEN);
#else
    mavlink_crow_imu_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.accel_x = accel_x;
    packet.accel_y = accel_y;
    packet.accel_z = accel_z;
    packet.gyro_x = gyro_x;
    packet.gyro_y = gyro_y;
    packet.gyro_z = gyro_z;
    packet.mag_x = mag_x;
    packet.mag_y = mag_y;
    packet.mag_z = mag_z;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CROW_IMU_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CROW_IMU;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CROW_IMU_MIN_LEN, MAVLINK_MSG_ID_CROW_IMU_LEN, MAVLINK_MSG_ID_CROW_IMU_CRC);
}

/**
 * @brief Pack a crow_imu message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms  Timestamp (ms)
 * @param accel_x  X accel (m/s^2 * 100)
 * @param accel_y  Y accel (m/s^2 * 100)
 * @param accel_z  Z accel (m/s^2 * 100)
 * @param gyro_x  X gyro (deg/s * 10)
 * @param gyro_y  Y gyro (deg/s * 10)
 * @param gyro_z  Z gyro (deg/s * 10)
 * @param mag_x  X mag (raw LSB)
 * @param mag_y  Y mag (raw LSB)
 * @param mag_z  Z mag (raw LSB)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_crow_imu_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_boot_ms, int16_t accel_x, int16_t accel_y, int16_t accel_z, int16_t gyro_x, int16_t gyro_y, int16_t gyro_z, int16_t mag_x, int16_t mag_y, int16_t mag_z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CROW_IMU_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int16_t(buf, 4, accel_x);
    _mav_put_int16_t(buf, 6, accel_y);
    _mav_put_int16_t(buf, 8, accel_z);
    _mav_put_int16_t(buf, 10, gyro_x);
    _mav_put_int16_t(buf, 12, gyro_y);
    _mav_put_int16_t(buf, 14, gyro_z);
    _mav_put_int16_t(buf, 16, mag_x);
    _mav_put_int16_t(buf, 18, mag_y);
    _mav_put_int16_t(buf, 20, mag_z);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CROW_IMU_LEN);
#else
    mavlink_crow_imu_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.accel_x = accel_x;
    packet.accel_y = accel_y;
    packet.accel_z = accel_z;
    packet.gyro_x = gyro_x;
    packet.gyro_y = gyro_y;
    packet.gyro_z = gyro_z;
    packet.mag_x = mag_x;
    packet.mag_y = mag_y;
    packet.mag_z = mag_z;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CROW_IMU_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CROW_IMU;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_CROW_IMU_MIN_LEN, MAVLINK_MSG_ID_CROW_IMU_LEN, MAVLINK_MSG_ID_CROW_IMU_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_CROW_IMU_MIN_LEN, MAVLINK_MSG_ID_CROW_IMU_LEN);
#endif
}

/**
 * @brief Pack a crow_imu message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_boot_ms  Timestamp (ms)
 * @param accel_x  X accel (m/s^2 * 100)
 * @param accel_y  Y accel (m/s^2 * 100)
 * @param accel_z  Z accel (m/s^2 * 100)
 * @param gyro_x  X gyro (deg/s * 10)
 * @param gyro_y  Y gyro (deg/s * 10)
 * @param gyro_z  Z gyro (deg/s * 10)
 * @param mag_x  X mag (raw LSB)
 * @param mag_y  Y mag (raw LSB)
 * @param mag_z  Z mag (raw LSB)
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_crow_imu_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_boot_ms,int16_t accel_x,int16_t accel_y,int16_t accel_z,int16_t gyro_x,int16_t gyro_y,int16_t gyro_z,int16_t mag_x,int16_t mag_y,int16_t mag_z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CROW_IMU_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int16_t(buf, 4, accel_x);
    _mav_put_int16_t(buf, 6, accel_y);
    _mav_put_int16_t(buf, 8, accel_z);
    _mav_put_int16_t(buf, 10, gyro_x);
    _mav_put_int16_t(buf, 12, gyro_y);
    _mav_put_int16_t(buf, 14, gyro_z);
    _mav_put_int16_t(buf, 16, mag_x);
    _mav_put_int16_t(buf, 18, mag_y);
    _mav_put_int16_t(buf, 20, mag_z);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CROW_IMU_LEN);
#else
    mavlink_crow_imu_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.accel_x = accel_x;
    packet.accel_y = accel_y;
    packet.accel_z = accel_z;
    packet.gyro_x = gyro_x;
    packet.gyro_y = gyro_y;
    packet.gyro_z = gyro_z;
    packet.mag_x = mag_x;
    packet.mag_y = mag_y;
    packet.mag_z = mag_z;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CROW_IMU_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_CROW_IMU;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CROW_IMU_MIN_LEN, MAVLINK_MSG_ID_CROW_IMU_LEN, MAVLINK_MSG_ID_CROW_IMU_CRC);
}

/**
 * @brief Encode a crow_imu struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param crow_imu C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_crow_imu_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_crow_imu_t* crow_imu)
{
    return mavlink_msg_crow_imu_pack(system_id, component_id, msg, crow_imu->time_boot_ms, crow_imu->accel_x, crow_imu->accel_y, crow_imu->accel_z, crow_imu->gyro_x, crow_imu->gyro_y, crow_imu->gyro_z, crow_imu->mag_x, crow_imu->mag_y, crow_imu->mag_z);
}

/**
 * @brief Encode a crow_imu struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param crow_imu C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_crow_imu_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_crow_imu_t* crow_imu)
{
    return mavlink_msg_crow_imu_pack_chan(system_id, component_id, chan, msg, crow_imu->time_boot_ms, crow_imu->accel_x, crow_imu->accel_y, crow_imu->accel_z, crow_imu->gyro_x, crow_imu->gyro_y, crow_imu->gyro_z, crow_imu->mag_x, crow_imu->mag_y, crow_imu->mag_z);
}

/**
 * @brief Encode a crow_imu struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param crow_imu C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_crow_imu_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_crow_imu_t* crow_imu)
{
    return mavlink_msg_crow_imu_pack_status(system_id, component_id, _status, msg,  crow_imu->time_boot_ms, crow_imu->accel_x, crow_imu->accel_y, crow_imu->accel_z, crow_imu->gyro_x, crow_imu->gyro_y, crow_imu->gyro_z, crow_imu->mag_x, crow_imu->mag_y, crow_imu->mag_z);
}

/**
 * @brief Send a crow_imu message
 * @param chan MAVLink channel to send the message
 *
 * @param time_boot_ms  Timestamp (ms)
 * @param accel_x  X accel (m/s^2 * 100)
 * @param accel_y  Y accel (m/s^2 * 100)
 * @param accel_z  Z accel (m/s^2 * 100)
 * @param gyro_x  X gyro (deg/s * 10)
 * @param gyro_y  Y gyro (deg/s * 10)
 * @param gyro_z  Z gyro (deg/s * 10)
 * @param mag_x  X mag (raw LSB)
 * @param mag_y  Y mag (raw LSB)
 * @param mag_z  Z mag (raw LSB)
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_crow_imu_send(mavlink_channel_t chan, uint32_t time_boot_ms, int16_t accel_x, int16_t accel_y, int16_t accel_z, int16_t gyro_x, int16_t gyro_y, int16_t gyro_z, int16_t mag_x, int16_t mag_y, int16_t mag_z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_CROW_IMU_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int16_t(buf, 4, accel_x);
    _mav_put_int16_t(buf, 6, accel_y);
    _mav_put_int16_t(buf, 8, accel_z);
    _mav_put_int16_t(buf, 10, gyro_x);
    _mav_put_int16_t(buf, 12, gyro_y);
    _mav_put_int16_t(buf, 14, gyro_z);
    _mav_put_int16_t(buf, 16, mag_x);
    _mav_put_int16_t(buf, 18, mag_y);
    _mav_put_int16_t(buf, 20, mag_z);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CROW_IMU, buf, MAVLINK_MSG_ID_CROW_IMU_MIN_LEN, MAVLINK_MSG_ID_CROW_IMU_LEN, MAVLINK_MSG_ID_CROW_IMU_CRC);
#else
    mavlink_crow_imu_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.accel_x = accel_x;
    packet.accel_y = accel_y;
    packet.accel_z = accel_z;
    packet.gyro_x = gyro_x;
    packet.gyro_y = gyro_y;
    packet.gyro_z = gyro_z;
    packet.mag_x = mag_x;
    packet.mag_y = mag_y;
    packet.mag_z = mag_z;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CROW_IMU, (const char *)&packet, MAVLINK_MSG_ID_CROW_IMU_MIN_LEN, MAVLINK_MSG_ID_CROW_IMU_LEN, MAVLINK_MSG_ID_CROW_IMU_CRC);
#endif
}

/**
 * @brief Send a crow_imu message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_crow_imu_send_struct(mavlink_channel_t chan, const mavlink_crow_imu_t* crow_imu)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_crow_imu_send(chan, crow_imu->time_boot_ms, crow_imu->accel_x, crow_imu->accel_y, crow_imu->accel_z, crow_imu->gyro_x, crow_imu->gyro_y, crow_imu->gyro_z, crow_imu->mag_x, crow_imu->mag_y, crow_imu->mag_z);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CROW_IMU, (const char *)crow_imu, MAVLINK_MSG_ID_CROW_IMU_MIN_LEN, MAVLINK_MSG_ID_CROW_IMU_LEN, MAVLINK_MSG_ID_CROW_IMU_CRC);
#endif
}

#if MAVLINK_MSG_ID_CROW_IMU_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_crow_imu_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_boot_ms, int16_t accel_x, int16_t accel_y, int16_t accel_z, int16_t gyro_x, int16_t gyro_y, int16_t gyro_z, int16_t mag_x, int16_t mag_y, int16_t mag_z)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int16_t(buf, 4, accel_x);
    _mav_put_int16_t(buf, 6, accel_y);
    _mav_put_int16_t(buf, 8, accel_z);
    _mav_put_int16_t(buf, 10, gyro_x);
    _mav_put_int16_t(buf, 12, gyro_y);
    _mav_put_int16_t(buf, 14, gyro_z);
    _mav_put_int16_t(buf, 16, mag_x);
    _mav_put_int16_t(buf, 18, mag_y);
    _mav_put_int16_t(buf, 20, mag_z);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CROW_IMU, buf, MAVLINK_MSG_ID_CROW_IMU_MIN_LEN, MAVLINK_MSG_ID_CROW_IMU_LEN, MAVLINK_MSG_ID_CROW_IMU_CRC);
#else
    mavlink_crow_imu_t *packet = (mavlink_crow_imu_t *)msgbuf;
    packet->time_boot_ms = time_boot_ms;
    packet->accel_x = accel_x;
    packet->accel_y = accel_y;
    packet->accel_z = accel_z;
    packet->gyro_x = gyro_x;
    packet->gyro_y = gyro_y;
    packet->gyro_z = gyro_z;
    packet->mag_x = mag_x;
    packet->mag_y = mag_y;
    packet->mag_z = mag_z;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CROW_IMU, (const char *)packet, MAVLINK_MSG_ID_CROW_IMU_MIN_LEN, MAVLINK_MSG_ID_CROW_IMU_LEN, MAVLINK_MSG_ID_CROW_IMU_CRC);
#endif
}
#endif

#endif

// MESSAGE CROW_IMU UNPACKING


/**
 * @brief Get field time_boot_ms from crow_imu message
 *
 * @return  Timestamp (ms)
 */
static inline uint32_t mavlink_msg_crow_imu_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field accel_x from crow_imu message
 *
 * @return  X accel (m/s^2 * 100)
 */
static inline int16_t mavlink_msg_crow_imu_get_accel_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  4);
}

/**
 * @brief Get field accel_y from crow_imu message
 *
 * @return  Y accel (m/s^2 * 100)
 */
static inline int16_t mavlink_msg_crow_imu_get_accel_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  6);
}

/**
 * @brief Get field accel_z from crow_imu message
 *
 * @return  Z accel (m/s^2 * 100)
 */
static inline int16_t mavlink_msg_crow_imu_get_accel_z(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  8);
}

/**
 * @brief Get field gyro_x from crow_imu message
 *
 * @return  X gyro (deg/s * 10)
 */
static inline int16_t mavlink_msg_crow_imu_get_gyro_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  10);
}

/**
 * @brief Get field gyro_y from crow_imu message
 *
 * @return  Y gyro (deg/s * 10)
 */
static inline int16_t mavlink_msg_crow_imu_get_gyro_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  12);
}

/**
 * @brief Get field gyro_z from crow_imu message
 *
 * @return  Z gyro (deg/s * 10)
 */
static inline int16_t mavlink_msg_crow_imu_get_gyro_z(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  14);
}

/**
 * @brief Get field mag_x from crow_imu message
 *
 * @return  X mag (raw LSB)
 */
static inline int16_t mavlink_msg_crow_imu_get_mag_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  16);
}

/**
 * @brief Get field mag_y from crow_imu message
 *
 * @return  Y mag (raw LSB)
 */
static inline int16_t mavlink_msg_crow_imu_get_mag_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  18);
}

/**
 * @brief Get field mag_z from crow_imu message
 *
 * @return  Z mag (raw LSB)
 */
static inline int16_t mavlink_msg_crow_imu_get_mag_z(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  20);
}

/**
 * @brief Decode a crow_imu message into a struct
 *
 * @param msg The message to decode
 * @param crow_imu C-struct to decode the message contents into
 */
static inline void mavlink_msg_crow_imu_decode(const mavlink_message_t* msg, mavlink_crow_imu_t* crow_imu)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    crow_imu->time_boot_ms = mavlink_msg_crow_imu_get_time_boot_ms(msg);
    crow_imu->accel_x = mavlink_msg_crow_imu_get_accel_x(msg);
    crow_imu->accel_y = mavlink_msg_crow_imu_get_accel_y(msg);
    crow_imu->accel_z = mavlink_msg_crow_imu_get_accel_z(msg);
    crow_imu->gyro_x = mavlink_msg_crow_imu_get_gyro_x(msg);
    crow_imu->gyro_y = mavlink_msg_crow_imu_get_gyro_y(msg);
    crow_imu->gyro_z = mavlink_msg_crow_imu_get_gyro_z(msg);
    crow_imu->mag_x = mavlink_msg_crow_imu_get_mag_x(msg);
    crow_imu->mag_y = mavlink_msg_crow_imu_get_mag_y(msg);
    crow_imu->mag_z = mavlink_msg_crow_imu_get_mag_z(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_CROW_IMU_LEN? msg->len : MAVLINK_MSG_ID_CROW_IMU_LEN;
        memset(crow_imu, 0, MAVLINK_MSG_ID_CROW_IMU_LEN);
    memcpy(crow_imu, _MAV_PAYLOAD(msg), len);
#endif
}
