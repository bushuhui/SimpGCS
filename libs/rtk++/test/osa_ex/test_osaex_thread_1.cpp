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

int osa_test_thread1(void);
static OSA_RESULT _thread1_proc(OSA_T_ARGS arg);

OSA_HANDLE hThread1;

int osa_test_thread1(void)
{
	OSA_RESULT res;
	
    printf("[Function] osa_test_thread1()\n");
    printf("[Description] Test thread create\n");
    printf("[Description] If you cann't 'Success' message this test failed!\n");

	res = osa_t_create(&hThread1, _thread1_proc, 0, 16*1024, 0);
	if( 0 != res ) {
        printf("[Error] Failed to create thread\n");
        printf("[Result] Failed\n");
	}

    osa_t_wait(hThread1);
    osa_t_delete(hThread1);

	return 0;
}

OSA_RESULT _thread1_proc(OSA_T_ARGS arg)
{
    printf("[MSG 1] Thread1 running\n");
    printf("[Result] Success\n");
    printf("\n");

	return 0;
}

int main(int argc, char *argv[])
{
    dbg_stacktrace_setup();
    return osa_test_thread1();
}

