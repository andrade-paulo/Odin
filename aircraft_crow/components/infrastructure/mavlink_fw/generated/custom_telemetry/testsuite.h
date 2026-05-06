/** @file
 *    @brief MAVLink comm protocol testsuite generated from custom_telemetry.xml
 *    @see https://mavlink.io/en/
 */
#pragma once
#ifndef CUSTOM_TELEMETRY_TESTSUITE_H
#define CUSTOM_TELEMETRY_TESTSUITE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAVLINK_TEST_ALL
#define MAVLINK_TEST_ALL
static void mavlink_test_minimal(uint8_t, uint8_t, mavlink_message_t *last_msg);
static void mavlink_test_custom_telemetry(uint8_t, uint8_t, mavlink_message_t *last_msg);

static void mavlink_test_all(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_minimal(system_id, component_id, last_msg);
    mavlink_test_custom_telemetry(system_id, component_id, last_msg);
}
#endif

#include "../minimal/testsuite.h"


static void mavlink_test_custom_attitude(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_CUSTOM_ATTITUDE >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_custom_attitude_t packet_in = {
        963497464,17443,17547,17651,17755,17859,17963
    };
    mavlink_custom_attitude_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_boot_ms = packet_in.time_boot_ms;
        packet1.accel_x_mg = packet_in.accel_x_mg;
        packet1.accel_y_mg = packet_in.accel_y_mg;
        packet1.accel_z_mg = packet_in.accel_z_mg;
        packet1.gyro_x_mr = packet_in.gyro_x_mr;
        packet1.gyro_y_mr = packet_in.gyro_y_mr;
        packet1.gyro_z_mr = packet_in.gyro_z_mr;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_CUSTOM_ATTITUDE_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_CUSTOM_ATTITUDE_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_custom_attitude_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_custom_attitude_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_custom_attitude_pack(system_id, component_id, &msg , packet1.time_boot_ms , packet1.accel_x_mg , packet1.accel_y_mg , packet1.accel_z_mg , packet1.gyro_x_mr , packet1.gyro_y_mr , packet1.gyro_z_mr );
    mavlink_msg_custom_attitude_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_custom_attitude_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_boot_ms , packet1.accel_x_mg , packet1.accel_y_mg , packet1.accel_z_mg , packet1.gyro_x_mr , packet1.gyro_y_mr , packet1.gyro_z_mr );
    mavlink_msg_custom_attitude_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_custom_attitude_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_custom_attitude_send(MAVLINK_COMM_1 , packet1.time_boot_ms , packet1.accel_x_mg , packet1.accel_y_mg , packet1.accel_z_mg , packet1.gyro_x_mr , packet1.gyro_y_mr , packet1.gyro_z_mr );
    mavlink_msg_custom_attitude_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("CUSTOM_ATTITUDE") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_CUSTOM_ATTITUDE) != NULL);
#endif
}

static void mavlink_test_custom_navigation(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_CUSTOM_NAVIGATION >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_custom_navigation_t packet_in = {
        963497464,963497672,963497880,963498088,18067,18171
    };
    mavlink_custom_navigation_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.lat = packet_in.lat;
        packet1.lon = packet_in.lon;
        packet1.alt_mm = packet_in.alt_mm;
        packet1.time_boot_ms = packet_in.time_boot_ms;
        packet1.vel_cm_s = packet_in.vel_cm_s;
        packet1.heading_cdeg = packet_in.heading_cdeg;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_CUSTOM_NAVIGATION_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_CUSTOM_NAVIGATION_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_custom_navigation_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_custom_navigation_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_custom_navigation_pack(system_id, component_id, &msg , packet1.lat , packet1.lon , packet1.alt_mm , packet1.time_boot_ms , packet1.vel_cm_s , packet1.heading_cdeg );
    mavlink_msg_custom_navigation_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_custom_navigation_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.lat , packet1.lon , packet1.alt_mm , packet1.time_boot_ms , packet1.vel_cm_s , packet1.heading_cdeg );
    mavlink_msg_custom_navigation_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_custom_navigation_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_custom_navigation_send(MAVLINK_COMM_1 , packet1.lat , packet1.lon , packet1.alt_mm , packet1.time_boot_ms , packet1.vel_cm_s , packet1.heading_cdeg );
    mavlink_msg_custom_navigation_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("CUSTOM_NAVIGATION") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_CUSTOM_NAVIGATION) != NULL);
#endif
}

static void mavlink_test_custom_health(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_CUSTOM_HEALTH >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_custom_health_t packet_in = {
        963497464,17443,151
    };
    mavlink_custom_health_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_boot_ms = packet_in.time_boot_ms;
        packet1.vbat_mv = packet_in.vbat_mv;
        packet1.status_mask = packet_in.status_mask;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_CUSTOM_HEALTH_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_CUSTOM_HEALTH_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_custom_health_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_custom_health_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_custom_health_pack(system_id, component_id, &msg , packet1.time_boot_ms , packet1.vbat_mv , packet1.status_mask );
    mavlink_msg_custom_health_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_custom_health_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_boot_ms , packet1.vbat_mv , packet1.status_mask );
    mavlink_msg_custom_health_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_custom_health_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_custom_health_send(MAVLINK_COMM_1 , packet1.time_boot_ms , packet1.vbat_mv , packet1.status_mask );
    mavlink_msg_custom_health_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("CUSTOM_HEALTH") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_CUSTOM_HEALTH) != NULL);
#endif
}

static void mavlink_test_custom_telemetry(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_custom_attitude(system_id, component_id, last_msg);
    mavlink_test_custom_navigation(system_id, component_id, last_msg);
    mavlink_test_custom_health(system_id, component_id, last_msg);
}

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // CUSTOM_TELEMETRY_TESTSUITE_H
