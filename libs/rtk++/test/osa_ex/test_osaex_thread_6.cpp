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

int osa_test_thread6(void);
static OSA_RESULT _thread6_proc(OSA_T_ARGS arg);

OSA_HANDLE hThread6;

int osa_test_thread6(void)
{
    OSA_RESULT  res;
    ru32      trial_num = 1000;
    ru32      i;
	
    printf("[Function] osa_test_thread6()\n");
    printf("[Description] Test thread memory leak\n");
    printf("[Description] If you cann't 'Success' message this test failed!\n");

    for(i = 1 ; i<=trial_num; i++) {
        res = osa_t_create(&hThread6, _thread6_proc, 0);
		if( 0 != res ) {
            printf("[ERROR] Fail at osa_t_create of thread[%d]\n", i);
            printf("[RESULT] Fail\n");

            osa_t_sleep(1000);

			return 0;
		}
		
        osa_t_sleep(10);

		res = osa_t_kill(hThread6);
        if( res != 0 ) {
            printf("[ERROR] Fail at osa_t_kill of thread[%d], res=%d\n",
                   osa_t_getid(hThread6), res);
            printf("[RESULT] Fail\n");
            osa_t_sleep(1000);

			return 0;
		}

		res = osa_t_delete(hThread6);
		if( 0 != res ) {
            printf("[ERROR] Fail at osa_t_create of thread[%d], res=%d\n",
                   osa_t_getid(hThread6), res);
            printf("[RESULT] Fail\n");
            osa_t_sleep(1000);

			return 0;
		}
    }

    printf("[RESULT] Success\n");
    printf("\n");

	return 0;
}

OSA_RESULT _thread6_proc(OSA_T_ARGS arg)
{
    printf("\n[MSG 1] Dummy thread was created [%5d]\n", osa_t_getid());

    // suspend forever
	osa_t_suspend(0);
	
	return 0;
}

int main(int argc, char *argv[])
{
    dbg_stacktrace_setup();
    return osa_test_thread6();
}
