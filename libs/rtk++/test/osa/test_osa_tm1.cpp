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

static void _timer1_proc(void *arg)
{
	static ru32	iCount = 0;
	
	test_db("[MSG 1] %d Timer expired!\n", iCount++);
	return;
}

int osa_test_tm1(void)
{
	OSA_HANDLE	hTimer;
	OSA_RESULT	res;
	
	
	test_db("[Function] osa_test_tm1()\n");
	
	res = osa_tm_create(&hTimer, 100, _timer1_proc, NULL, OSA_TM_PERIODIC);
	if( res ) {
		test_db("[ERR] Failed to create timer!\n");
		goto _osa_TEST_TM1_ERR0;
		return -1;
	}
	
	osa_t_sleep(2000);
	
	res = osa_tm_delete(hTimer);
	if( res ) {
		test_db("[ERR] Failed at osa_tm_delete!\n");
		goto _osa_TEST_TM1_ERR0;
	}

    test_db("\n");
	
	return 0;

_osa_TEST_TM1_ERR0:
	test_db("[MSG] Failed!\n");
	return -1;
}
