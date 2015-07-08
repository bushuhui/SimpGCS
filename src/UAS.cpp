

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rtk_utils.h>

#include "UAS.h"

using namespace rtk;

void UAS_timerFunc(void *arg)
{
    UAS *u = (UAS*) arg;

    u->timerFunction(arg);
}


UAS::UAS()
{
    // initialize parameters
    init();

    // clear home
    clearHome();

    // initialize timer & mutex for msg wirting
    m_timer = 0;
    m_mutexMsgWrite = NULL;

    // status message time
    m_uavStatusMsgTime = -1;
    uavStatusText[0] = 0;
    uavSeverity = 1;

    m_gcsStatusMsgTime = -1;
    gcsStatusText[0] = 0;
    gcsSeverity = 1;

    // package lost stastic
    m_pkgLost = 0;
    m_pkgLastID = -1;

    // MAVLINK connected or not
    m_bLinkConnected = 0;

    // received message in one second
    m_recvMessageInSec = 0;

    // UAV stream requested
    //  see GCS_MAVLINK::data_stream_send(void)
    m_bStreamRequested = 0;

    // requrest information frequence
    //  STREAM_RAW_SENSORS:
    //      MSG_RAW_IMU1
    //      MSG_RAW_IMU2
    //      MSG_RAW_IMU3
    //  STREAM_EXTENDED_STATUS:
    //      MSG_EXTENDED_STATUS1
    //      MSG_EXTENDED_STATUS2
    //      MSG_CURRENT_WAYPOINT
    //      MSG_GPS_RAW
    //      MSG_NAV_CONTROLLER_OUTPUT
    //      MSG_LIMITS_STATUS
    //  STREAM_POSITION:
    //      MSG_LOCATION
    //  STREAM_RAW_CONTROLLER:
    //      MSG_SERVO_OUT
    //  STREAM_RC_CHANNELS:
    //      MSG_RADIO_OUT
    //      MSG_RADIO_IN
    //  STREAM_EXTRA1:
    //      MSG_ATTITUDE
    //      MSG_SIMSTATE
    //  STREAM_EXTRA2
    //      MSG_VFR_HUD
    //  STREAM_EXTRA3:
    //      MSG_AHRS
    //      MSG_HWSTATUS
    //      MSG_SYSTEM_TIME
    //      MSG_RANGEFINDER
    //      MSG_TERRAIN
    //      MSG_MOUNT_STATUS
    //      MSG_OPTICAL_FLOW
    //      MSG_GIMBAL_REPORT
    m_frqStreamRawSensors = 10;
    m_frqStreamExtStatus = 10;
    m_frqStreamRCChannels = 5;
    m_frqStreamRawController = 2;
    m_frqStreamPos = 10;
    m_frqStreamExtr1 = 10;
    m_frqStreamExtr2 = 1;
    m_frqStreamExtr3 = 2;
}

UAS::~UAS()
{
    stopRecv();
    release();
}

