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
#include <stdlib.h>
#include <string.h>

#include "rtk_osa.h"
#include "rtk_test_module.h"

using namespace rtk;

/******************************************************************************
 *	Local Function
 *****************************************************************************/
static ri32	test_init(void *arg);
static ri32	test_release(void);

static ri32 	test_run(ri32 argc, char *argv[], RTK_TestModule_t *parent_mod);
static ri32 	test_stop(void);
static ri32 	test_event(OSA_InputEvent_t *ev);

static ri32	test_print(void);

/******************************************************************************
 *	GFX test module define
 *****************************************************************************/
static RTK_TestModule_t	g_oTestModule =
{
    "OSA Thread",
	0,
	NULL,
	NULL,
	NULL,

	test_init,
	test_release,

	test_run,
	test_stop,
	test_event
};
RTK_TestModule_t	*g_pTM_osa_Thread = &g_oTestModule;

extern int osa_test_thread1(void);
extern int osa_test_thread2(void);
extern int osa_test_thread3(void);
extern int osa_test_thread4(void);
extern int osa_test_thread5(void);
extern int osa_test_thread6(void);
extern int osa_test_thread7(void);
extern int osa_test_thread8(void);


static ri32	test_init(void *arg)
{
	return 0;
}

static ri32	test_release(void)
{
	return 0;
}

static ri32 	test_run(ri32 argc, char *argv[], RTK_TestModule_t *parent_mod)
{
	test_print();
	g_oTestModule.pParentMod = parent_mod;
	g_oTestModule.pActiveMod = NULL;

	return 0;
}

static ri32 	test_stop(void)
{
	return 0;
}

static ri32 	test_event(OSA_InputEvent_t *ev)
{
	OSA_InputEvent_t		evQuit;

	/* run select module */
	switch(ev->code) {
	case OSA_VK_1:
		osa_test_thread1();
		break;
		
	case OSA_VK_2:
		osa_test_thread2();
		break;
	
	case OSA_VK_3:
		osa_test_thread3();
		break;
	
	case OSA_VK_4:
		osa_test_thread4();
		break;
	
	case OSA_VK_5:
		osa_test_thread5();
		break;
	
	case OSA_VK_6:
		osa_test_thread6();
		break;
	
	case OSA_VK_7:
		osa_test_thread7();
		break;
	
	case OSA_VK_8:
		osa_test_thread8();
		break;
		
	case OSA_VK_COLOR_KEY_0:
    case OSA_VK_ESCAPE:
		evQuit.type = OSA_EV_QUIT;
		g_oTestModule.pParentMod->event(&evQuit);
		return 0;

	default:
		return -1;
	}

	return 0;
}

static ri32	test_print(void)
{
	test_db("********************************************************\n");
    test_db("	Test OSA Thread\n");
	test_db("********************************************************\n");
	test_db("1. osa_test_thread1(), thread crating\n");
	test_db("2. osa_test_thread2(), thread sleep\n");
	test_db("3. osa_test_thread3(), argument passing\n");
	test_db("4. osa_test_thread4(), suspend/resume\n");
	test_db("5. osa_test_thread5(), self suspend\n");
	test_db("6. osa_test_thread6(), memory leak check\n");
	test_db("7. osa_test_thread7(), kill\n");
	test_db("8. osa_test_thread8(), delete\n");
	test_db("CLKEY0: Exit\n");
    test_db("\n");

	return 0;
}

