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

class Thread1 : public RThread
{
public:
    Thread1() {}
    ~Thread1() {}

    virtual int thread_func(void *arg)
    {
        printf("[MSG 1 ] Thread1 running\n");
        printf("[Result] Success\n");
        printf("\n");

        return 0;
    }
};

int test_thread1(CParamArray *pa)
{
    printf("[Function   ] test_thread1()\n");
    printf("[Description] Test thread create\n");
    printf("[Description] If you cann't 'Success' message this test failed!\n\n");

    Thread1 t;

    t.start();
    t.wait();

    printf("test_thread1 finished\n");

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Thread2 : public RThread
{
public:
    Thread2() {}
    ~Thread2() {}

    virtual int thread_func(void *arg)
    {
        int     res;
        ru32  i = 0;
        ru32  iSleepTime = 2000;

        printf("[MSG 1] Thread2 running\n");

        while(i<20) {
            printf("[MSG 1] Running in loop: %d\n", i);
            if( i%5 == 0 ) {
                printf("[MSG 2] Sleeping for %d ms\n", iSleepTime);
                res = sleep(iSleepTime);
                if( 0 != res ) {
                    printf("[ERROR] failed to sleep\n");
                    printf("[RESULT] Failed\n");

                    return 0;
                }
            }

            i++;
        }

        printf("[Result] Success\n");
        printf("\n");

        return 0;
    }
};

int test_thread2(CParamArray *pa)
{
    OSA_RESULT res;

    printf("[Function] osa_test_thread2()\n");
    printf("[Description] Test thread sleep\n");
    printf("[Description] If you cann't 'Success' message this test failed!\n");

    Thread2 t;
    t.start();
    t.wait();


    return 0;
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Thread3 : public RThread
{
public:
    Thread3() {}
    ~Thread3() {}

    virtual int thread_func(void *arg)
    {
        size_t  *p_arg = (size_t*) arg;

        printf("[MSG 1] Thread3 running\n");

        if( *p_arg != m_val ) {
            printf("[ERROR] input arg error!\n");
            printf("[RESULT] Failed\n");
            return 0;
        }

        printf("[RESULT] Success\n");
        printf("\n");

        return 0;
    }

public:
    size_t      m_val;
};



int test_thread3(CParamArray *pa)
{
    printf("[Function] osa_test_thread3()\n");
    printf("[Description] Test thread agrument passing\n");
    printf("[Description] If you cann't 'Success' message this test failed!\n");

    Thread3 t;
    size_t  arg;

    arg = 1245;
    t.m_val = arg;

    t.start(&arg);
    t.wait();

    return 0;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static ru32   _thread4_data = 0;

class Thread4 : public RThread
{
public:
    Thread4() {}
    ~Thread4() {}

    virtual int thread_func(void *arg)
    {
        printf("[MSG  1] Thread4 running\n");

        _thread4_data = 1;

        while(_thread4_data <= 40)
        {
            printf("[MSG 15] Counting thread count: %d\n", _thread4_data);
            osa_t_sleep(1000);
            _thread4_data ++;
        }

        printf("[MSG  1] Thread4 run over!\n");

        return 0;
    }
};


int test_thread4(CParamArray *pa)
{
    OSA_RESULT res;
    ru32 sleep_time = 5000;
    ru32 data;

    printf("[Function] test_thread4()\n");
    printf("[Description] Test thread suspend/resume\n");
    printf("[Description] If you cann't 'Success' message this test failed!\n");

    Thread4 t;

    res = t.start();
    if( 0 != res ) {
        printf("[Error] Failed to create thread\n");
        printf("[Result] Failed\n");
    }

    printf("[MSG  2] Wait for %d seconds\n", sleep_time / 1000);
    osa_t_sleep(sleep_time);

    printf("[MSG  3] Suspend counting thread\n");
    res = t.suspend();
    if( 0 != res ) {
        printf("[ERROR] Failed to suspend, res=%d\n", res);
        printf("[RESULT] Failed\n");
        return 0;
    }

    printf("[MSG  4] Try to suspend the supended thread\n");
    res = t.suspend();
    if( E_OSA_T_SUSPEND != res ) {
        printf("[ERROR] Failed at osa_t_suspend, res=%d\n", res);
        printf("[ERROR] E_OSA_T_SUSPEND must be returned\n");
        printf("[RESULT] Failed\n");

        return 0;
    }

    data = _thread4_data;
    printf("[MSG  5] After suspend, counting data is %d\n", data);
    printf("[MSG  6] Wait for %d seconds\n", sleep_time / 1000);
    osa_t_sleep(sleep_time);
    printf("[MSG  7] After sleep, counting data is %d\n", _thread4_data);
    if( data != _thread4_data ) {
        printf("[ERROR] Fail at osa_t_suspend\n");
        printf("[MSG 8] osa_t_suspend didn't work properly\n");
        printf("[RESULT] Fail\n");

        return 0;
    }

    printf("[MSG  8] Resume counting thread(1)\n");
    res = t.resume();
    if( res != E_OSA_T_SUSPEND ) {
        printf("[ERROR] Fail at osa_t_resume, res=%d\n", res);
        printf("[MSG 9] osa_t_resume must return E_OSA_T_SUSPEND\n");
        printf("[RESULT] Fail\n");
        return 0;
    }

    printf("[MSG  9] Resume counting thread(2)\n");
    res = t.resume();
    if( res != 0 ) {
        printf("[ERROR] Fail at osa_t_resume, res=%d\n", res);
        printf("[RESULT] Fail\n");
        return 0;
    }

    printf("[MSG 10] Try to resume running thread\n");
    res = t.resume();
    if( res != E_OSA_T_RUNNING ) {
        printf("[ERROR] Fail at osa_t_resume, res=%d\n", res);
        printf("[MSG 11] E_OSA_T_RUNNING must be returned\n");
        printf("[RESULT] Fail\n");
        return 0;
    }

    data = _thread4_data;
    printf("[MSG 11] After resume counting data is %d\n", data);
    printf("[MSG 12] Wait for %d seconds\n", sleep_time / 1000);
    osa_t_sleep(sleep_time);
    printf("[MSG 13] After sleep, counting data is %d\n", _thread4_data);
    if( data == _thread4_data ) {
        printf("[ERROR] Fail at osa_t_resume\n");
        printf("[MSG 14] osa_t_resume didn't work properly\n");
        printf("[RESULT] Fail\n");

        return 0;
    }

    printf("[RESULT] Success\n");
    printf("\n");

    t.kill();

    return 0;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static ru32 thread5_data = 0;

class Thread5 : public RThread
{
public:
    Thread5() {}
    ~Thread5() {}

    virtual int thread_func(void *arg)
    {
        OSA_RESULT res;

        printf("[MSG 4] Thread5 running\n");
        printf("[MSG 5] Suspend myself\n");

        res = suspend();
        if( 0 != res ) {
            printf("[ERROR] Fail at osa_t_suspend(NULL), res=%d\n", res);
            printf("[RESULT] Fail\n");
            return 0;
        }

        printf("[MSG 6] Resumed by main thread\n");

        thread5_data = 1;

        return 0;
    }
};



int test_thread5(CParamArray *pa)
{
    OSA_RESULT res;
    ru32 suspend_time = 3000;

    printf("[Function] test_thread5()\n");
    printf("[Description] Test self suspend\n");
    printf("[Description] If you cann't 'Success' message this test failed!\n");

    thread5_data = 0;

    printf("[MSG 1] Create self suspend thread\n");
    Thread5 t;

    res = t.start();
    if( 0 != res ) {
        printf("[Error] Failed to create thread\n");
        printf("[Result] Failed\n");
    }

    printf("[MSG 2] Wait for %d seconds\n", suspend_time / 1000);
    osa_t_sleep(suspend_time);

    printf("[MSG 3] Resume self suspend thread\n");
    res = t.resume();
    if( 0 != res ) {
        printf("[ERROR] Fail at osa_t_resume, res=%d\n", res);
        printf("[RESULT] Fail\n");

        return 0;
    }

    res = osa_t_sleep(2000);
    if( res != 0 ) {
        printf("[ERROR] fail at osa_t_sleep, res=%d\n", res);
        printf("[RESULT] Fail\n");

        return 0;
    }

    if( thread5_data == 1 )
        printf("[RESULT] Success\n");
    else
        printf("[RESULT] Filed\n");
    printf("\n");

    t.kill();

    return 0;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Thread6 : public RThread
{
public:
    Thread6() {}
    ~Thread6() {}

    virtual int thread_func(void *arg)
    {
        printf("\n[MSG 1] Dummy thread was created [%5d]\n", get_id());

        // suspend forever
        osa_t_suspend(0);

        return 0;
    }
};


int test_thread6(CParamArray *pa)
{
    OSA_RESULT  res;
    ru32      trial_num = 1000;
    ru32      i;

    printf("[Function] test_thread6()\n");
    printf("[Description] Test thread memory leak\n");
    printf("[Description] If you cann't 'Success' message this test failed!\n");

    Thread6 *t = NULL;

    for(i=1; i<=trial_num; i++) {
        t = new Thread6;

        res = t->start();
        if( 0 != res ) {
            printf("[ERROR] Fail at osa_t_create of thread[%d]\n", i);
            printf("[RESULT] Fail\n");

            osa_t_sleep(1000);

            return 0;
        }

        osa_t_sleep(10);

        res = t->kill();
        if( res != 0 ) {
            printf("[ERROR] Fail at osa_t_kill of thread[%d], res=%d\n",
                   t->get_id(), res);
            printf("[RESULT] Fail\n");
            osa_t_sleep(1000);

            return 0;
        }

        res = t->free();
        if( 0 != res ) {
            printf("[ERROR] Fail at osa_t_create of thread[%d], res=%d\n",
                   t->get_id(), res);
            printf("[RESULT] Fail\n");
            osa_t_sleep(1000);

            return 0;
        }

        delete t;
        t = NULL;
    }

    printf("[RESULT] Success\n");
    printf("\n");

    return 0;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


class Thread7 : public RThread
{
public:
    Thread7() {}
    ~Thread7() {}

    virtual int thread_func(void *arg)
    {
        int     i=0;

        printf("\n[MSG 1] Dummy thread was created [%d]\n", get_id());

        // endless loop
        while(1) {
            osa_t_sleep(5);
            printf("[MSG 1] i = %6d [%d]\n", i++, get_id());
        }

        return 0;
    }
};


int test_thread7(CParamArray *pa)
{
    OSA_RESULT  res;
    ru32      trial_num = 1000;
    ru32      i;

    printf("[Function] osa_test_thread6()\n");
    printf("[Description] Test thread memory leak\n");
    printf("[Description] If you cann't 'Success' message this test failed!\n");

    Thread7 *t = NULL;

    for(i=1; i<=trial_num; i++) {
        // create a thread
        t = new Thread7;

        res = t->start();
        if( 0 != res ) {
            printf("[ERROR] Fail at osa_t_create of thread[%d]\n", i);
            printf("[RESULT] Fail\n");

            osa_t_sleep(1000);

            return 0;
        }

        // sleep for some time
        //osa_t_sleep(10);
        t->wait(200);

        // kill created thread
        res = t->kill();
        if( res != 0 ) {
            printf("[ERROR] Fail at osa_t_kill of thread[%d], res=%d\n",
                   t->get_id(), res);
            printf("[RESULT] Fail\n");
            osa_t_sleep(500);

            return 0;
        }

        // delete thread resource
        res = t->free();
        if( 0 != res ) {
            printf("[ERROR] Fail at osa_t_create of thread[%d], res=%d\n",
                   t->get_id(), res);
            printf("[RESULT] Fail\n");
            osa_t_sleep(500);

            return 0;
        }

        osa_t_sleep(100);
    }

    printf("[RESULT] Success\n");
    printf("\n");

    return 0;
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct RTK_TestFunctionArray g_fa[] =
{
    RTK_FUNC_TEST_DEF(test_thread1,             "Thread basic usage"),
    RTK_FUNC_TEST_DEF(test_thread2,             "Thread sleep"),
    RTK_FUNC_TEST_DEF(test_thread3,             "Thread argument"),
    RTK_FUNC_TEST_DEF(test_thread4,             "Thread suspend/resume"),
    RTK_FUNC_TEST_DEF(test_thread5,             "Thread self suspend"),
    RTK_FUNC_TEST_DEF(test_thread6,             "Thread memory leakage test 1"),
    RTK_FUNC_TEST_DEF(test_thread7,             "Thread memory leakage test 2"),

    {NULL,  "NULL",  "NULL"},
};

int main(int argc, char *argv[])
{
    CParamArray     pa;

    return rtk_test_main(argc, argv, g_fa, pa);
}