void UAS::init(void)
{
    // system information
    gcsID = 254;
    gcsCompID = 1;

    uavID = 1;
    uavCompID = 1;
    uavType = 0;
    uavCustomMode = 0;
    uavBaseMode = 0;
    uavSystemStatus = 0;
    uavMavlinkChan = 0;

    sensorsPresent = 0;
    sensorsEnabled = 0;
    sensorsHealth = 0;

    cpuLoad = 0;
    battVolt = 0;
    battCurrent = 0;
    battRemaining = 0;
    commDropRate = 0;

    systimeUnix = 0;
    bootTime = 0;

    // navigation
    gpsTime = 0;
    lat = 9999;
    lon = 9999;
    alt = 0;
    latHome = 9999;
    lonHome = 9999;
    altHome = 0;
    hHome   = 0;
    HDOP_h = 9999;
    HDOP_v = 9999;
    gpsGroundSpeed = 0;
    gpsFixType = 0;
    nSat = 0;

    gpLat = 9999;
    gpLon = 9999;
    gpAlt = 0;
    gpH = 0;
    gpVx = 0;
    gpVy = 0;
    gpVz = 0;
    gpHeading = 0;

    Ax = 0;
    Ay = 0;
    Az = 0;
    Gx = 0;
    Gy = 0;
    Gz = 0;
    Mx = 0;
    My = 0;
    Mz = 0;

    Ax_raw = 0;
    Ay_raw = 0;
    Az_raw = 0;
    Gx_raw = 0;
    Gy_raw = 0;
    Gz_raw = 0;
    Mx_raw = 0;
    My_raw = 0;
    Mz_raw = 0;

    roll = 0;
    pitch = 0;
    yaw = 0;
    rollSpd = 0;
    pitchSpd = 0;
    yawSpd = 0;    

    // Telemetry
    radioRX_errors      = 0;
    radioFixed          = 0;
    radioRSSI           = 0;
    radioRSSI_remote    = 0;
    radioTXBuf          = 0;
    radioNoise          = 0;
    radioNoise_remote   = 0;

    // GCS
    gcsCPULoad          = 0;            ///< CPU load
    gcsBattVolt         = 0;            ///< battery voltage
    gcsBattCurrent      = 0;            ///< battery current
    gcsBattRemaining    = 0;            ///< remaining battery
    gcsCommDropRate     = 0;            ///< communication drop rate

    gcsRoll             = 0;
    gcsPitch            = 0;
    gcsYaw              = 0;
    gcsRollSpd          = 0;
    gcsPitchSpd         = 0;
    gcsYawSpd           = 0;

    gcsGpsTime          = 0;
    gcsLat              = 0;
    gcsLon              = 0;
    gcsAlt              = 0;
    gcsH                = 0;
    gcsHeading          = 0;

    gcsHDOP_h           = 9999;
    gcsHDOP_v           = 9999;
    gcsGpsGroundSpeed   = 0;
    gcsGpsFixType       = 0;
    gcsNSat             = 0;
}

void UAS::release(void)
{
    return;
}

int UAS::beginRecv(void)
{
    // output buffer
    m_mutexMsgWrite = new rtk::RMutex();
    m_msgBuffer.reserve(1024);

    // create timer
    if( 0 != osa_tm_create(&m_timer, 1000, UAS_timerFunc, this) ) {
        dbg_pe("Can not creat timer");
    }

    return 0;
}

int UAS::stopRecv(void)
{
    if( m_timer != 0 )
        osa_tm_delete(m_timer);
    if( m_mutexMsgWrite != NULL )
        delete m_mutexMsgWrite;

    m_timer = 0;
    m_mutexMsgWrite = NULL;

    return 0;
}


int UAS::parse_mavlink_msg(mavlink_message_t &msg)
{
    // FIXME: parse MAVLINK message based on sysid
    //  0   ~ 49:  MAV
    //  50  ~ 249: Telemetry
    //  250 ~ 255: GCS

    if( msg.sysid < 50 )
        return parse_mavlink_msg_mav(msg);

    if( msg.sysid >= 50 && msg.sysid < 249 )
        return parse_mavlink_msg_telem(msg);

    if( msg.sysid >= 250 )
        return parse_mavlink_msg_gcs(msg);
}

