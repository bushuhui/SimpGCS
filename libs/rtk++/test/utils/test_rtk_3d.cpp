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

// test OpenCV functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "rtk_debug.h"
#include "rtk_utils.h"
#include "rtk_3d.h"

using namespace std;
using namespace rtk;


/**
 * @brief Test Geodesic distance matrix
 * @param pa - parameter array
 * @return 0 - success
 */
int test_geodesic_distance(CParamArray *pa)
{
    string          fname;
    char            fn_buf[200];

    MeshData        m;

    float           *gd, *agd, v;
    double          ma;

    int             i, j;


    // get parameters
    fname = "b80.off";
    pa->s("fname", fname);

    // load mesh
    m.load(fname.c_str());

    gd  = new float[m.vex_num*m.vex_num];
    agd = new float[m.vex_num];

    m.calc_geodesic_matrix(gd);
    ma = m.mesh_area();

    for(i=0; i<m.vex_num; i++) {
        v = 0.0;
        for(j=0; j<m.vex_num; j++) v += gd[i*m.vex_num+j];
        agd[i] = v/ma;
    }

    // output data
    {
        sprintf(fn_buf, "%s_agd", fname.c_str());
        FILE *fp = fopen(fn_buf, "w"); ASSERT(FP);

        for(i=0; i<m.vex_num; i++) fprintf(fp, "%g\n", agd[i]);

        fclose(fp);
    }

    delete [] gd;
    delete [] agd;

    return 0;
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct RTK_TestFunctionArray g_fa[] =
{
    RTK_FUNC_TEST_DEF(test_geodesic_distance,       "List directory"),


    {NULL,  "NULL",  "NULL"},
};


int main(int argc, char *argv[])
{
    CParamArray     pa;

    return rtk_test_main(argc, argv, g_fa, pa);
}
