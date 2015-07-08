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

#ifndef __RTK_CV_DESCRIPTOR_H__
#define __RTK_CV_DESCRIPTOR_H__

#include <stdio.h>
#include <vector>
#include <opencv2/core/core.hpp>

#include <rtk_paramarray.h>

namespace rtk {

////////////////////////////////////////////////////////////////////////////////
/// descriptor
////////////////////////////////////////////////////////////////////////////////
int cv_census_transform(cv::Mat &img_in, cv::Mat &img_ct);
int cv_centrist(cv::Mat &img_in, cv::Mat &img_lab, double **hist);

int cv_pact_init(std::string &fn_pca);
int cv_pact_scaleratio(double *scaleratio);
int cv_pact(cv::Mat &img_in, double **hist);

int cv_pact_sp_seg(cv::Mat &img, cv::Mat &img_ct, cv::Mat &img_lab,
                   double *phist,
                   int desc_dim=42,
                   FILE *pca_fp=NULL);

int cv_ct_sp_seg(cv::Mat &img, cv::Mat &img_ct, cv::Mat &img_lab,
                   double *phist,
                   int desc_dim=256);

int cv_pact_sp(cv::Mat &img_in, double **hist, CParamArray *pa=NULL,
               FILE *pca_fp=NULL);


////////////////////////////////////////////////////////////////////////////////
/// segmentation
////////////////////////////////////////////////////////////////////////////////
int cv_seg2bmap(cv::Mat &img_lab, cv::Mat &img_bmap);
int cv_superpixel_ers(cv::Mat &img, cv::Mat &img_lab,
                   int nC,
                   double lambda=0.5, double sigma=5.0,
                   int kernel = 0, int conn8 = 1);


////////////////////////////////////////////////////////////////////////////////
/// lines
////////////////////////////////////////////////////////////////////////////////
void kht_lines(cv::Mat &img_bin,
               std::vector<cv::Vec2f> &lines,
               const size_t cluster_min_size = 10,
               const double cluster_min_deviation = 2.0,
               const double delta = 0.5,
               const double kernel_min_height = 0.002,
               const double n_sigmas = 2.0);


typedef cv::Vec<float, 7> VecLSDLine;

/** LSD Full Interface

    @param img_in      Input gray-scale image
    @param lines       Output Lines

                       A float vector array of size 7 x n_out, containing the list
                       of line segments detected. The array contains first
                       7 values of line segment number 1, then the 7 values
                       of line segment number 2, and so on, and it finish
                       by the 7 values of line segment number n_out.
                       The seven values are:
                       - x1,y1,x2,y2,width,p,-log10(NFA)
                       .
                       for a line segment from coordinates (x1,y1) to (x2,y2),
                       a width 'width', an angle precision of p in (0,1) given
                       by angle_tolerance/180 degree, and NFA value 'NFA'.
                       If 'out' is the returned pointer, the 7 values of
                       line segment number 'n+1' are obtained with
                       'out[7*n+0]' to 'out[7*n+6]'.



    @param scale       When different from 1.0, LSD will scale the input image
                       by 'scale' factor by Gaussian filtering, before detecting
                       line segments.
                       Example: if scale=0.8, the input image will be subsampled
                       to 80% of its size, before the line segment detector
                       is applied.
                       Suggested value: 0.8

    @param sigma_scale When scale!=1.0, the sigma of the Gaussian filter is:
                       sigma = sigma_scale / scale,   if scale <  1.0
                       sigma = sigma_scale,           if scale >= 1.0
                       Suggested value: 0.6

    @param quant       Bound to the quantization error on the gradient norm.
                       Example: if gray levels are quantized to integer steps,
                       the gradient (computed by finite differences) error
                       due to quantization will be bounded by 2.0, as the
                       worst case is when the error are 1 and -1, that
                       gives an error of 2.0.
                       Suggested value: 2.0

    @param ang_th      Gradient angle tolerance in the region growing
                       algorithm, in degrees.
                       Suggested value: 22.5

    @param log_eps     Detection threshold, accept if -log10(NFA) > log_eps.
                       The larger the value, the more strict the detector is,
                       and will result in less detections.
                       (Note that the 'minus sign' makes that this
                       behavior is opposite to the one of NFA.)
                       The value -log10(NFA) is equivalent but more
                       intuitive than NFA:
                       - -1.0 gives an average of 10 false detections on noise
                       -  0.0 gives an average of 1 false detections on noise
                       -  1.0 gives an average of 0.1 false detections on nose
                       -  2.0 gives an average of 0.01 false detections on noise
                       .
                       Suggested value: 0.0

    @param density_th  Minimal proportion of 'supporting' points in a rectangle.
                       Suggested value: 0.7

    @param n_bins      Number of bins used in the pseudo-ordering of gradient
                       modulus.
                       Suggested value: 1024

 */
void lsd_lines(cv::Mat &img_in,
               std::vector<VecLSDLine> &lines,
               double scale = 0.8,
               double sigma_scale = 0.6,
               double quad = 2.0,
               double ang_th = 22.5,
               double log_eps = 0.0,
               double density_th = 0.7,
               int n_bins = 1024);


////////////////////////////////////////////////////////////////////////////////
/// utils
////////////////////////////////////////////////////////////////////////////////
int cv_print_Mat(cv::Mat &m);

int cv_draw_hist(cv::Mat &img_hist, double *hist, int h_bin, int bw, int bh);
int cv_draw_hist_auto(cv::Mat &img_hist, double *val, int val_n, int h_bin, int bw, int bh);


} // end of namespace rtk

#endif // end of __RTK_CV_DESCRIPTOR_H__