int UAS::parse_mavlink_msg_mav(mavlink_message_t &msg)
{
    // count received message in one second
    m_recvMessageInSec ++;

    // stastic lost package
    if( m_pkgLastID >= 0 ) {
        if( msg.seq < m_pkgLastID )
            m_pkgLost += (255+msg.seq) - m_pkgLastID;
        else
            m_pkgLost += msg.seq - m_pkgLastID - 1;
    }
    m_pkgLastID = msg.seq;

    // for each message type
    switch( msg.msgid ) {
    case MAVLINK_MSG_ID_HEARTBEAT:
        mavlink_msg_heartbeat_decode(&msg, &msg_hb);

        uavID               = msg.sysid;
        uavCompID           = msg.compid;

        uavCustomMode       = msg_hb.custom_mode;
        uavType             = msg_hb.type;
        uavAutopilot        = msg_hb.autopilot;
        uavBaseMode         = msg_hb.base_mode;
        uavSystemStatus     = msg_hb.system_status;
        uavMavlinkVersion   = msg_hb.mavlink_version;

        break;

    case MAVLINK_MSG_ID_SYS_STATUS:
        mavlink_msg_sys_status_decode(&msg, &msg_ss);

        sensorsPresent      = msg_ss.onboard_control_sensors_present;
        sensorsEnabled      = msg_ss.onboard_control_sensors_enabled;
        sensorsHealth       = msg_ss.onboard_control_sensors_health;
        cpuLoad             = msg_ss.load * 1.0 / 10.0;
        battVolt            = m_avgBatMAV.push(msg_ss.voltage_battery * 1.0 / 1000.0);
        battCurrent         = msg_ss.current_battery * 1.0 / 100.0;
        battRemaining       = msg_ss.battery_remaining;
        commDropRate        = msg_ss.drop_rate_comm * 1.0 / 100.0;

        mavlink_sys_status_sensor_getIDs(sensorsPresent, sensorsPresentList);
        mavlink_sys_status_sensor_getIDs(sensorsEnabled, sensorsEnabledList);
        mavlink_sys_status_sensor_getIDs(sensorsHealth, sensorsHealthList);
        mavlink_sys_status_sensor_getID_Difference(sensorsPresentList, sensorsHealthList,
                                                   sensorsUnhealthList);

        break;

    case MAVLINK_MSG_ID_STATUSTEXT:
        mavlink_statustext_t    st;
        mavlink_msg_statustext_decode(&msg, &st);

        uavSeverity = st.severity;
        strcpy(uavStatusText, st.text);

        fmt::print_colored(fmt::RED, "STATUS_MAV[{0}] = {1}\n",
                           uavSeverity, uavStatusText);
        m_uavStatusMsgTime = 0;

        break;

    case MAVLINK_MSG_ID_SYSTEM_TIME:
        mavlink_msg_system_time_decode(&msg, &msg_st);

        systimeUnix         = msg_st.time_unix_usec;
        bootTime            = msg_st.time_boot_ms;

        break;


    case MAVLINK_MSG_ID_GPS_RAW_INT:
        mavlink_msg_gps_raw_int_decode(&msg, &msg_gps_raw);

        gpsTime             = msg_gps_raw.time_usec;
        lat                 = msg_gps_raw.lat * 1.0 / 1e7;
        lon                 = msg_gps_raw.lon * 1.0 / 1e7;
        alt                 = msg_gps_raw.alt * 1.0 / 1000.0;
        HDOP_h              = msg_gps_raw.eph * 1.0 / 100.0;
        HDOP_v              = msg_gps_raw.epv * 1.0 / 100.0;
        gpsGroundSpeed      = msg_gps_raw.vel * 1.0 / 100.0;
        gpsFixType          = msg_gps_raw.fix_type;
        nSat                = msg_gps_raw.satellites_visible;

        // FIXME: fix HDOP values
        if( fabs(HDOP_h) < 0.001 ) HDOP_h = 9999;
        if( fabs(HDOP_v) < 0.001 ) HDOP_v = 9999;

        // FIXME: get home position
        if( gpsFixType >= 3 && homeSetCount > 0 ) {
            latHome = lat;
            lonHome = lon;
            altHome = alt;
            hHome   = gpH;

            if( homeSetCount > 0 ) homeSetCount --;
        }

        break;

    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
        mavlink_msg_global_position_int_decode(&msg, &msg_gp);

        bootTime            = msg_gp.time_boot_ms;
        gpLat               = msg_gp.lat * 1.0 / 1e7;
        gpLon               = msg_gp.lon * 1.0 / 1e7;
        gpAlt               = msg_gp.alt * 1.0 / 1e3;
        gpH                 = msg_gp.relative_alt * 1.0 / 1e3;
        gpVx                = msg_gp.vx * 1.0 / 100.0;
        gpVy                = msg_gp.vy * 1.0 / 100.0;
        gpVz                = msg_gp.vz * 1.0 / 100.0;
        gpHeading           = msg_gp.hdg * 1.0 / 100.0;

        break;

    case MAVLINK_MSG_ID_RAW_IMU:
        mavlink_msg_raw_imu_decode(&msg, &msg_imu_raw);

        Ax_raw              = msg_imu_raw.xacc;
        Ay_raw              = msg_imu_raw.yacc;
        Az_raw              = msg_imu_raw.zacc;
        Gx_raw              = msg_imu_raw.xgyro;
        Gy_raw              = msg_imu_raw.ygyro;
        Gz_raw              = msg_imu_raw.zgyro;
        Mx_raw              = msg_imu_raw.xmag;
        My_raw              = msg_imu_raw.ymag;
        Mz_raw              = msg_imu_raw.zmag;

        break;

    case MAVLINK_MSG_ID_SCALED_IMU2:
        mavlink_msg_scaled_imu2_decode(&msg, &msg_imu);

        Ax                  = msg_imu.xacc;
        Ay                  = msg_imu.yacc;
        Az                  = msg_imu.zacc;
        Gx                  = msg_imu.xgyro;
        Gy                  = msg_imu.ygyro;
        Gz                  = msg_imu.zgyro;
        Mx                  = msg_imu.xmag;
        My                  = msg_imu.ymag;
        Mz                  = msg_imu.zmag;

        break;

    case MAVLINK_MSG_ID_ATTITUDE:
        mavlink_msg_attitude_decode(&msg, &msg_att);

        bootTime            = msg_att.time_boot_ms;
        roll                = msg_att.roll  * 180.0 / M_PI;
        pitch               = msg_att.pitch * 180.0 / M_PI;
        yaw                 = msg_att.yaw   * 180.0 / M_PI;
        rollSpd             = msg_att.rollspeed;
        pitchSpd            = msg_att.pitchspeed;
        yawSpd              = msg_att.yawspeed;

        break;



    case MAVLINK_MSG_ID_RC_CHANNELS_RAW:
        mavlink_msg_rc_channels_raw_decode(&msg, &msg_rc_raw);

        rcRaw[0]            = msg_rc_raw.chan1_raw;
        rcRaw[1]            = msg_rc_raw.chan2_raw;
        rcRaw[2]            = msg_rc_raw.chan3_raw;
        rcRaw[3]            = msg_rc_raw.chan4_raw;
        rcRaw[4]            = msg_rc_raw.chan5_raw;
        rcRaw[5]            = msg_rc_raw.chan6_raw;
        rcRaw[6]            = msg_rc_raw.chan7_raw;
        rcRaw[7]            = msg_rc_raw.chan8_raw;
        rcRaw_port          = msg_rc_raw.port;
        rcRSSI              = msg_rc_raw.rssi;

        /*
        printf("rcRaw (%5d): ", rcRSSI);
        for(int i=0; i<8; i++) printf("%4d ", rcRaw[i]);
        printf("\n");
        */

        break;

    case MAVLINK_MSG_ID_RC_CHANNELS:
        mavlink_msg_rc_channels_decode(&msg, &msg_rc_all);

        rcAll[0]            = msg_rc_all.chan1_raw;
        rcAll[1]            = msg_rc_all.chan2_raw;
        rcAll[2]            = msg_rc_all.chan3_raw;
        rcAll[3]            = msg_rc_all.chan4_raw;
        rcAll[4]            = msg_rc_all.chan5_raw;
        rcAll[5]            = msg_rc_all.chan6_raw;
        rcAll[6]            = msg_rc_all.chan7_raw;
        rcAll[7]            = msg_rc_all.chan8_raw;
        rcAll[8]            = msg_rc_all.chan9_raw;
        rcAll[9]            = msg_rc_all.chan10_raw;
        rcAll[10]           = msg_rc_all.chan11_raw;
        rcAll[11]           = msg_rc_all.chan12_raw;
        rcAll[12]           = msg_rc_all.chan13_raw;
        rcAll[13]           = msg_rc_all.chan14_raw;
        rcAll[14]           = msg_rc_all.chan15_raw;
        rcAll[15]           = msg_rc_all.chan16_raw;
        rcAll[16]           = msg_rc_all.chan17_raw;
        rcAll[17]           = msg_rc_all.chan18_raw;
        rcAll_channels      = msg_rc_all.rssi;

        //printf("rcAll (%5d): ", rcRSSI);
        //for(int i=0; i<16; i++) printf("%4d ", rcAll[i]);
        //printf("\n");

        break;

    case MAVLINK_MSG_ID_RC_CHANNELS_OVERRIDE:
        mavlink_rc_channels_override_t pack;
        mavlink_msg_rc_channels_override_decode(&msg, &pack);

        /*
        printf("RC_OVERRIDE: %3d %3d-%3d, %4d %4d %4d %4d %4d %4d %4d %4d\n",
               msg.sysid, pack.target_system, pack.target_component,
               pack.chan1_raw, pack.chan2_raw, pack.chan3_raw, pack.chan4_raw,
               pack.chan5_raw, pack.chan6_raw, pack.chan7_raw, pack.chan8_raw);
        */

        break;
    }

    return 0;
}

