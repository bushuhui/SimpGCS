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

#ifndef __RTK_PR_H__
#define __RTK_PR_H__

#include <vector>

namespace rtk {

////////////////////////////////////////////////////////////////////////////////
/// Histogram match
////////////////////////////////////////////////////////////////////////////////

double emd   (int n, double *h1, double *h2);
double emd   (double *v1, int v1_n, double *v2, int v2_n);
double emd_cv(double *v1, int v1_n, double *v2, int v2_n);

double hik_dis(int n, double *h1, double *h2);

////////////////////////////////////////////////////////////////////////////////
/// k-means
////////////////////////////////////////////////////////////////////////////////
int kmeans_yakmo(int fea_n, std::vector<double> &dat,
                 int k,
                 std::vector<double> &cents,
                 int iter_n = 100);

////////////////////////////////////////////////////////////////////////////////
/// spectral embedding
////////////////////////////////////////////////////////////////////////////////
int calc_spectral_embedding(int n, float *c,
                            int fea_n, float *fea);
int calc_spectral_embedding2(int n, float *c,
                            int fea_n, float *fea);

////////////////////////////////////////////////////////////////////////////////
/// sparse coding
////////////////////////////////////////////////////////////////////////////////

int calc_bow_fea(int dic_n, int fea_m, double *dic, double *x,
                 double *bow_fea,
                 double bow_d_sig,
                 int *bow_i=NULL);

// Locality-constrained Linear Coding
int calc_llc_sc(int dic_n, int fea_m, double *dic, double *x, double *sc,
                int knn=5, double lambda=1e-4,
                int *dic_i=NULL);

} // end of namespcae rtk

#endif // end of __RTK_PR_H__
