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

RMutex      *_test_mutex1;

ri32       _test_mutex1_data;
ru32      _test_mutex_res[3];

class Thread_Mutex : public RThread
{
public:
    Thread_Mutex() {}
    ~Thread_Mutex() {}

    virtual int thread_func(void *arg)
    {
        OSA_RESULT  res;
        OSA_RESULT  mu_res;
        ri32       tmp;
        size_t      arg_i;

        arg_i = (size_t) arg;

        printf("[MSG 1] _thread1_proc running\n");

        tmp = (arg_i+1) * 100;
        res = osa_t_sleep(tmp);
        if( 0 != res ) {
            printf("[ERROR] Fail at osa_t_sleep, res=%d\n", res);
            printf("[RESULT] Fail\n");

            return 0;
        }

        mu_res = _test_mutex1->lock();
        if( 0 != mu_res ) {
            printf("[ERROR] Fail at osa_mu_lock, res=%d\n", mu_res);
            printf("[RESULT] Fail\n");

            return 0;
        }

        tmp = (3 - arg_i) * 200;
        res = osa_t_sleep(tmp);
        if( 0 != res ) {
            printf("[ERROR] Fail at osa_t_sleep, res=%d\n", res);
            printf("[RESULT] Fail\n");

            return 0;
        }

        tmp = _test_mutex1_data;
        if (tmp != arg_i) {
            printf("[ERROR] Fail - Mutex didn't work properly\n");
            printf("[RESULT] Fail\n");

            return 0;
        }

        _test_mutex1_data = tmp + 1;

        printf("[MSG 1] Thread[%d]: value=%d\n", (ri32)arg_i, _test_mutex1_data);

        mu_res = _test_mutex1->unlock();
        if( 0 != mu_res ) {
            printf("[ERROR] Fail at osa_mu_unlock, res=%d\n", mu_res);
            printf("[RESULT] Fail\n");

            return 0;
        }

        _test_mutex_res[arg_i] = true;

        return 0;
    }
};


int test_mutex1(CParamArray *pa)
{
    OSA_RESULT  res;
    ru32      i;

    Thread_Mutex    t[3];

    printf("[Function] osa_test_mutex1()\n");
    printf("[Description] To test the basic functinality of mutex\n");
    printf("[Description] If you cann't 'Success' message this test failed!\n");

    _test_mutex1_data = 0;
    for(i=0; i<3; i++)
        _test_mutex_res[i] = false;

    _test_mutex1 = new RMutex();

    for(i=0; i<3; i++) {
        res = t[i].start((void*) ((size_t)i));
        if( 0 != res ) {
            printf("[ERROR] Fail to create thread\n");
            printf("[RESULT] Fail\n");

            return 0;
        }
    }

    res = osa_t_sleep(3000);
    if( 0 != res ) {
        printf("[ERROR] Fail at osa_t_sleep, res=%d\n", res);
        printf("[RESULT] Fail\n");

        return 0;
    }

    delete _test_mutex1;

    for(i=0; i<3; i++) {
        t[i].kill();
        t[i].free();
    }

    if( (_test_mutex_res[0] == true) &&
        (_test_mutex_res[1] == true) &&
        (_test_mutex_res[2] == true) )
        printf("[RESULT] Success\n");

    printf("\n");

    return 0;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct RTK_TestFunctionArray g_fa[] =
{
    RTK_FUNC_TEST_DEF(test_mutex1,              "Thread mutex"),


    {NULL,  "NULL",  "NULL"},
};

int main(int argc, char *argv[])
{
    CParamArray     pa;

    return rtk_test_main(argc, argv, g_fa, pa);
}