int UAS::parse_mavlink_msg_gcs(mavlink_message_t &msg)
{
    // for each message type
    switch( msg.msgid ) {
    case MAVLINK_MSG_ID_SYS_STATUS:
        mavlink_sys_status_t ss;
        mavlink_msg_sys_status_decode(&msg, &ss);

        gcsCPULoad          = ss.load * 1.0 / 10.0;
        gcsBattVolt         = m_avgBatGCS.push(ss.voltage_battery * 1.0 / 1000.0);
        gcsBattCurrent      = ss.current_battery * 1.0 / 100.0;
        gcsBattRemaining    = ss.battery_remaining;
        gcsCommDropRate     = ss.drop_rate_comm * 1.0 / 100.0;

        break;

    case MAVLINK_MSG_ID_STATUSTEXT:
        mavlink_statustext_t    st;
        mavlink_msg_statustext_decode(&msg, &st);

        gcsSeverity = st.severity;
        strcpy(gcsStatusText, st.text);

        fmt::print_colored(fmt::RED, "STATUS_GCS[{0}] = {1}\n",
                           gcsSeverity, gcsStatusText);
        m_gcsStatusMsgTime = 0;

        break;

    case MAVLINK_MSG_ID_GPS_RAW_INT:
        mavlink_gps_raw_int_t gps_raw;
        mavlink_msg_gps_raw_int_decode(&msg, &gps_raw);

        gcsGpsTime          = gps_raw.time_usec;
        gcsLat              = gps_raw.lat * 1.0 / 1e7;
        gcsLon              = gps_raw.lon * 1.0 / 1e7;
        gcsAlt              = gps_raw.alt * 1.0 / 1000.0;
        gcsHDOP_h           = gps_raw.eph * 1.0 / 100.0;
        gcsHDOP_v           = gps_raw.epv * 1.0 / 100.0;
        gcsGpsGroundSpeed   = gps_raw.vel * 1.0 / 100.0;
        gcsGpsFixType       = gps_raw.fix_type;
        gcsNSat             = gps_raw.satellites_visible;

        if( fabs(gcsHDOP_h) < 0.001 ) gcsHDOP_h = 9999;
        if( fabs(gcsHDOP_v) < 0.001 ) gcsHDOP_v = 9999;

        break;

    case MAVLINK_MSG_ID_GLOBAL_POSITION_INT:
        mavlink_global_position_int_t gp;
        mavlink_msg_global_position_int_decode(&msg, &gp);

        gcsH                = gp.relative_alt * 1.0 / 1e3;
        gcsHeading          = gp.hdg * 1.0 / 100.0;

        break;


    case MAVLINK_MSG_ID_ATTITUDE:
        mavlink_attitude_t att;
        mavlink_msg_attitude_decode(&msg, &att);

        gcsRoll             = att.roll  * 180.0 / M_PI;
        gcsPitch            = att.pitch * 180.0 / M_PI;
        gcsYaw              = att.yaw   * 180.0 / M_PI;
        gcsRollSpd          = att.rollspeed;
        gcsPitchSpd         = att.pitchspeed;
        gcsYawSpd           = att.yawspeed;

        break;
    }

    return 0;
}

