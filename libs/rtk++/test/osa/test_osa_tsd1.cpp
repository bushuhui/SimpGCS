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

extern int osa_test_tsd(void);

int osa_test_tsd1(void);
static OSA_RESULT _thread1_proc(OSA_T_ARGS arg);


int osa_test_tsd1(void)
{
	OSA_RESULT res;
	OSA_HANDLE hThread1;
	
	test_db("[Function] osa_test_tsd1()\n");
	test_db("[Description] 32bit TSD data getting/setting test\n");
	test_db("[Description] If you cann't 'Success' message this test failed!\n");

	res = osa_t_create(&hThread1, _thread1_proc, 0, 16*1024, 0);
	if( 0 != res ) {
		test_db("[Error] Failed to create thread\n");
		test_db("[Result] Failed\n");
	}

	res = osa_t_sleep(2000);

	osa_t_kill(hThread1);
	osa_t_delete(hThread1);
	
	return 0;
}

OSA_RESULT _thread1_proc(OSA_T_ARGS arg)
{
	OSA_RESULT res;
	ru32 start = 0x00000000;
	ru32 end = 0x00000000;
	ru32 i, val;
	
	for(i=start; i<end; i+= 12345678) {
		res = osa_tsd_set((OSA_DATA) i);
		if( res != 0 ) {
			test_db("[ERROR] Fail at osa_tsd_set, res=%d\n", res);
			test_db("[RESULT] Fail\n");
			return 0;
		}

		res = osa_tsd_get((OSA_DATA *) &val);
		if( res != 0 ) {
			test_db("[ERROR] Fail at osa_tsd_get, res=%d\n", res);
			test_db("[RESULT] Fail\n");
			return 0;
		}

        if(val != i) {
			test_db("[ERROR] set tsd and returned tsd are different\n");
			test_db("[MSG 1] Thread return tsd=0x%08x, expected value=0x%08x\n", val, i);
			test_db("[RESULT] Fail\n");

            return 0;
        }
	}

	test_db("[RESULT] Success\n");
    test_db("\n");

	return 0;
}


