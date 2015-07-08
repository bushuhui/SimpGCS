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


#ifndef __RTK_DEBUG_H__
#define __RTK_DEBUG_H__

#include "cppformat/format.h"

namespace rtk {


////////////////////////////////////////////////////////////////////////////////
/// ASSERT macro
////////////////////////////////////////////////////////////////////////////////
#ifdef RTK_DEBUG

    #define ASSERT(f) \
        do { \
            if (!f ) { \
                fprintf(stderr, "ERROR (FILE: %s, LINE: %d, FUNC: %s)\n", \
                    __FILE__, __LINE__, __FUNCTION__); \
                exit(1); \
            } \
        } while (0); \


    #define ASSERT2(f, s) \
        do { \
            if (!f ) { \
                fprintf(stderr, "ERROR: %s (FILE: %s, LINE: %d, FUNC: %s)\n", \
                    s, __FILE__, __LINE__, __FUNCTION__); \
                exit(1); \
            } \
        } while (0); \

#else
    #define ASSERT(f)
    #define ASSERT2(f, s)
#endif

////////////////////////////////////////////////////////////////////////////////
/// debug level
////////////////////////////////////////////////////////////////////////////////
#define RTK_DEBUG_LEVEL_ERROR	1
#define RTK_DEBUG_LEVEL_WARN	2
#define RTK_DEBUG_LEVEL_INFO    3
#define RTK_DEBUG_LEVEL_TRACE	4       // default
#define RTK_DEBUG_LEVEL_NORM	5

// debug level functions
void dbg_set_level(int i);
int  dbg_get_level(void);
void dbg_push_level(int i);
int  dbg_pop_level(void);


////////////////////////////////////////////////////////////////////////////////
/// usefull debug print functions
////////////////////////////////////////////////////////////////////////////////

// debug print functions
void dbg_printf(int level,
                const char *fname, int line, const char *func,
                const char *szFmtString, ...);

void dbg_print_cppformat(int level,
                         const char *fname, const int line, const char *func,
                         const char *format, fmt::ArgList args);
FMT_VARIADIC(void, dbg_print_cppformat, int, const char *, const int, const char *, const char *)


// level 1: error message
#define dbg_pe(...) dbg_printf(1, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

// level 2: warning message
#define dbg_pw(...) dbg_printf(2, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

// level 3: information message (default)
#define dbg_pi(...) dbg_printf(3, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

// level 4: trace message
#define dbg_pt(...) dbg_printf(4, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

// level 5: normal message
#define dbg_pn(...) dbg_printf(5, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)



////////////////////////////////////////////////////////////////////////////////
/// debug stack trace functions
////////////////////////////////////////////////////////////////////////////////
int dbg_stacktrace_setup(void);

} // end of namespace rtk

#endif // __RTK_DEBUG_H__

