#ifndef __MAVLINK_UTILS_H__
#define __MAVLINK_UTILS_H__

#include <stdint.h>

#include <string>
#include <list>
#include <vector>

#include <mavlink/v1.0/common/mavlink.h>

typedef std::vector<std::string> mavlink_nameList;

////////////////////////////////////////////////////////////////////////////////
/// enum to name
////////////////////////////////////////////////////////////////////////////////
int mavlink_autopilot_name(uint8_t ap, char **name);
int mavlink_mav_type_name(uint8_t mt, char **name);
int mavlink_mav_mode_name(uint8_t mf, mavlink_nameList &nl);
int mavlink_mav_state_name(uint8_t ms, char **name);

////////////////////////////////////////////////////////////////////////////////
/// MAV_SYS_STATUS_SENSOR
////////////////////////////////////////////////////////////////////////////////
#define MAV_SYS_STATUS_SENSOR_ID_NAME_DEF(id, name, desc) { id, name, desc }

struct MAVLINK_SYS_STATUS_SENSOR_IDNAME_Struct {
    uint32_t    id;
    char        name[256];
    char        desc[256];
};

typedef  std::vector<uint32_t>     mavlink_sys_status_sensor_list;
typedef  std::vector<std::string>  mavlink_sys_status_sensor_namelist;

int mavlink_sys_status_sensor_getIDs(uint32_t s, mavlink_sys_status_sensor_list &l);
int mavlink_sys_status_sensor_getName(uint32_t id, char **name);
int mavlink_sys_status_sensor_getNames(mavlink_sys_status_sensor_list &l,
                                        mavlink_sys_status_sensor_namelist &nl);

int mavlink_sys_status_sensor_getID_Difference(mavlink_sys_status_sensor_list &l1,
                                               mavlink_sys_status_sensor_list &l2,
                                               mavlink_sys_status_sensor_list &ld);


////////////////////////////////////////////////////////////////////////////////
/// utils
////////////////////////////////////////////////////////////////////////////////


/**
 *  Value averager
 */
template<class T>
class ValueAverager
{
public:
    ValueAverager() {
        init();

        valArray.resize(nMax);
    }

    ValueAverager(int n) {
        init();

        nMax = n;
        valArray.resize(nMax);
    }

    ~ValueAverager() {
        init();
    }

    T push(T v) {
        valArray[idx%nMax] = v;
        num ++;
        idx ++;

        return calcAvg();
    }

    T calcAvg(void) {
        T   avg;
        int i;

        avg = 0;
        if( idx < nMax ) {
            for(i=0; i<idx; i++) avg += valArray[i];

            avg = avg / idx;
        } else {
            for(i=0; i<nMax; i++) avg += valArray[i];

            avg = avg / nMax;
        }

        return avg;
    }

    void setSize(int n) {
        init();

        nMax = n;
        valArray.resize(nMax);
    }

    void init(void) {
        nMax = 10;
        num  = 0;
        idx  = 0;
        valArray.clear();
    }

public:
    int                 nMax, num, idx;
    std::vector<T>      valArray;
};


#endif // end of __MAVLINK_UTILS_H__
