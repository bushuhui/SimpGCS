#ifndef __UTILS_GPS_H__
#define __UTILS_GPS_H__

#include <rtk_types.h>

namespace rtk {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

double calc_earth_dis(double long_1, double lat_1, double long_2, double lat_2);
double calc_longitude_unit(double lat);
int calc_earth_offset(double long1, double lat1, double long2, double lat2,
                      double &dx, double &dy);

} // end of namespace rtk

#endif // end of __UTILS_GPS_H__
