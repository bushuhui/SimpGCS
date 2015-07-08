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

int osa_test_thread4(void);
static OSA_RESULT _thread4_proc(OSA_T_ARGS arg);

OSA_HANDLE hThread4;
static ru32 _thread4_data = 0;


int osa_test_thread4(void)
{
	OSA_RESULT res;
	ru32 sleep_time = 5000;
	ru32 data;
	
	test_db("[Function] osa_test_thread4()\n");
	test_db("[Description] Test thread suspend/resume\n");
	test_db("[Description] If you cann't 'Success' message this test failed!\n");

	res = osa_t_create(&hThread4, _thread4_proc, 0, 16*1024, 0);
	if( 0 != res ) {
		test_db("[Error] Failed to create thread\n");
		test_db("[Result] Failed\n");
	}

	test_db("[MSG 2] Wait for %d seconds\n", sleep_time / 1000);
	osa_t_sleep(sleep_time);

	test_db("[MSG 3] Suspend counting thread\n");
	res = osa_t_suspend(hThread4);
	if( 0 != res ) {
		test_db("[ERROR] Failed to suspend, res=%d\n", res);
		test_db("[RESULT] Failed\n");
		return 0;
	}

	test_db("[MSG 4] Try to suspend the supended thread\n");
	res = osa_t_suspend(hThread4);
	if( E_OSA_T_SUSPEND != res ) {
		test_db("[ERROR] Failed at osa_t_suspend, res=%d\n", res);
		test_db("[ERROR] E_OSA_T_SUSPEND must be returned\n");
		test_db("[RESULT] Failed\n");

		return 0;
	}

    data = _thread4_data;
	test_db("[MSG 5] After suspend, counting data is %d\n", data);
	test_db("[MSG 6] Wait for %d seconds\n", sleep_time / 1000);
	osa_t_sleep(sleep_time);
	test_db("[MSG 7] After sleep, counting data is %d\n", _thread4_data);
	if( data != _thread4_data ) {
		test_db("[ERROR] Fail at osa_t_suspend\n");
		test_db("[MSG 8] osa_t_suspend didn't work properly\n");
		test_db("[RESULT] Fail\n");

		return 0;
	}
	
	test_db("[MSG 8] Resume counting thread(1)\n");
	res = osa_t_resume(hThread4);
	if( res != E_OSA_T_SUSPEND ) {
		test_db("[ERROR] Fail at osa_t_resume, res=%d\n", res);
		test_db("[MSG 9] osa_t_resume must return E_OSA_T_SUSPEND\n");
		test_db("[RESULT] Fail\n");
		return 0;
	}
	
	test_db("[MSG 9] Resume counting thread(2)\n");
	res = osa_t_resume(hThread4);
	if( res != 0 ) {
		test_db("[ERROR] Fail at osa_t_resume, res=%d\n", res);
		test_db("[RESULT] Fail\n");
		return 0;
	}

	test_db("[MSG 10] Try to resume running thread\n");
	res = osa_t_resume(hThread4);
	if( res != E_OSA_T_RUNNING ) {
		test_db("[ERROR] Fail at osa_t_resume, res=%d\n", res);
		test_db("[MSG 11] E_OSA_T_RUNNING must be returned\n");
		test_db("[RESULT] Fail\n");
		return 0;
	}
	
    data = _thread4_data;
	test_db("[MSG 11] After resume counting data is %d\n", data);
	test_db("[MSG 12] Wait for %d seconds\n", sleep_time / 1000);
	osa_t_sleep(sleep_time);
	test_db("[MSG 13] After sleep, counting data is %d\n", _thread4_data);
	if( data == _thread4_data ) {
		test_db("[ERROR] Fail at osa_t_resume\n");
		test_db("[MSG 14] osa_t_resume didn't work properly\n");
		test_db("[RESULT] Fail\n");

		return 0;
	}

	test_db("[RESULT] Success\n");
    test_db("\n");

	osa_t_kill(hThread4);
	osa_t_delete(hThread4);

	return 0;
}

OSA_RESULT _thread4_proc(OSA_T_ARGS arg)
{
	test_db("[MSG 1] Thread4 running\n");

	_thread4_data = 1;

    while(_thread4_data <= 10)
    {
		test_db("[MSG 15] Counting thread count: %d\n", _thread4_data);
		osa_t_sleep(1000);
		_thread4_data ++;
    }

	test_db("[MSG 1] Thread4 run over!\n");
	
	return 0;
}

