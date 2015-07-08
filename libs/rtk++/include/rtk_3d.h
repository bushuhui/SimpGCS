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


#ifndef __RTK_3D__
#define __RTK_3D__

#include "rtk_types.h"

namespace rtk {

class MeshData {
public:
    int     vex_num, ele_num, edge_num;

    double  *vex_arr;
    int     *ele_arr;

public:
    MeshData();
    ~MeshData();

    void init(void);
    void free(void);

    int load(const char *fname);
    int save(const char *fname);
    //int load_haveedge(HalfEdge *);

    int get_ele_by_vex(int vex_id, int *ele_n, int *ele);
    int calc_vex_norm(int vex_id, double *v);

    int get_vex_num(void) { return vex_num; }
    int get_ele_num(void) { return ele_num; }
    int get_vex(int vex_id, double *v);

    double *get_vex(void) { return vex_arr; }
    int    *get_ele(void) { return ele_arr; }

    double mesh_area(void);

    // calculate geodesic distance matrix
    int calc_geodesic_matrix(float *da);
};

} // end of namespace rtk

#endif // end of __RTK_3D__
