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

int osa_test_thread7(void);
static OSA_RESULT _thread7_proc(OSA_T_ARGS arg);

OSA_HANDLE hThread7;

int osa_test_thread7(void)
{
    OSA_RESULT  res;
    ru32      trial_num = 1000;
    ru32      i;
	
    printf("[Function] osa_test_thread6()\n");
    printf("[Description] Test thread memory leak\n");
    printf("[Description] If you cann't 'Success' message this test failed!\n");

    for(i = 1 ; i<=trial_num; i++) {
        // create a thread
        res = osa_t_create(&hThread7, _thread7_proc, 0);
		if( 0 != res ) {
            printf("[ERROR] Fail at osa_t_create of thread[%d]\n", i);
            printf("[RESULT] Fail\n");

            osa_t_sleep(1000);

			return 0;
		}
		
        // sleep for some time
        //osa_t_sleep(10);
        osa_t_wait(hThread7, 200);

        // kill created thread
		res = osa_t_kill(hThread7);
        if( res != 0 ) {
            printf("[ERROR] Fail at osa_t_kill of thread[%d], res=%d\n",
                   osa_t_getid(hThread7), res);
            printf("[RESULT] Fail\n");
            osa_t_sleep(500);

			return 0;
		}

        // delete thread resource
		res = osa_t_delete(hThread7);
		if( 0 != res ) {
            printf("[ERROR] Fail at osa_t_create of thread[%d], res=%d\n",
                   osa_t_getid(hThread7), res);
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

OSA_RESULT _thread7_proc(OSA_T_ARGS arg)
{
    int     i=0;
    printf("\n[MSG 1] Dummy thread was created [%d]\n", osa_t_getid());

	// endless loop
    while(1) {
        osa_t_sleep(5);
        printf("[MSG 1] i = %6d [%d]\n", i++, osa_t_getid());
    }

	return 0;
}

int main(int argc, char *argv[])
{
    int ret;

    dbg_stacktrace_setup();
    osa_t_init();

    ret = osa_test_thread7();

    osa_t_release();

    return ret;
}
