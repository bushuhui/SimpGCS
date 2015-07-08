
#include <stdio.h>
 
#include "rtk_osa.h"
#include "rtk_debug.h"

using namespace rtk;


static ri32 test_event(OSA_InputEvent_t *ev)
{
	/* run select module */
	switch(ev->code) {
	case OSA_VK_1:
	    //printf("1 pressed\n");
		break;

	default:
		return -1;
	}

	return 0;
}

int osa_test_event(void)
{
    // initialize event sub-system
    osa_ev_init();
    
    // regist event receiver
    osa_ev_registerInputEventHandler(test_event);
    
    // loop for ever
    osa_ev_loop();
    
    // release event settings
    osa_ev_release();
    
    return 0;
}


int main(int argc, char *argv[])
{
    dbg_stacktrace_setup();
    osa_test_event();

    return 0;
}

