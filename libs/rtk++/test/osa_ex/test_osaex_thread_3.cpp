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

int osa_test_thread3(void);
static OSA_RESULT _thread3_proc(OSA_T_ARGS arg);
static size_t     _thread_arg = 0x3423454d;

OSA_HANDLE hThread3;

int osa_test_thread3(void)
{
	OSA_RESULT res;

    printf("[Function] osa_test_thread3()\n");
    printf("[Description] Test thread agrument passing\n");
    printf("[Description] If you cann't 'Success' message this test failed!\n");

    res = osa_t_create(&hThread3, _thread3_proc, &_thread_arg);
	if( 0 != res ) {
        printf("[ERROR] Failed to create thread\n");
        printf("[RESULT] Failed\n");
	}

    osa_t_wait(hThread3);
    osa_t_delete(hThread3);

	return 0;
}

OSA_RESULT _thread3_proc(OSA_T_ARGS arg)
{
    size_t  *p_arg = (size_t*) arg;

    printf("[MSG 1] Thread3 running\n");

    if( *p_arg != _thread_arg ) {
        printf("[ERROR] input arg error!\n");
        printf("[RESULT] Failed\n");
		return 0;
	}

    printf("[RESULT] Success\n");
    printf("\n");

	return 0;
}

int main(int argc, char *argv[])
{
    dbg_stacktrace_setup();
    return osa_test_thread3();
}
