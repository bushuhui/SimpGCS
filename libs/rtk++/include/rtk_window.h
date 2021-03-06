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

#ifndef __RTK_WINDOW_H__
#define __RTK_WINDOW_H__

#include "rtk_image.h"

namespace rtk {

class RTK_Window
{
public:
    RTK_Window();
    RTK_Window(int w, int h);

    // window functions
    int width(void);
    int height(void);
    int set_size(int w, int h);
    int set_caption(const char *cap);

    // show/hide
    int show(void);
    int hide(void);

    // image functions
    int imshow(RTK_Image *img);
    int imsave(const char *fname);

    RTK_Image*  imget(void);

private:
    void        *m_pri_data;
    RTK_Image   *m_img;
};

} // end of namespace rtk

#endif // end of __RTK_WINDOW_H__
