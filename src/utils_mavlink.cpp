

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>

#include "utils_mavlink.h"


////////////////////////////////////////////////////////////////////////////////
/// enum to name
////////////////////////////////////////////////////////////////////////////////

char g_mavlink_autopilot_name[][200] =
{
    "Generic",
    "PIXHAWK",
    "Slugs",
    "APM",
    "OpenPilot",
    "Waypoints only",
    "Waypoints & Simple Nav",
    "Mission full",
    "INVALID",
    "PPZ",
    "UDB",
    "FP",
    "PX4",
    "SMACCMPILOT",
    "AUTOQUAD",
    "ARMAZILA",
    "AEROB",
    "ASLUAV",
    ""
};

int mavlink_autopilot_name(uint8_t ap, char **name)
{
    if ( ap > 17 ) {
        return -1;
    } else {
        *name = g_mavlink_autopilot_name[ap];
        return 0;
    }
}


char g_mavlink_mav_type_name[][200] =
{
    "GENERIC",
    "FIXED_WING",
    "QUADROTOR",
    "COAXIAL",
    "HELICOPTER",
    "ANTENNA_TRACKER",
    "GCS",
    "AIRSHIP",
    "FREE_BALLOON",
    "ROCKET",
    "GROUND_ROVER",
    "SURFACE_BOAT",
    "SUBMARINE",
    "HEXAROTOR",
    "OCTOROTOR",
    "TRICOPTER",
    "FLAPPING_WING",
    "KITE",
    "ONBOARD_CONTROLLER",
    "VTOL_DUOROTOR",
    "VTOL_QUADROTOR",
    "GIMBAL",
    ""
};

int mavlink_mav_type_name(uint8_t mt, char **name)
{
    if( mt > 26 ) {
        return -1;
    } else {
        *name = g_mavlink_mav_type_name[mt];
        return 0;
    }
}


char g_mavlink_mav_mode_name[][200] =
{
    "Custom",
    "Test",
    "Auto",
    "Guided",
    "Stabilize",
    "HIL",
    "Manual Input",
    "Armed",
    ""
};

int mavlink_mav_mode_getName(uint8_t mf, char **name)
{
    if( mf > 7 ) {
        return -1;
    } else {
        *name = g_mavlink_mav_mode_name[mf];
        return 0;
    }
}

int mavlink_mav_mode_name(uint8_t mf, mavlink_nameList &nl)
{
    int         i;
    uint32_t    m;
    char        *name;

    m = 1;
    for(i=0; i<7; i++) {
        if( (mf & m) > 0 ) {
            mavlink_mav_mode_getName(i, &name);
            nl.push_back(name);
        }
    }

    return 0;
}


char g_mavlink_mav_state_name[][200] =
{
    "UNINIT",
    "BOOT",
    "CALIBRATING",
    "STANDBY",
    "ACTIVE",
    "CRITICAL",
    "EMERGENCY",
    "POWEROFF",
    ""
};

