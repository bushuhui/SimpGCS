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

#ifndef __RTK_TEST_MODULE_H__
#define __RTK_TEST_MODULE_H__

#include "rtk_types.h"

namespace rtk {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define TESTMODULE_MAX_NAMELENG		30

typedef struct RTK_TestModule {
	char		szName[TESTMODULE_MAX_NAMELENG];
	ru32		uiType;
	void		*pData;
	
	struct RTK_TestModule	*pParentMod;
	struct RTK_TestModule	*pActiveMod;

	ri32		(*init)(void *arg);
	ri32		(*deinit)(void);

    ri32		(*run)(ri32 argc, char *argv[], struct RTK_TestModule *parent_mod);
	ri32		(*stop)(void);
    ri32		(*event)(OSA_InputEvent_t *ev);
} RTK_TestModule_t;


// define print function
#define test_db		printf


} // end namespace rtk

#endif /* end of __RTK_TEST_MODULE_H__ */

