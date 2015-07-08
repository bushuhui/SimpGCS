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
#include "rtk_osa++.h"
#include "rtk_utils.h"
#include "rtk_debug.h"

using namespace rtk;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class TestThread : public RThread
{
public:
    virtual int thread_func(void *arg=NULL) {
        ri64            pid, tid;
        std::string     hn;

        osa_get_pid(&pid);
        osa_get_tid(&tid);
        osa_get_hostname(hn);

        printf("[%3d] pid = %d, tid = %d, hostname = %s\n", threadID, pid, tid, hn.c_str());

        while(m_isAlive) {

        }

        return 0;
    }

public:
    int     threadID;
};

int test_osa_utils(CParamArray *pa)
{
    TestThread      t1, t2;
    ri64            pid, tid;
    std::string     hn;


    t1.threadID = 0;
    t1.start();

    t2.threadID = 1;
    t2.start();

    osa_get_pid(&pid);
    osa_get_tid(&tid);
    osa_get_hostname(hn);

    printf("main: pid = %d, tid = %d, hostname = %s\n", pid, tid, hn.c_str());

    t1.setAlive(0);
    t2.setAlive(0);
    t1.wait(100);
    t2.wait(100);

    return 0;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct RTK_TestFunctionArray g_fa[] =
{
    RTK_FUNC_TEST_DEF(test_osa_utils,               "OSA utils"),


    {NULL,  "NULL",  "NULL"},
};

int main(int argc, char *argv[])
{
    CParamArray     pa;

    return rtk_test_main(argc, argv, g_fa, pa);
}
