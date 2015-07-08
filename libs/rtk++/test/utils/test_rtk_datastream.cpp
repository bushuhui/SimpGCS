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

#include <string>
#include <vector>
#include <iostream>

#include "rtk_debug.h"
#include "rtk_utils.h"
#include "rtk_datetime.h"
#include "rtk_datastream.h"

using namespace std;
using namespace rtk;


int test_datastream(CParamArray *pa)
{
    ri8             i1;
    ru8             i2;
    ri16            i3;
    ru16            i4;
    ri32            i5;
    ru32            i6;
    ri64            i7;
    ru64            i8;
    float           f1;
    double          f2;

    string          s, s2;

    int             i;
    RDataStream      ds, ds2, ds3;


    i1 = 1;     i2 = 2;
    i3 = 3;     i4 = 4;
    i5 = 5;     i6 = 6;
    i7 = 7;     i8 = 8;
    f1 = 0.315345;  f2 = 234.0231e10;
    s = "hello xxxx";

    ds << i1 << i2 << i3 << i4 << i5 << i6 << i7 << i8;
    ds << f1 << f2;
    ds << s;


    i1 = 34;        i2 = 56;
    i3 = 12341;     i4 = 42341;
    i5 = 862331;    i6 = 345782;
    i7 = 67851;     i8 = 945256;
    f1 = 5629.123;  f2 = 985.234e10;
    s = "hello yyyy";

    ds2 << i1 << i2 << i3 << i4 << i5 << i6 << i7 << i8;
    ds2 << f1 << f2;
    ds2 << s;
    ds << ds2;

    printf("ds.size()  = %d\n", ds.size());
    printf("ds2.size() = %d\n", ds2.size());


    // print ds1
    printf("===== ds1 =====\n");

    ds.rewind();
    s2 = "";
    ds >> i1 >> i2 >> i3 >> i4 >> i5 >> i6 >> i7 >> i8;
    ds >> f1 >> f2;
    ds >> s2;
    ds >> ds3;

    i = 0;
    printf("[%02d] %d\n", i++, i1);
    printf("[%02d] %d\n", i++, i2);
    printf("[%02d] %d\n", i++, i3);
    printf("[%02d] %d\n", i++, i4);
    printf("[%02d] %d\n", i++, i5);
    printf("[%02d] %d\n", i++, i6);
    printf("[%02d] %lld\n", i++, i7);
    printf("[%02d] %lld\n", i++, i8);
    printf("[%02d] %f\n", i++, f1);
    printf("[%02d] %f\n", i++, f2);
    printf("[%02d] %s\n", i++, s2.c_str());

    // print ds2
    printf("\n\n===== ds2 =====\n");

    printf("ds2.size() = %d\n", ds2.size());
    ds2.rewind();
    s2 = "";

    ds2 >> i1 >> i2 >> i3 >> i4 >> i5 >> i6 >> i7 >> i8;
    ds2 >> f1 >> f2;
    ds2 >> s2;

    i=0;
    printf("[%02d] %d\n", i++, i1);
    printf("[%02d] %d\n", i++, i2);
    printf("[%02d] %d\n", i++, i3);
    printf("[%02d] %d\n", i++, i4);
    printf("[%02d] %d\n", i++, i5);
    printf("[%02d] %d\n", i++, i6);
    printf("[%02d] %lld\n", i++, i7);
    printf("[%02d] %lld\n", i++, i8);
    printf("[%02d] %f\n", i++, f1);
    printf("[%02d] %f\n", i++, f2);
    printf("[%02d] %s\n", i++, s2.c_str());


    // print ds3
    printf("\n\n===== ds3 =====\n");

    printf("ds3.size() = %d\n", ds3.size());
    ds3.rewind();

    s2 = "";

    ds3 >> i1 >> i2 >> i3 >> i4 >> i5 >> i6 >> i7 >> i8;
    ds3 >> f1 >> f2;
    ds3 >> s2;

    i=0;
    printf("[%02d] %d\n", i++, i1);
    printf("[%02d] %d\n", i++, i2);
    printf("[%02d] %d\n", i++, i3);
    printf("[%02d] %d\n", i++, i4);
    printf("[%02d] %d\n", i++, i5);
    printf("[%02d] %d\n", i++, i6);
    printf("[%02d] %lld\n", i++, i7);
    printf("[%02d] %lld\n", i++, i8);
    printf("[%02d] %f\n", i++, f1);
    printf("[%02d] %f\n", i++, f2);
    printf("[%02d] %s\n", i++, s2.c_str());

    return 0;
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct RTK_TestFunctionArray g_fa[] =
{
    RTK_FUNC_TEST_DEF(test_datastream,              "Test RDataStream basic usage"),

    {NULL,  "NULL",  "NULL"},
};


int main(int argc, char *argv[])
{
    CParamArray     pa;

    return rtk_test_main(argc, argv, g_fa, pa);
}
