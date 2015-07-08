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

#include "rtk_types.h"
#include "rtk_utils.h"

using namespace rtk;

class Test_Image
{
public:
    template<class T>
    int print(RTK_Point2<T> &p);
};

template<class T>
int Test_Image::print(RTK_Point2<T> &p)
{
    printf("%f %f\n", p.x, p.y);
    return 0;
}

int test_basic_types(CParamArray *pa)
{
    // test 1
    RTK_Point2<float>   p1, p2;

    p1.x = 10;
    p1.y = 20;
    p2.x = 20;
    p2.y = 30;

    printf("p1: %f %f\n", p1.x, p1.y);
    printf("p2: %f %f\n", p2.x, p2.y);

    // test 2
    Test_Image img;
    img.print(p1);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct RTK_TestFunctionArray g_fa[] =
{
    RTK_FUNC_TEST_DEF(test_basic_types,             "basic types"),

    {NULL,  "NULL",  "NULL"},
};


int main(int argc, char *argv[])
{
    CParamArray     pa;

    return rtk_test_main(argc, argv, g_fa, pa);
}
