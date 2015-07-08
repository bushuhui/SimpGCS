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

#include "rtk_types.h"
#include "rtk_debug.h"

using namespace rtk;


// test class
class foo_class
{
public:
    foo_class() { 
        dbg_pt("1");
        foo1(); 
        dbg_pt("2");
    }

private:
    void foo1() { 
        dbg_pt("1");
        foo2(); 
        dbg_pt("2");
    }

    void foo2() { 
        dbg_pt("1");
        foo3(); 
        dbg_pt("2");
    }

    void foo3() { 
        dbg_pt("1");
        foo4(); 
        dbg_pt("2");
    }

    void foo4() { 
        dbg_pt("1");
        crash(); 
        dbg_pt("2");
    }

    void crash() { 
        dbg_pt("1");

        char * p = NULL; 

        dbg_pt("2");

        *p = 0; 

        dbg_pt("3");
    }
};


int main(int argc, char *argv[])
{
    // setup debug utils
    dbg_stacktrace_setup();
    dbg_set_level(5);

    // test dbg_ print functions
    dbg_pe("Error message sample");
    dbg_pw("Warnning message sample");
    dbg_pi("Program information");
    dbg_pt("run here");
    dbg_pn("normal message, actually not frequently used");

    // create a class
    foo_class *f;

    dbg_pt("before create object");
    f = new foo_class();
    dbg_pt("finished create object");
    
    return 0;
}

