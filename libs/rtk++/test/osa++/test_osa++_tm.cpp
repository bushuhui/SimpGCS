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

#include "rtk_osa.h"
#include "rtk_osa++.h"
#include "rtk_utils.h"
#include "rtk_debug.h"

using namespace rtk;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Timer_1 : public RTimer
{
public:
    Timer_1() {}
    ~Timer_1() {}

    virtual void timer_func(void *arg)
    {
        printf("[MSG 1] Timer1 %d\n", m_count);
        fflush(stdout);
    }
};


int test_timer1(CParamArray *pa)
{
    OSA_RESULT  res;
    Timer_1     tm;

    printf("[Function] test_timer1()\n");
    printf("[Description] To test the basic functinality of timer\n");
    printf("[Description] If you cann't 'Success' message this test failed!\n");


    res = tm.start(500);
    if( 0 != res ) {
        printf("[ERROR] Fail to create timer\n");
        printf("[RESULT] Fail\n");

        return 0;
    }

    res = osa_t_sleep(3000);
    if( 0 != res ) {
        printf("[ERROR] Fail at osa_t_sleep, res=%d\n", res);
        printf("[RESULT] Fail\n");

        return 0;
    }

    tm.pause();
    printf("[MSG 2] pause timer for 3000 ms\n");
    osa_t_sleep(3000);

    tm.resume();
    printf("[MSG 2] resume timer\n");
    osa_t_sleep(3000);

    tm.stop();

    printf("\n[RESULT] Success\n");

    return 0;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct RTK_TestFunctionArray g_fa[] =
{
    RTK_FUNC_TEST_DEF(test_timer1,              "Timer test 1"),


    {NULL,  "NULL",  "NULL"},
};

int main(int argc, char *argv[])
{
    CParamArray     pa;

    return rtk_test_main(argc, argv, g_fa, pa);
}
