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
#include <math.h>

#include "geodesic_algorithm_dijkstra.h"
#include "geodesic_algorithm_subdivision.h"
#include "geodesic_algorithm_exact.h"

#include "rtk_3d.h"
#include "rtk_math.h"
#include "rtk_debug.h"

namespace rtk {

MeshData::MeshData()
{
    init();
}

MeshData::~MeshData()
{
    free();
}

void MeshData::init(void)
{
    vex_num = 0;
    ele_num = 0;
    edge_num = 0;

    vex_arr = NULL;
    ele_arr = NULL;
}

void MeshData::free(void)
{
    if( vex_arr != NULL ) {
        delete [] vex_arr;
        vex_arr = NULL;
    }
    if( ele_arr != NULL ) {
        delete [] ele_arr;
        ele_arr = NULL;
    }

    vex_num = 0;
    ele_num = 0;
    edge_num = 0;
}

int MeshData::load(const char *fname)
{
    FILE    *fp;
    char    buf[2000];
    int     i, j;
    int     i1, i2, i3, i4;
    float   f1, f2, f3, f4;

    fp = fopen(fname, "r");
    ASSERT(fp);

    // read file header
    j = fscanf(fp, "%s", buf);

    // read vertex, element, edege number
    j = fscanf(fp, "%d %d %d", &i1, &i2, &i3);

    this->free();
    vex_num = i1;
    ele_num = i2;
    vex_arr = new double[vex_num*3];
    ele_arr = new int[ele_num*3];

    // read vertex array
    for(i=0; i<vex_num; i++) {
        // FIXME: only support 3-D vertex (exclude color)
        j = fscanf(fp, "%f %f %f", &f1, &f2, &f3);

        vex_arr[i*3+0] = f1;
        vex_arr[i*3+1] = f2;
        vex_arr[i*3+2] = f3;
    }

    // read element array
    for(i=0; i<ele_num; i++) {
        // FIXME: only support triangle element (exclude color)
        j = fscanf(fp, "%d %d %d %d", &i1, &i2, &i3, &i4);

        ele_arr[i*3+0] = i2;
        ele_arr[i*3+1] = i3;
        ele_arr[i*3+2] = i4;
    }

    fclose(fp);

    return 0;
}

int MeshData::save(const char *fname)
{
    FILE    *fp;
    int     i;

    fp = fopen(fname, "w");
    ASSERT(fp);

    // write file header
    fprintf(fp, "OFF\n");

    // write vertex, element, edege number
    fprintf(fp, "%d %d %d\n",
            vex_num, ele_num, 0);

    // write vertex array
    for(i=0; i<vex_num; i++)
        fprintf(fp, "%f %f %f\n",
                vex_arr[i*3+0], vex_arr[i*3+1], vex_arr[i*3+2]);

    // write element array
    for(i=0; i<ele_num; i++)
        fprintf(fp, "%d %d %d %d\n",
                3,
                ele_arr[i*3+0], ele_arr[i*3+1], ele_arr[i*3+2]);

    fclose(fp);

    return 0;
}

/**
 *  Get elements which contain the vertex
 */
int MeshData::get_ele_by_vex(int vex_id, int *ele_n, int *ele)
{
    int     i, n;

    n = 0;
    for(i=0; i<ele_num*3; i++) {
        if( ele_arr[i] == vex_id ) {
            ele[n++] = i/3;
        }
    }

    *ele_n = n;

    return 0;
}

/**
 *  Get vertex data
 */
int MeshData::get_vex(int vex_id, double *v)
{
    v[0] = vex_arr[vex_id*3+0];
    v[1] = vex_arr[vex_id*3+1];
    v[2] = vex_arr[vex_id*3+2];

    return 0;
}

/**
 *  Calculate vertex normal vector
 */
int MeshData::calc_vex_norm(int vex_id, double *v)
{
    int     ele_n, *ele;
    double  v1[3], v2[3], v3[3];
    int     i1, i2, i3, ei;
    int     i, j;
    double  l;

    // clear v
    for(i=0; i<3; i++) v[i] = 0.0;

    // get elements which contain given vertex
    ele_n = 40;
    ele = new int[ele_n];
    get_ele_by_vex(vex_id, &ele_n, ele);

    // for each element
    for(i=0; i<ele_n; i++) {
        ei = ele[i];
        i1 = ele_arr[ei*3+0];
        i2 = ele_arr[ei*3+1];
        i3 = ele_arr[ei*3+2];

        // v2 - v1
        v1[0] = vex_arr[i2*3+0] - vex_arr[i1*3+0];
        v1[1] = vex_arr[i2*3+1] - vex_arr[i1*3+1];
        v1[2] = vex_arr[i2*3+2] - vex_arr[i1*3+2];

        // v3 - v1
        v2[0] = vex_arr[i3*3+0] - vex_arr[i1*3+0];
        v2[1] = vex_arr[i3*3+1] - vex_arr[i1*3+1];
        v2[2] = vex_arr[i3*3+2] - vex_arr[i1*3+2];

        // (v2-v1)x(v3-v1)
        vec3_cross_product(v1, v2, v3);
        v[0] += v3[0];
        v[1] += v3[1];
        v[2] += v3[2];
    }

    // average normal vector
    v[0] /= ele_n;
    v[1] /= ele_n;
    v[2] /= ele_n;

    // normalize length
    l = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    v[0] /= l;
    v[1] /= l;
    v[2] /= l;

    delete [] ele;

    return 0;
}

double MeshData::mesh_area(void)
{
    int         i;
    int         i1, i2, i3;
    double      s;
    double      *p1, *p2, *p3;

    s = 0;
    for(i=0; i<ele_num; i++) {
        i1 = ele_arr[i*3+0];
        i2 = ele_arr[i*3+1];
        i3 = ele_arr[i*3+2];

        p1 = vex_arr+i1*3;
        p2 = vex_arr+i2*3;
        p3 = vex_arr+i3*3;

        s += triangle3_area(p1, p2, p3);
    }

    return s;
}

int MeshData::calc_geodesic_matrix(float *da)
{
    std::vector<double>     points;
    std::vector<unsigned>   faces;

    int                     i, j;
    float                   dis;

    // check mesh
    if( ele_num < 1 || vex_num < 1 ) {
        dbg_pe("The mesh is empty!\n");
        return -1;
    }

    // convert data type
    {
        points.resize(vex_num*3);
        faces.resize(ele_num*3);

        double  *p1 = &(points[0]);
        for(i=0; i<vex_num*3; i++) p1[i] = vex_arr[i];
        unsigned *p2 = &(faces[0]);
        for(i=0; i<ele_num*3; i++) p2[i] = ele_arr[i];
    }

    // initialize mesh obj
    geodesic::Mesh mesh;
    mesh.initialize_mesh_data(points, faces);

    // create Dijkstra algorithm obj
    geodesic::GeodesicAlgorithmDijkstra dij(&mesh);

    for(i=0; i<vex_num; i++) da[i*vex_num + i] = 0.0;

    // calculate geodesic distance matrix
    for(i=0; i<vex_num-1; i++) {
        std::vector<geodesic::SurfacePoint> s;
        s.push_back(geodesic::SurfacePoint(&mesh.vertices()[i]));

        // cover the whole mesh
        dij.propagate(s);

        // calc geodesic distance
        std::vector<geodesic::SurfacePoint> path;
        for(j=i+1; j<vex_num; j++) {
            geodesic::SurfacePoint pt(&mesh.vertices()[j]);
            dij.trace_back(pt, path);

            dis = geodesic::length(path);
            da[i*vex_num + j] = dis;
            da[j*vex_num + i] = dis;
        }
    }

    return 0;
}


} // end of namesapce rtk
