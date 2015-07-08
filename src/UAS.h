#ifndef __UAS_H__
#define __UAS_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils_mavlink.h"
#include "qFlightInstruments.h"


/**
 * @brief The UAS Type enum
 */
enum UAS_Type
{
    UAS_MAV     = 0,
    UAS_GCS     = 1,
    UAS_TELEM   = 2,
};

/**
 * @brief The UAS class
 */
class UAS
{
public:
    UAS();
    ~UAS();

    void init(void);
    void release(void);

public:
    // basic information
    mavlink_heartbeat_t             msg_hb;
    mavlink_sys_status_t            msg_ss;
    mavlink_system_time_t           msg_st;

    uint8_t                         gcsID;                  ///< GCS ID
    uint8_t                         gcsCompID;              ///< GCS component ID
    uint8_t                         uavID;                  ///< UAV ID
    uint8_t                         uavCompID;              ///< Component ID
    uint8_t                         uavType;                ///< UAV type
    uint8_t                         uavAutopilot;           ///< Autopilot type
    uint8_t                         uavBaseMode;            ///< base mode
    uint8_t                         uavCustomMode;          ///< custom mode
    uint8_t                         uavSystemStatus;        ///< System Status
    uint8_t                         uavMavlinkVersion;      ///< mavlink version
    int                             uavMavlinkChan;         ///< mavlink channel

    int                             uavSeverity;            ///< Severity of status. Relies on the definitions within RFC-5424. See enum MAV_SEVERITY.
    char                            uavStatusText[50];      ///< system status text

    uint32_t                        sensorsPresent;         ///< sensors availiable
    uint32_t                        sensorsEnabled;         ///< sensors enabled
    uint32_t                        sensorsHealth;          ///< sensors health
    mavlink_sys_status_sensor_list  sensorsPresentList;     ///< availiable list
    mavlink_sys_status_sensor_list  sensorsEnabledList;     ///< enabled list
    mavlink_sys_status_sensor_list  sensorsHealthList;      ///< health list
    mavlink_sys_status_sensor_list  sensorsUnhealthList;    ///< unhealth list

    float                           cpuLoad;                ///< CPU load
    float                           battVolt;               ///< battery voltage
    float                           battCurrent;            ///< battery current
    float                           battRemaining;          ///< remaining battery
    float                           commDropRate;           ///< communication drop rate

    uint64_t                        systimeUnix;            ///< Timestamp of the master clock in microseconds since UNIX epoch.
    uint32_t                        bootTime;               ///< Timestamp of the component clock since boot time in milliseconds.

    // Telemetry
    int                             radioRX_errors;
    int                             radioFixed;
    int                             radioRSSI;
    int                             radioRSSI_remote;
    int                             radioTXBuf;
    int                             radioNoise;
    int                             radioNoise_remote;

    // navigation
    mavlink_gps_raw_int_t           msg_gps_raw;
    mavlink_global_position_int_t   msg_gp;
    mavlink_raw_imu_t               msg_imu_raw;
    mavlink_scaled_imu2_t           msg_imu;
    mavlink_attitude_t              msg_att;


    uint64_t                        gpsTime;
    double                          lat, lon, alt;
    double                          latHome, lonHome, altHome, hHome;
    int                             homeSetCount;
    double                          HDOP_h, HDOP_v;
    double                          gpsGroundSpeed;
    int                             gpsFixType;
    int                             nSat;


    double                          gpLat, gpLon;
    double                          gpAlt, gpH;
    double                          gpVx, gpVy, gpVz;
    double                          gpHeading;

    int                             Ax, Ay, Az;
    int                             Gx, Gy, Gz;
    int                             Mx, My, Mz;
    int                             Ax_raw, Ay_raw, Az_raw;
    int                             Gx_raw, Gy_raw, Gz_raw;
    int                             Mx_raw, My_raw, Mz_raw;

    float                           roll, pitch, yaw;
    float                           rollSpd, pitchSpd, yawSpd;


    // RC data
    mavlink_rc_channels_raw_t       msg_rc_raw;
    mavlink_rc_channels_t           msg_rc_all;
    int                             rcRSSI;
    int                             rcRaw[8];
    int                             rcAll[16];
    int                             rcRaw_port;
    int                             rcAll_channels;


    // GCS
    float                           gcsCPULoad;             ///< CPU load
    float                           gcsBattVolt;            ///< battery voltage
    float                           gcsBattCurrent;         ///< battery current
    float                           gcsBattRemaining;       ///< remaining battery
    float                           gcsCommDropRate;        ///< communication drop rate

    int                             gcsSeverity;            ///< Severity of status. Relies on the definitions within RFC-5424. See enum MAV_SEVERITY.
    char                            gcsStatusText[50];      ///< system status text
    int                             m_gcsStatusMsgTime;

    float                           gcsRoll, gcsPitch, gcsYaw;
    float                           gcsRollSpd, gcsPitchSpd, gcsYawSpd;

    uint64_t                        gcsGpsTime;
    double                          gcsLat, gcsLon, gcsAlt, gcsH, gcsHeading;
    double                          gcsHDOP_h, gcsHDOP_v;
    double                          gcsGpsGroundSpeed;
    int                             gcsGpsFixType;
    int                             gcsNSat;

protected:
    rtk::RMutex                     *m_mutexMsgWrite;
    rtk::OSA_HANDLE                 m_timer;
    std::vector<uint8_t>            m_msgBuffer;

    int                             m_bLinkConnected;
    uint64_t                        m_tmHB1, m_tmHB2;
    int                             m_pkgLost, m_pkgLastID;
    int                             m_recvMessageInSec;
    int                             m_uavStatusMsgTime;

    ValueAverager<float>            m_avgBatMAV, m_avgBatGCS;

    int                             m_bStreamRequested;
    int                             m_frqStreamRawSensors;
    int                             m_frqStreamExtStatus;
    int                             m_frqStreamRCChannels;
    int                             m_frqStreamRawController;
    int                             m_frqStreamPos;
    int                             m_frqStreamExtr1;
    int                             m_frqStreamExtr2;
    int                             m_frqStreamExtr3;

public:
    int parse_mavlink_msg(mavlink_message_t &msg);

    int parse_mavlink_msg_mav(mavlink_message_t &msg);
    int parse_mavlink_msg_gcs(mavlink_message_t &msg);
    int parse_mavlink_msg_telem(mavlink_message_t &msg);


    int send_mavlink_msg(mavlink_message_t &msg);

    int put_msg_buff(uint8_t *buf, int len);
    int get_msg_buff(uint8_t *buf, int *len);

    int gen_listmap_important(ListMap &lm);
    int gen_listmap_all(ListMap &lm);

    int link_connected(void) {
        return m_bLinkConnected;
    }

    int clearHome(void) {
        latHome = 9999;
        lonHome = 9999;
        altHome = 0;
        hHome   = 0;

        homeSetCount = 10;
    }


    int beginRecv(void);
    int stopRecv(void);
    int timerFunction(void *arg);
};

#endif // end of __UAS_H__
