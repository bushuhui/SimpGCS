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

int osa_test_thread7(void);
static OSA_RESULT _thread7_proc(OSA_T_ARGS arg);

OSA_HANDLE hThread7;

int osa_test_thread7(void)
{
	OSA_RESULT res;
	
	test_db("[Function] osa_test_thread7()\n");
	test_db("[Description] Test thread create\n");
	test_db("[Description] If you cann't 'Success' message this test failed!\n");

	res = osa_t_create(&hThread7, _thread7_proc, 0, 16*1024, 0);
	if( 0 != res ) {
		test_db("[Error] Failed to create thread\n");
		test_db("[Result] Failed\n");
	}

	return 0;
}

OSA_RESULT _thread7_proc(OSA_T_ARGS arg)
{
	test_db("[MSG 1] Thread7 running\n");
	test_db("[Result] Success\n");
    test_db("\n");

	return 0;
}

