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
    "Test Portable layer OSA",
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

RTK_TestModule_t	*g_pTM_OSA = &g_oTestModule;

extern RTK_TestModule_t		*g_pTM_osa_Thread;
extern RTK_TestModule_t 	*g_pTM_osa_TSD;
extern RTK_TestModule_t 	*g_pTM_osa_Mutex;
extern RTK_TestModule_t 	*g_pTM_osa_Semaphore;
extern RTK_TestModule_t 	*g_pTM_osa_TM;


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
    RTK_TestModule_t	*pActiveMod;
    OSA_InputEvent_t	evQuit;

	/* set message to active module */
	if( g_oTestModule.pActiveMod != NULL ) {
		/* active sub-module quit */
		if( ev->type == OSA_EV_QUIT ) {
			g_oTestModule.pActiveMod = NULL;
			test_print();
			return 0;
		}
		
        // set event to sub-module
		g_oTestModule.pActiveMod->event(ev);
		return 0;
	}

	/* run select module */
	switch(ev->code) {
	case OSA_VK_1:
        pActiveMod = g_pTM_osa_Thread;
		break;
		
	case OSA_VK_2:
        pActiveMod = g_pTM_osa_TSD;
		break;
		
	case OSA_VK_3:
        pActiveMod = g_pTM_osa_Mutex;
		break;

	case OSA_VK_4:
        pActiveMod = g_pTM_osa_Semaphore;
		break;

	case OSA_VK_8:
        pActiveMod = g_pTM_osa_TM;
		break;
		
	case OSA_VK_COLOR_KEY_0:
    case OSA_VK_ESCAPE:
		evQuit.type = OSA_EV_QUIT;
        if( g_oTestModule.pParentMod != NULL )
            g_oTestModule.pParentMod->event(&evQuit);
        else
            osa_ev_quit();

		return 0;

	default:
		return -1;
	}

	g_oTestModule.pActiveMod = pActiveMod;
	pActiveMod->run(0, NULL, &g_oTestModule);
	
	return 0;
}

static ri32	test_print(void)
{
	test_db("********************************************************\n");
	test_db("* Test PL OSA sub-system\n");
	test_db("********************************************************\n");
	test_db("1. Thread\n");
	test_db("2. TSD\n");
	test_db("3. Mutex\n");
	test_db("4. Semaphore\n");
	test_db("5. Condition variable\n");
	test_db("6. Message queue\n");
	test_db("7. Memory\n");
	test_db("8. Time\n");
	test_db("9. Event\n");
	test_db("CLKEY0. Exit\n");
    test_db("\n");

	return 0;
}

/******************************************************************************
 * Test main function
 *****************************************************************************/
int main(int argc, char *argv[])
{
    // osa init
    osa_init();

    // run top test module
    test_run(argc, argv, NULL);
    osa_ev_registerInputEventHandler(test_event);

    // loop forever
    osa_ev_loop();

    // osa release
    osa_release();

    return 0;
}