int UAS::parse_mavlink_msg_telem(mavlink_message_t &msg)
{
    // for each message type
    switch( msg.msgid ) {
    case MAVLINK_MSG_ID_RADIO_STATUS:
        mavlink_radio_status_t rs;

        mavlink_msg_radio_status_decode(&msg, &rs);
        radioRX_errors      = rs.rxerrors;
        radioFixed          = rs.fixed;
        radioRSSI           = rs.rssi;
        radioRSSI_remote    = rs.remrssi;
        radioTXBuf          = rs.txbuf;
        radioNoise          = rs.noise;
        radioNoise_remote   = rs.remnoise;

        break;
    }

    return 0;
}

int UAS::gen_listmap_important(ListMap &lm)
{
    char                *name;

    int                 rssi_min = 90, rssi_max = 220;
    double              rssi_l, rssi_r;

    int                 bt_min, bt_sec, bt_msec;

    // clear old data
    lm.clear();


    bt_msec = bootTime % 1000;
    bt_sec  = bootTime/1000;
    bt_min  = bt_sec / 60;
    bt_sec  = bt_sec % 60;
    lm["sys_bTime"] = QString::fromStdString(
                trim(fmt::sprintf("%d:%02d.%03d", bt_min, bt_sec, bt_msec)));


    //mavlink_mav_type_name(uavType, &name);
    //lm["sys_uavType"]   = name;
    //mavlink_autopilot_name(uavAutopilot, &name);
    //lm["sys_AP"]        = name;
    //mavlink_mav_state_name(uavSystemStatus, &name);
    //lm["sys_state"]     = name;

    if( strlen(uavStatusText) > 0 ) {
        lm["sys_status"] = QString::fromStdString(
                    trim(fmt::sprintf("[%d] %s", uavSeverity, uavStatusText)));
    } else {
        lm["sys_status"] = "";
    }

    //lm["sys_pkgLost"] = QString::fromStdString(trim(fmt::sprintf("%d", m_pkgLost)));



    /*
    mavlink_mav_mode_name(uavBaseMode, nl);
    for(int i=0; i<nl.size(); i++) {
        if( i == 0 ) nl_all = nl_all + nl[i];
        else         nl_all = nl_all + ", " + nl[i];
    }
    lm["sys_mode"]      = QString::fromStdString(nl_all);
    */


    //lm["sys_uavID"]     = QString::fromStdString(trim(fmt::sprintf("%d", uavID)));

    lm["sys_bat_v"]     = QString::fromStdString(trim(fmt::sprintf("%6.2f", battVolt)));
    lm["sys_bat_c"]     = QString::fromStdString(trim(fmt::sprintf("%6.2f", battCurrent)));
    lm["sys_bat_R"]     = QString::fromStdString(trim(fmt::sprintf("%6.2f%%", battRemaining)));
    lm["sys_CPU"]       = QString::fromStdString(trim(fmt::sprintf("%6.2f%%", cpuLoad)));

    //lm["att_roll"]      = QString::fromStdString(trim(fmt::sprintf("%12f", roll)));
    //lm["att_pitch"]     = QString::fromStdString(trim(fmt::sprintf("%12f", pitch)));
    //lm["att_yaw"]       = QString::fromStdString(trim(fmt::sprintf("%12f", yaw)));

    lm["gp_alt"]        = QString::fromStdString(trim(fmt::sprintf("%9.2f", gpAlt)));
    lm["gp_H"]          = QString::fromStdString(trim(fmt::sprintf("%9.2f", gpH)));
    lm["gp_nSat"]       = QString::fromStdString(trim(fmt::sprintf("%d", nSat)));
    lm["gp_HDOP_H"]     = QString::fromStdString(trim(fmt::sprintf("%9.2f", HDOP_h)));
    lm["gp_HDOP_V"]     = QString::fromStdString(trim(fmt::sprintf("%8.2f", HDOP_v)));
    lm["gp_heading"]    = QString::fromStdString(trim(fmt::sprintf("%6.2f", gpHeading)));

    switch( gpsFixType ) {
    case 0:
    case 1:
        lm["gp_Fixed"]  = "Not fixed";
        break;

    case 2:
        lm["gp_Fixed"]  = "2D fixed";
        break;

    case 3:
        lm["gp_Fixed"]  = "3D fixed";
        break;

    case 4:
        lm["gp_Fixed"]  = "DGPS";
        break;

    case 5:
        lm["gp_Fixed"]  = "RTK";
        break;
    }


    // Telemetry RSSI
    rssi_l = (radioRSSI - rssi_min)*1.0 / (rssi_max - rssi_min) * 100.0;
    if( rssi_l > 100.0 ) rssi_l = 100.0;
    if( rssi_l < 0.0 )   rssi_l = 0.0;
    rssi_r = (radioRSSI_remote - rssi_min)*1.0 / (rssi_max - rssi_min) * 100.0;
    if( rssi_r > 100.0 ) rssi_r = 100.0;
    if( rssi_r < 0.0 )   rssi_r = 0.0;
    lm["RSSI"] = QString::fromStdString(trim(fmt::sprintf("%3d(%5.1f%%), %3d(%5.1f%%)",
                                                              radioRSSI, rssi_l,
                                                              radioRSSI_remote, rssi_r)));

    // GCS
    if( strlen(gcsStatusText) > 0 ) {
        lm["GCS_status"] = QString::fromStdString(trim(fmt::sprintf("[%d] %s",
                                                                    gcsSeverity, gcsStatusText)));
    } else {
        lm["GCS_status"] = "";
    }

    lm["GCS_bat_v"]     = QString::fromStdString(trim(fmt::sprintf("%6.2f", gcsBattVolt)));
    lm["GCS_bat_R"]     = QString::fromStdString(trim(fmt::sprintf("%6.2f%%", gcsBattRemaining)));
    lm["GCS_CPU"]       = QString::fromStdString(trim(fmt::sprintf("%6.2f%%", gcsCPULoad)));

    lm["GCS_alt"]       = QString::fromStdString(trim(fmt::sprintf("%9.2f", gcsAlt)));
    lm["GCS_H"]         = QString::fromStdString(trim(fmt::sprintf("%9.2f", gcsH)));
    lm["GCS_nSat"]      = QString::fromStdString(trim(fmt::sprintf("%d",   gcsNSat)));
    lm["GCS_HDOP_H"]    = QString::fromStdString(trim(fmt::sprintf("%9.2f", gcsHDOP_h)));
    lm["GCS_HDOP_V"]    = QString::fromStdString(trim(fmt::sprintf("%9.2f", gcsHDOP_v)));
    lm["GCS_heading"]   = QString::fromStdString(trim(fmt::sprintf("%6.2f", gcsHeading)));

    switch( gcsGpsFixType ) {
    case 0:
    case 1:
        lm["GCS_GPS"]  = "Not fixed";
        break;

    case 2:
        lm["GCS_GPS"]  = "2D fixed";
        break;

    case 3:
        lm["GCS_GPS"]  = "3D fixed";
        break;

    case 4:
        lm["GCS_GPS"]  = "DGPS";
        break;

    case 5:
        lm["GCS_GPS"]  = "RTK";
        break;
    }

    return 0;
}

