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

#ifndef __RTK_IMAGE_H__
#define __RTK_IMAGE_H__

#include <SDL/SDL.h>
#include <QImage>
#include <opencv2/core/core.hpp>

#include "rtk_types.h"

namespace rtk {

class RTK_Image {
public:
    RTK_Image();
    RTK_Image(int w, int h);
    ~RTK_Image();

    // set/get image properties
    int width(void);
    int height(void);
    int set_size(int w, int h);
    int get_size(int *w, int *h);

    // get raw pixel
    ru8* get_bits(void);

    // alloc/free
    int create(int w, int h);
    int alloc(int w, int h);
    int free(void);

    int clear(void);
    int clear(RTK_Color &c);

    // set drawing properties
    int set_pen_width(double pw);
    int set_pen_color(RTK_Color &pc);
    int set_font(const char *fname);
    int set_font_size(double s);
    int set_font_color(RTK_Color &fc);

    // transformations
    int translate(double tx, double ty);
    int scale(double sx, double sy);
    int rotate(double ang);

    // drawing functions
    int move_to(double x, double y);
    int line_to(double x, double y);
    int draw_line(double x1, double y1, double x2, double y2);
    int draw_rect(double x1, double y1, double x2, double y2);
    int draw_circle(double x, double y, double r);
    int draw_ellipse(double x, double y, double w, double h, double ang);

    int close_path(void);
    int stroke(void);
    int fill(void);
    int fill_preserve(void);

    int draw_text(double x, double y, const char *s, int xalign=0, int yalign=0, double ang=0.0);

    // load/save image file
    int save(const char *fname);
    int load(const char *fname);

    // SDL supports
    int to_sdl(SDL_Surface **sdl_surf);
    int from_sdl(SDL_Surface *sdl_surf);
    SDL_Surface* to_sdl(void);

    // Qt QImage supports
    int to_QImage(QImage **qimage);
    int from_QImage(QImage *qimage);
    QImage* to_QImage(void);

    // OpenCV Mat supports
    int to_mat(cv::Mat **cv_img);
    int from_mat(cv::Mat *cv_img);

protected:
    int     m_iw, m_ih;
    int     m_jpeg_qual;
    void    *m_pri_data;

    // load/save functions
    int save_png(const char *fname);
    int load_png(const char *fname);
    int save_bmp(const char *fname);
    int load_bmp(const char *fname);
    int save_jpg(const char *fname);
    int load_jpg(const char *fname);
};

}

#endif // end of __RTK_IMAGE_H__
