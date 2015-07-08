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

int osa_test_thread3(void);
static OSA_RESULT _thread3_proc(OSA_T_ARGS arg);

OSA_HANDLE  hThread3;
size_t      _thread_arg = 0x3423454d;

int osa_test_thread3(void)
{
    OSA_RESULT  res;

	test_db("[Function] osa_test_thread3()\n");
	test_db("[Description] Test thread agrument passing\n");
	test_db("[Description] If you cann't 'Success' message this test failed!\n");

    res = osa_t_create(&hThread3, _thread3_proc, &_thread_arg, 16*1024, 0);
	if( 0 != res ) {
		test_db("[ERROR] Failed to create thread\n");
		test_db("[RESULT] Failed\n");
	}

	return 0;
}

OSA_RESULT _thread3_proc(OSA_T_ARGS arg)
{
    size_t      *arg_ = (size_t*) arg;

	test_db("[MSG 1] Thread3 running\n");

    if( *arg_ != _thread_arg ) {
		test_db("[ERROR] input arg error!\n");
		test_db("[RESULT] Failed\n");
		return 0;
	}

	test_db("[RESULT] Success\n");
    test_db("\n");

	return 0;
}

