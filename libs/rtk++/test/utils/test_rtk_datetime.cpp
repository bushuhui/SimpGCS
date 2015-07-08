/******************************************************************************

  Robot Toolkit ++ (RTK++)

  Copyright (c) 2007-2013 Shuhui Bu <bushuhui@nwpu.edu.cn>
  http://www.adv-ci.com

  ----------------------------------------------------------------------------

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <string>
#include <vector>
#include <iostream>

#include "rtk_debug.h"
#include "rtk_utils.h"
#include "rtk_datetime.h"

using namespace std;
using namespace rtk;


int test_currentDateTime(CParamArray *pa)
{
    RDateTime        dt;
    RDateTime        t1, t2;

    dt.setCurrentDate();
    cout << "Current date: " << dt << endl;

    dt.setCurrentTime();
    cout << "Current time: " << dt << endl;

    dt.setCurrentDateTime(DATETIME_UTC);
    cout << "Current time: " << dt << "\n\n";

    t1 = dt;

    cout << "Current time (toUTC)      : " << t1.toUTC() << endl;
    cout << "Current time (toLocalTime): " << t1.toLocalTime() << "\n\n";


    t2 = t1.toUTC();
    t1.setCurrentDateTime();
    cout << "t1 : " << t1 << endl;
    cout << "t2 : " << t2 << endl;
    cout << fmt::sprintf("diffTime t1-t2 = %f s\n", t1.diffTime(t2));
    cout << fmt::sprintf("diffTime t2-t1 = %f s\n", t2.diffTime(t1));
    cout << endl;

    t1.toUTC();
    cout << "t1 : " << t1 << endl;
    cout << "t2 : " << t2 << endl;
    cout << fmt::sprintf("diffTime t1-t2 = %f s\n", t1.diffTime(t2));
    cout << fmt::sprintf("diffTime t2-t1 = %f s\n", t2.diffTime(t1));
    cout << endl;

    t1.setTime(8, 10, 12.34);
    cout << "t1 : " << t1 << endl;
    cout << "t2 : " << t2 << endl;
    cout << fmt::sprintf("diffTime t1-t2 = %f s\n", t1.diffTime(t2));
    cout << fmt::sprintf("diffTime t2-t1 = %f s\n", t2.diffTime(t1));
    cout << endl;

    return 0;
}

int test_time_t(CParamArray *pa)
{
    int         i;
    RDateTime    t, t2;
    ri64        ts, ts2;

    ts = t.toTime_t();
    cout << t << "\n";
    printf("ts = %lld\n", ts);

    for(i=0; i<100; i++) {
        t.setCurrentDateTime();
        ts2 = time(NULL);
        ts = t.toTime_t();
        printf("[%04d] ts = %lld, time = %lld, ", i, ts, ts2);

        t2.fromTime_t(ts2);
        cout << t << ", t2 = " << t2 << "\n";

        tm_sleep(10);
    }

    return 0;
}

int test_timeStamp(CParamArray *pa)
{
    int         i;
    RDateTime    t, t2;
    ri64        ts;

    ts = t.toTimeStamp();
    cout << t << "\n";
    printf("ts = %lld\n", ts);

    for(i=0; i<100; i++) {
        t.setCurrentDateTime();
        ts = t.toTimeStamp();
        printf("[%04d] ts = %lld, ", i, ts);

        t2.fromTimeStamp(ts);
        cout << "t1 = " << t << ", t2 = " << t2 << "\n";

        tm_sleep(10);
    }

    return 0;
}

int test_datetime_stream(CParamArray *pa)
{
    RDateTime        dt;
    RDateTime        t1, t2;
    RDataStream      s;

    dt.setCurrentDateTime(DATETIME_UTC);
    cout << "t1: " << dt << endl;
    t1 = dt;

    t2.setCurrentDate();
    cout << "t2: " << t2 << "\n\n";

    t1.toStream(s);
    s.rewind();
    t2.fromStream(s);

    cout << "t2: " << t2 << "\n\n";

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct RTK_TestFunctionArray g_fa[] =
{
    RTK_FUNC_TEST_DEF(test_currentDateTime,         "Set to current date time"),
    RTK_FUNC_TEST_DEF(test_timeStamp,               "Test time stamp"),
    RTK_FUNC_TEST_DEF(test_time_t,                  "Test time_t"),
    RTK_FUNC_TEST_DEF(test_datetime_stream,         "Test datetime stream"),

    {NULL,  "NULL",  "NULL"},
};


int main(int argc, char *argv[])
{
    CParamArray     pa;

    return rtk_test_main(argc, argv, g_fa, pa);
}
