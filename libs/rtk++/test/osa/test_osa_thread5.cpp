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
#include "rtk_test_module.h"

using namespace rtk;

int osa_test_thread5(void);
static OSA_RESULT _thread5_proc(OSA_T_ARGS arg);

OSA_HANDLE hThread5;
static ru32 thread5_data = 0;
	
int osa_test_thread5(void)
{
	OSA_RESULT res;
	ru32 suspend_time = 3000;
	
	test_db("[Function] osa_test_thread1()\n");
	test_db("[Description] Test self suspend\n");
	test_db("[Description] If you cann't 'Success' message this test failed!\n");

	thread5_data = 0;

	test_db("[MSG 1] Create self suspend thread\n");
	res = osa_t_create(&hThread5, _thread5_proc, 0, 16*1024, 0);
	if( 0 != res ) {
		test_db("[Error] Failed to create thread\n");
		test_db("[Result] Failed\n");
	}

	test_db("[MSG 2] Wait for %d seconds\n", suspend_time / 1000);
	osa_t_sleep(suspend_time);

	test_db("[MSG 3] Resume self suspend thread\n");
	res = osa_t_resume(hThread5);
	if( 0 != res ) {
		test_db("[ERROR] Fail at osa_t_resume, res=%d\n", res);
		test_db("[RESULT] Fail\n");

		return 0;
	}

	res = osa_t_sleep(2000);
	if( res != 0 ) {
		test_db("[ERROR] fail at osa_t_sleep, res=%d\n", res);
		test_db("[RESULT] Fail\n");

		return 0;
	}

	if( thread5_data == 1 )
		test_db("[RESULT] Success\n");
	else
		test_db("[RESULT] Filed\n");
    test_db("\n");

	osa_t_kill(hThread5);
	osa_t_delete(hThread5);

	return 0;
}

OSA_RESULT _thread5_proc(OSA_T_ARGS arg)
{
	OSA_RESULT res;
	
	test_db("[MSG 4] Thread5 running\n");
	test_db("[MSG 5] Suspend myself\n");

	res = osa_t_suspend(0);
	if( 0 != res ) {
		test_db("[ERROR] Fail at osa_t_suspend(NULL), res=%d\n", res);
		test_db("[RESULT] Fail\n");
		return 0;
	}

	test_db("[MSG6] Resumed by main thread\n");

	thread5_data = 1;

	return 0;
}

