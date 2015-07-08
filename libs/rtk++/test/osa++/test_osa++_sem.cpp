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

RSemaphore  *_test_semaphore1;
ru32      _test_sm1_ready;

class Thread_Sem : public RThread
{
public:
    Thread_Sem() {}
    ~Thread_Sem() {}

    virtual int thread_func(void *arg)
    {
        OSA_RESULT res;

        res = osa_t_sleep(3000);
        if( res != 0 ) {
            printf("[ERROR] Fail at osa_t_sleep, res=%d\n", res);
            printf("[RESULT] Fail\n");

            return 0;
        }

        _test_sm1_ready = true;

        printf("[MSG 2] signal sem\n");
        res = _test_semaphore1->signal();
        if( res != 0 ) {
            printf("[ERROR] Fail at osa_sem_signal, res=%d\n", res);
            printf("[RESULT] Fail\n");

            return 0;
        }

        return 0;
    }
};


int test_semaphore1(CParamArray *pa)
{
    OSA_RESULT  res;
    Thread_Sem  t;

    printf("[Function] osa_test_semaphore1()\n");
    printf("[Description] To test the basic functinality of mutex\n");
    printf("[Description] If you cann't 'Success' message this test failed!\n");

    _test_sm1_ready = false;

    /* Create semaphore with count 0 */
    _test_semaphore1 = new RSemaphore();

    /* Create a thread */
    res = t.start();
    if( 0 != res ) {
        printf("[ERROR] Fail at osa_t_create, res=%d\n", res);
        printf("[RESULT] Fail\n");

        return 0;
    }

    /* Wait for locking semaphore */
    printf("[MSG 1] wait for signal\n");
    res = _test_semaphore1->wait();
    if( 0 != res ) {
        printf("[ERROR] Fail at osa_sem_wait, res=%d\n", res);
        printf("[RESULT] Fail\n");

        return 0;
    }
    printf("[MSG 1] wait signal successful\n");


    if (_test_sm1_ready == false) {
        printf("[ERROR] The osa_sem_wait didn['t wait until sm is available\n");
        printf("[RESULT] Fail\n");

        return 0;
    }

    /* Try to lock semaphore without waiting: Expect OSA_WAIT_NO */
    res = _test_semaphore1->waitTimeout(OSA_WAIT_NO);
    if( res != E_OSA_SEM_TIMEOUT ) {
        printf("[ERROR] Fail at osa_sem_wait_timeout, res=%d\n", res);
        printf("[MSG 1] E_OSA_SEM_TIMEOUT(%d) must be returned\n", E_OSA_SEM_TIMEOUT);
        printf("[RESULT] Fail\n");

        return 0;
    }

    /* Try to lock semaphore with timeout: Expect E_OSA_SEM_TIMEOUT */
    res = _test_semaphore1->waitTimeout(100);
    if( res != E_OSA_SEM_TIMEOUT ) {
        printf("[ERROR] Fail at osa_sem_wait, res=%d\n", res);
        printf("[MSG 2] E_OSA_SEM_TIMEOUT(%d) must be returned\n", E_OSA_SEM_TIMEOUT);
        printf("[RESULT] Fail\n");

        return 0;
    }

    /* Delete semaphore */
    delete _test_semaphore1;


    t.kill();
    t.free();

    printf("\n[RESULT] Success\n");

    return 0;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct RTK_TestFunctionArray g_fa[] =
{
    RTK_FUNC_TEST_DEF(test_semaphore1,      "Semaphore 1"),


    {NULL,  "NULL",  "NULL"},
};

int main(int argc, char *argv[])
{
    CParamArray     pa;

    return rtk_test_main(argc, argv, g_fa, pa);
}