int UAS::gen_listmap_all(ListMap &lm)
{
    return 0;
}

int UAS::timerFunction(void *arg)
{
    mavlink_message_t beat, msg;
    static int bRequestDataStream = 0;

    // send heartbeat
    mavlink_msg_heartbeat_pack(gcsID, gcsCompID, &beat,
                               MAV_TYPE_GCS, MAV_AUTOPILOT_INVALID,
                               MAV_MODE_MANUAL_ARMED, 0, MAV_STATE_ACTIVE);
    send_mavlink_msg(beat);

    // check connection
    if( m_recvMessageInSec < 2 ) {
        m_bLinkConnected = 0;
    } else {
        m_bLinkConnected = 1;
    }

    // auto clean status message
    if( m_uavStatusMsgTime >= 0 ) {
        m_uavStatusMsgTime ++;

        if( m_uavStatusMsgTime > 30 ) {
            m_uavStatusMsgTime = -1;
            uavStatusText[0] = 0;
            uavSeverity = 0;
        }
    }

    // auto clean GCS status message
    if( m_gcsStatusMsgTime >= 0 ) {
        m_gcsStatusMsgTime ++;

        if( m_gcsStatusMsgTime > 30 ) {
            m_gcsStatusMsgTime = -1;
            gcsStatusText[0] = 0;
            gcsSeverity = 0;
        }
    }

    // send stream request
    if( m_bLinkConnected == 0 || bRequestDataStream == 0 ) {
        bRequestDataStream = 1;
        //dbg_pt("request data stream");
        mavlink_request_data_stream_t packet;

        packet.target_system = uavID;
        packet.target_component = uavCompID;
        packet.start_stop = 1;

        packet.req_stream_id = MAV_DATA_STREAM_RAW_SENSORS;
        packet.req_message_rate = m_frqStreamRawSensors;
        mavlink_msg_request_data_stream_encode(uavID, uavCompID, &msg, &packet);
        send_mavlink_msg(msg);

        packet.req_stream_id = MAV_DATA_STREAM_EXTENDED_STATUS;
        packet.req_message_rate = m_frqStreamExtStatus;
        mavlink_msg_request_data_stream_encode(uavID, uavCompID, &msg, &packet);
        send_mavlink_msg(msg);

        packet.req_stream_id = MAV_DATA_STREAM_RC_CHANNELS;
        packet.req_message_rate = m_frqStreamRCChannels;
        mavlink_msg_request_data_stream_encode(uavID, uavCompID, &msg, &packet);
        send_mavlink_msg(msg);

        packet.req_stream_id = MAV_DATA_STREAM_RAW_CONTROLLER;
        packet.req_message_rate = m_frqStreamRawController;
        mavlink_msg_request_data_stream_encode(uavID, uavCompID, &msg, &packet);
        send_mavlink_msg(msg);

        packet.req_stream_id = MAV_DATA_STREAM_POSITION;
        packet.req_message_rate = m_frqStreamPos;
        mavlink_msg_request_data_stream_encode(uavID, uavCompID, &msg, &packet);
        send_mavlink_msg(msg);

        packet.req_stream_id = MAV_DATA_STREAM_EXTRA1;
        packet.req_message_rate = m_frqStreamExtr1;
        mavlink_msg_request_data_stream_encode(uavID, uavCompID, &msg, &packet);
        send_mavlink_msg(msg);

        packet.req_stream_id = MAV_DATA_STREAM_EXTRA2;
        packet.req_message_rate = m_frqStreamExtr2;
        mavlink_msg_request_data_stream_encode(uavID, uavCompID, &msg, &packet);
        send_mavlink_msg(msg);

        packet.req_stream_id = MAV_DATA_STREAM_EXTRA3;
        packet.req_message_rate = m_frqStreamExtr3;
        mavlink_msg_request_data_stream_encode(uavID, uavCompID, &msg, &packet);
        send_mavlink_msg(msg);
    }

    m_recvMessageInSec = 0;

    return 0;
}

int UAS::send_mavlink_msg(mavlink_message_t &msg)
{
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];

    // Write message into buffer, prepending start sign
    int len = mavlink_msg_to_send_buffer(buffer, &msg);
    static uint8_t messageKeys[256] = MAVLINK_MESSAGE_CRCS;
    mavlink_finalize_message_chan(&msg, gcsID, gcsCompID,
                                  uavMavlinkChan,
                                  msg.len, messageKeys[msg.msgid]);

    return put_msg_buff(buffer, len);
}

int UAS::put_msg_buff(uint8_t *buf, int len)
{
    m_mutexMsgWrite->lock();

    for(int i=0; i<len; i++)
        m_msgBuffer.push_back(buf[i]);

    m_mutexMsgWrite->unlock();

    return 0;
}

int UAS::get_msg_buff(uint8_t *buf, int *len)
{
    m_mutexMsgWrite->lock();

    *len = m_msgBuffer.size();
    for(int i=0; i<*len; i++) {
        buf[i] = m_msgBuffer[i];
    }

    m_msgBuffer.clear();

    m_mutexMsgWrite->unlock();

    return 0;
}