int mavlink_mav_state_name(uint8_t ms, char **name)
{
    if( ms > 7 ) {
        return -1;
    } else {
        *name = g_mavlink_mav_state_name[ms];
        return 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// MAV_SYS_STATUS_SENSOR
////////////////////////////////////////////////////////////////////////////////

static MAVLINK_SYS_STATUS_SENSOR_IDNAME_Struct g_mavlink_sys_status_sensor_idname[] =
{
    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_SENSOR_3D_GYRO,
                "3D Gyro", "3D Gryo"),
    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_SENSOR_3D_ACCEL,
                "3D Accelerometer", "3D Gryo"),
    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_SENSOR_3D_MAG,
                "3D Magnetometer", "3D Magnetometer"),
    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_SENSOR_ABSOLUTE_PRESSURE,
                "Differential pressure", "Differential pressure"),
    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_SENSOR_GPS,
                "GPS", "GPS"),
    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_SENSOR_OPTICAL_FLOW,
                "Optical flow", "Optical flow"),
    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_SENSOR_VISION_POSITION,
                "Computer vision position", "Computer vision position"),
    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_SENSOR_LASER_POSITION,
                "laser based position", "laser based position"),
    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_SENSOR_EXTERNAL_GROUND_TRUTH,
                "External ground truth", "External ground truth"),
    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_SENSOR_ANGULAR_RATE_CONTROL,
                "3D angular rate control", "3D angular rate control"),
    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_SENSOR_ATTITUDE_STABILIZATION,
                "Attitude stabilization", "Attitude stabilization"),
    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_SENSOR_YAW_POSITION,
                "Yaw position", "Yaw position"),
    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_SENSOR_Z_ALTITUDE_CONTROL,
                "z/altitude control", "z/altitude control"),
    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_SENSOR_XY_POSITION_CONTROL,
                "x/y position control", "x/y position control"),
    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_SENSOR_MOTOR_OUTPUTS,
                "motor outputs / control", "motor outputs / control"),
    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_SENSOR_RC_RECEIVER,
                "rc receiver", "rc receiver"),
    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_SENSOR_3D_GYRO2,
                "2nd 3D gyro", "2nd 3D gyro"),
    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_SENSOR_3D_ACCEL2,
                "2nd 3D accelerometer", "2nd 3D accelerometer"),
    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_SENSOR_3D_MAG2,
                "2nd 3D magnetometer", "2nd 3D magnetometer"),
    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_GEOFENCE,
                "geofence", "geofence"),
    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_AHRS,
                "AHRS subsystem health", "AHRS subsystem health"),
    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_TERRAIN,
                "z/altitude control", "z/altitude control"),

    MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(MAV_SYS_STATUS_SENSOR_ENUM_END,
                "None", "None"),
};

int mavlink_sys_status_sensor_getIDs(uint32_t s,
                                        mavlink_sys_status_sensor_list &l)
{
    int         i;
    uint32_t    m;

    m = 1;

    l.clear();

    for(i=0; i<31; i++) {
        if( (s & m) != 0 ) {
            l.push_back(m);
        }

        m = m << 1;
    }

    return 0;
}


int mavlink_sys_status_sensor_getName(uint32_t id, char **name)
{
    int     i;

    i = 0;
    *name = NULL;

    while(1) {
        if( g_mavlink_sys_status_sensor_idname[i].id == MAV_SYS_STATUS_SENSOR_ENUM_END ) {
            break;
        }

        if( g_mavlink_sys_status_sensor_idname[i].id == id ) {
            *name = g_mavlink_sys_status_sensor_idname[i].name;
            return 0;
        }

        i++;
    }

    return -1;
}

int mavlink_sys_status_sensor_getNames(mavlink_sys_status_sensor_list &l,
                                       mavlink_sys_status_sensor_namelist &nl)
{
    char    *name;
    mavlink_sys_status_sensor_list::iterator it;

    nl.clear();

    for(it=l.begin(); it!=l.end(); it++) {
        if( 0 == mavlink_sys_status_sensor_getName(*it, &name) ) {
            nl.push_back(name);
        } else {
            break;
        }
    }

    return 0;
}

int mavlink_sys_status_sensor_getID_Difference(mavlink_sys_status_sensor_list &l1,
                                               mavlink_sys_status_sensor_list &l2,
                                               mavlink_sys_status_sensor_list &ld)
{
    mavlink_sys_status_sensor_list  _l1, _l2;
    mavlink_sys_status_sensor_list::iterator it;

    mavlink_sys_status_sensor_list   v(32);

    _l1 = l1;
    _l2 = l2;

    std::sort(_l1.begin(), _l1.end());
    std::sort(_l2.begin(), _l2.end());

    it = std::set_difference(_l1.begin(), _l1.end(), _l2.begin(), _l2.end(), v.begin());
    v.resize(it-v.begin());
    ld = v;

    return 0;
}
