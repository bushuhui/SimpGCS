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

#include "rtk_debug.h"
#include "rtk_utils.h"

using namespace std;
using namespace rtk;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Test1 : public RTK_TestModule
{
public:
    virtual int run(CParamArray *pa) {
        fmt::print("Hello first test module!\n");

        return 0;
    }

    virtual int help(CParamArray *pa) {
        fmt::print("Test RTK_TestModule and show usage!\n");

        return 0;
    }
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct RTK_TestModuleArray g_ma[] =
{
    RTK_TESTMODULE_DEF(Test1,           "Test module class"),

    {NULL, "NULL", "NULL"}
};

int main(int argc, char *argv[])
{
    return rtk_test_main(argc, argv, g_ma);
}
