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
#include "rtk_debug.h"

using namespace rtk;

int osa_test_thread5(void);
static OSA_RESULT _thread5_proc(OSA_T_ARGS arg);

OSA_HANDLE hThread5;
static ru32 thread5_data = 0;
	
int osa_test_thread5(void)
{
	OSA_RESULT res;
	ru32 suspend_time = 3000;
	
    printf("[Function] osa_test_thread1()\n");
    printf("[Description] Test self suspend\n");
    printf("[Description] If you cann't 'Success' message this test failed!\n");

	thread5_data = 0;

    printf("[MSG 1] Create self suspend thread\n");
    res = osa_t_create(&hThread5, _thread5_proc, 0);
	if( 0 != res ) {
        printf("[Error] Failed to create thread\n");
        printf("[Result] Failed\n");
	}

    printf("[MSG 2] Wait for %d seconds\n", suspend_time / 1000);
	osa_t_sleep(suspend_time);

    printf("[MSG 3] Resume self suspend thread\n");
	res = osa_t_resume(hThread5);
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

	osa_t_kill(hThread5);
	osa_t_delete(hThread5);

	return 0;
}

OSA_RESULT _thread5_proc(OSA_T_ARGS arg)
{
	OSA_RESULT res;
	
    printf("[MSG 4] Thread5 running\n");
    printf("[MSG 5] Suspend myself\n");

	res = osa_t_suspend(0);
	if( 0 != res ) {
        printf("[ERROR] Fail at osa_t_suspend(NULL), res=%d\n", res);
        printf("[RESULT] Fail\n");
		return 0;
	}

    printf("[MSG6] Resumed by main thread\n");

	thread5_data = 1;

	return 0;
}

int main(int argc, char *argv[])
{
    dbg_stacktrace_setup();
    return osa_test_thread5();
}

