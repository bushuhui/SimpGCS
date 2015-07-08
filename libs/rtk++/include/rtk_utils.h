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

#ifndef __RTK_UTILS_H__
#define __RTK_UTILS_H__

#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <vector>

#include "rtk_types.h"
#include "rtk_paramarray.h"
#include "rtk_datetime.h"
#include "rtk_debug.h"

#include "cppformat/format.h"

namespace rtk {

////////////////////////////////////////////////////////////////////////////////
/// time functions
////////////////////////////////////////////////////////////////////////////////

///
/// \brief get mil-second
/// \return mil-second (unsigned 64-bit interger)
///
ru64 tm_get_millis(void);

///
/// \brief get mil-second
/// \return mil-second (unsigned 64-bit interger)
///
ru64 tm_get_ms(void);

///
/// \brief get micro second
/// \return micro-second (unsigned 64-bit interger)
///
ru64 tm_get_us(void);

///
/// \brief sleep a mil-second
/// \param t - mil-second (unsigned 32-bit interger)
///
void tm_sleep(ru32 t);


////////////////////////////////////////////////////////////////////////////////
/// string functions
////////////////////////////////////////////////////////////////////////////////
typedef std::vector<std::string> StringArray;


///
/// \brief split_text
///     split given string by delims
///
/// \param intext - input text
/// \param delims - delims
/// \return string array
///
StringArray split_text(const std::string &intext, const std::string &delims);

///
/// \brief split_line
///     split given string by line
///
/// \param intext - input text
/// \return string array
///
/// FIXME: blank line is not supported!
///
StringArray split_line(const std::string &intext);

// string trim functions
std::string ltrim(const std::string &s);
std::string rtrim(const std::string &s);
std::string trim(const std::string &s);

// string lower & upper
std::string str_tolower(std::string &s);
std::string str_toupper(std::string &s);

// string to int, float, double
int    str_to_int(const std::string &s);
float  str_to_float(const std::string &s);
double str_to_double(const std::string &s);


////////////////////////////////////////////////////////////////////////////////
/// arguments functions
////////////////////////////////////////////////////////////////////////////////
void save_arguments(int argc, char *argv[], std::string &fname);


////////////////////////////////////////////////////////////////////////////////
/// file & path functions
////////////////////////////////////////////////////////////////////////////////
long filelength(FILE *fp);
long filelength(const char *fname);

int path_exist(const char *p);
int path_mkdir(const char *p);

int path_delfile(const std::string &p);

int path_lsdir(const std::string &dir_name, StringArray &dl);
int path_isdir(const std::string &p);
int path_isfile(const std::string &p);

///
/// \brief auto generate filename with date-time suffix
/// \param fn_base - file base name
/// \return auto-generated file name
///
const std::string auto_filename(const char *fn_base=NULL);

///
/// \brief auto generate filename with date-time suffix
/// \param fn_base - file base name
/// \return auto-generated file name
///
const std::string auto_filename(const std::string &fn_base);

// split path & file name
StringArray path_split(const std::string &fname);
StringArray path_splitext(const std::string &fname);
std::string path_extname(std::string &fname);

std::string path_join(const std::string &p1, const std::string &p2);
std::string path_join(const std::string &p1, const std::string &p2, const std::string &p3);
std::string path_join(const StringArray &p);


////////////////////////////////////////////////////////////////////////////////
/// text file functions
////////////////////////////////////////////////////////////////////////////////
int readlines(const char *fn, StringArray &lns, int buf_len=8196);


////////////////////////////////////////////////////////////////////////////////
/// array write/read functions
////////////////////////////////////////////////////////////////////////////////

int save_darray(const char *fn, ru64 n,  double *d);
int load_darray(const char *fn, ru64 &n, double **d);
int save_farray(const char *fn, ru64 n,  float *d);
int load_farray(const char *fn, ru64 &n, float **d);

int save_darray(const char *fn, ru64 n,  ru64 m,  double *d);
int load_darray(const char *fn, ru64 &n, ru64 &m, double **d);
int save_farray(const char *fn, ru64 n,  ru64 m,  float *d);
int load_farray(const char *fn, ru64 &n, ru64 &m, float **d);


////////////////////////////////////////////////////////////////////////////////
/// memory function
////////////////////////////////////////////////////////////////////////////////
void memcpy_fast(void *dst, void *src, ru32 s);

void conv_argb8888_bgr888     (ru8 *src, ru8 *dst, ru32 s);
void conv_argb8888_bgr888_fast(ru8 *src, ru8 *dst, ru32 s);
void conv_bgr888_argb8888     (ru8 *src, ru8 *dst, ru32 s);
void conv_bgr888_argb8888_fast(ru8 *src, ru8 *dst, ru32 s);
void conv_rgb888_bgr888       (ru8 *src, ru8 *dst, ru32 s);
void conv_rgb888_bgr888_fast  (ru8 *src, ru8 *dst, ru32 s);


////////////////////////////////////////////////////////////////////////////////
/// Color pallete
////////////////////////////////////////////////////////////////////////////////
void get_pal_color(int pal, ru8 v, ru8 *r, ru8 *g, ru8 *b);


////////////////////////////////////////////////////////////////////////////////
/// test module functions
////////////////////////////////////////////////////////////////////////////////
typedef int (*RTK_FUNC_TEST)(CParamArray *pa);

struct RTK_TestFunctionArray {
    RTK_FUNC_TEST       f;
    char                name[200];
    char                desc[200];
};

#define RTK_FUNC_TEST_DEF(f,d) {f,#f,d}

int rtk_test_main(int argc, char *argv[],
                  RTK_TestFunctionArray fa[], CParamArray &pa);


////////////////////////////////////////////////////////////////////////////////
/// test module class & functions
////////////////////////////////////////////////////////////////////////////////

class RTK_TestModule {
public:
    RTK_TestModule() {}
    virtual ~RTK_TestModule() {}

    virtual int run(CParamArray *pa) {
        fmt::print("Default function!\n");

        return 0;
    }

    virtual int help(CParamArray *pa) {
        fmt::print("Default help!\n");

        return 0;
    }
};

struct RTK_TestModuleArray {
    RTK_TestModule      *m;
    char                name[200];
    char                desc[200];
};

#define RTK_TESTMODULE_DEF(f,d) {new f, #f, d}

int rtk_test_main(int argc, char *argv[], RTK_TestModuleArray ma[]);


} // end of namespcae rtk

#endif // end of __RTK_UTILS_H__
