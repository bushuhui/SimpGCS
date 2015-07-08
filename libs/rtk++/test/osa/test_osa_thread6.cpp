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

int osa_test_thread6(void);
static OSA_RESULT _thread6_proc(OSA_T_ARGS arg);

OSA_HANDLE hThread6;

int osa_test_thread6(void)
{
	OSA_RESULT res;
	ru32 trial_num = 1000;
	ru32 i;
	
	test_db("[Function] osa_test_thread6()\n");
	test_db("[Description] Test thread memory leak\n");
	test_db("[Description] If you cann't 'Success' message this test failed!\n");

	res = osa_t_create(&hThread6, _thread6_proc, 0, 16*1024, 0);
	if( 0 != res ) {
		test_db("[Error] Failed to create thread\n");
		test_db("[Result] Failed\n");
	}

    for(i = 1 ; i<=trial_num; i++)
    {
		res = osa_t_create(&hThread6, _thread6_proc, 0, 16*1024, 0);
		if( 0 != res ) {
			test_db("[ERROR] Fail at osa_t_create of thread[%d]\n", i);
			test_db("[RESULT] Fail\n");

			return 0;
		}
		
        osa_t_sleep(100);

        if(i%50 == 0)
            test_db("[MSG 2] Thread[%d] was created\n", i);

		res = osa_t_kill(hThread6);
		if( res != 0 ) {
			test_db("[ERROR] Fail at osa_t_kill of thread[%d], res=%d\n", i, res);
			test_db("[RESULT] Fail\n");
			return 0;
		}

		res = osa_t_delete(hThread6);
		if( 0 != res ) {
			test_db("[ERROR] Fail at osa_t_create of thread[%d], res=%d\n", i, res);
			test_db("[RESULT] Fail\n");
			return 0;
		}

        osa_t_sleep(100);
    }

	test_db("[RESULT] Success\n");
    test_db("\n");

	return 0;
}

OSA_RESULT _thread6_proc(OSA_T_ARGS arg)
{
	test_db("[MSG 1] Dummy thread was created\n");
	osa_t_suspend(0);
	
	return 0;
}

