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

int osa_test_semaphore1(void);
static OSA_RESULT _thread1_proc(OSA_T_ARGS arg);

OSA_HANDLE _test_semaphore1;
ru32 _test_sm1_ready;

int osa_test_semaphore1(void)
{
	OSA_RESULT res;
	OSA_HANDLE hThread;
	
	test_db("[Function] osa_test_semaphore1()\n");
	test_db("[Description] To test the basic functinality of mutex\n");
	test_db("[Description] If you cann't 'Success' message this test failed!\n");

    _test_sm1_ready = false;

    /* Create semaphore with count 0 */
	res = osa_sem_create(&_test_semaphore1, 0, 0);
	if( 0 != res ) {
		test_db("[ERROR] Fail at osa_sem_create, res=%d\n", res);
		test_db("[RESULT] Fail\n");

		return 0;
	}

    /* Create a thread */
	res = osa_t_create(&hThread, _thread1_proc, 0, 16 * 1024, 0);
	if( 0 != res ) {
		test_db("[ERROR] Fail at osa_t_create, res=%d\n", res);
		test_db("[RESULT] Fail\n");
		
		return 0;
	}

    /* Wait for locking semaphore */
	res = osa_sem_wait(_test_semaphore1);
	if( 0 != res ) {
		test_db("[ERROR] Fail at osa_sem_wait, res=%d\n", res);
		test_db("[RESULT] Fail\n");

		return 0;
	}

    if (_test_sm1_ready == false) {
		test_db("[ERROR] The osa_sem_wait didn['t wait until sm is available\n");
		test_db("[RESULT] Fail\n");

		return 0;
    }

    /* Try to lock semaphore without waiting: Expect OSA_WAIT_NO */
	res = osa_sem_wait_timeout(_test_semaphore1, OSA_WAIT_NO);
	if( res != E_OSA_SEM_TIMEOUT ) {
		test_db("[ERROR] Fail at osa_sem_wait_timeout, res=%d\n", res);
		test_db("[MSG 1] E_OSA_SEM_TIMEOUT(%d) must be returned\n", E_OSA_SEM_TIMEOUT);
		test_db("[RESULT] Fail\n");

		return 0;
	}

    /* Try to lock semaphore with timeout: Expect E_OSA_SEM_TIMEOUT */
	res = osa_sem_wait_timeout(_test_semaphore1, 100);
	if( res != E_OSA_SEM_TIMEOUT ) {
		test_db("[ERROR] Fail at osa_sem_wait, res=%d\n", res);
		test_db("[MSG 1] E_OSA_SEM_TIMEOUT(%d) must be returned\n", E_OSA_SEM_TIMEOUT);
		test_db("[RESULT] Fail\n");

		return 0;
	}

    /* Delete semaphore */
	res = osa_sem_delete(_test_semaphore1);
	if( res != 0 ) {
		test_db("[ERROR] Fail at osa_sem_delete, res=%d\n", res);
		test_db("[RESULT] Fail\n");

		return 0;
	}

	osa_t_kill(hThread);
	osa_t_delete(hThread);

	test_db("[RESULT] Success\n");
    test_db("\n");

	return 0;
}

OSA_RESULT _thread1_proc(OSA_T_ARGS arg)
{
	OSA_RESULT res;

	res = osa_t_sleep(2000);
	if( res != 0 ) {
		test_db("[ERROR] Fail at osa_t_sleep, res=%d\n", res);
		test_db("[RESULT] Fail\n");

		return 0;
	}

    _test_sm1_ready = true;

	res = osa_sem_signal(_test_semaphore1);
	if( res != 0 ) {
		test_db("[ERROR] Fail at osa_sem_signal, res=%d\n", res);
		test_db("[RESULT] Fail\n");

		return 0;
	}

	return 0;
}
