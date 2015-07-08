#include <stdio.h>
#include <math.h>
#include <time.h>

#include <sys/time.h>

#include "utils_GPS.h"


namespace rtk {


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


#define EARTH_RADIUS 6378137.0  ///< Earth radius (unit: m)
#define sqr(x) ((x)*(x))

double calc_earth_dis(double long_1, double lat_1, double long_2, double lat_2)
{
    double          radLat1, radLat2;
    double          a, b, s;

    radLat1 = lat_1*M_PI/180.0;
    radLat2 = lat_2*M_PI/180.0;

    a = radLat1 - radLat2;
    b = (long_1 - long_2)*M_PI/180.0;

    s = 2*asin( sqrt(sqr(sin(a/2)) + cos(radLat1)*cos(radLat2)*sqr(b/2)) );
    s = s*EARTH_RADIUS;
    //s = round(s*10000)/10000.0;

    return s;
}


double calc_longitude_unit(double lat)
{
    double      a, f, e_2, l, l2;
    double      phi, phi_rad;
    int         i;

    a = EARTH_RADIUS;
    f = 1.0/298.257222101;
    e_2 = 2*f - f*f;

    phi_rad = lat*M_PI/180.0;

    l  = M_PI/180.0 * a * cos(phi_rad) / sqrt(1 - e_2*sin(phi_rad)*sin(phi_rad));
    l2 = a*cos(phi_rad) * M_PI/180.0;

    return l;

}

int calc_earth_offset(double lng1, double lat1,
                      double lng2, double lat2,
                      double &dx, double &dy)
{
    double      lng_unit;
    double      a;

    a = EARTH_RADIUS;

    lng_unit = calc_longitude_unit(lat1);
    dx = (lng2 - lng1) * lng_unit;
    dy = a*(lat2 - lat1)*M_PI/180.0;

    return 0;
}

} // end of namespace rtk
