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

#include <sys/types.h>
#include <dirent.h>
#include <limits.h>

#include <string>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>

#include "rtk_debug.h"
#include "rtk_utils.h"

using namespace std;
using namespace rtk;



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef char         Char;
typedef const Char*  String;

void swap(String& a, String& b) {
    if( &a == &b ) { return; }
    const String  temp = a;
    a = b;
    b = temp;
}

int qsort_cmp(const void* p1, const void* p2) {
    const String&  s1 = *(const String *)p1;
    const String&  s2 = *(const String *)p2;
    return strcmp(s1, s2);
}

bool stl_sort_func_cmp(const String& s1, const String& s2) {
    return strcmp(s1, s2) < 0;
}

struct stl_sort_functor_cmp {
    bool operator()(const String& s1, const String& s2) const
    {
        return strcmp(s1, s2) < 0;
    }
};

void do_qsort(String* v, const int v_num) {
    qsort(&v[0], v_num, sizeof(&v[0]), qsort_cmp);
}

void do_stl_sort_func(String* v, const int v_num) {
    std::sort(&v[0], &v[v_num], stl_sort_func_cmp);
}

void do_stl_sort_functor(String* v, const int v_num) {
    std::sort(&v[0], &v[v_num], stl_sort_functor_cmp());
}

void multikey_quicksort(String* begin_, String* end_, int offset) {
    if( begin_ >= end_ ) {
        // do nothing
    }
    else if( end_ - begin_ < 8 ) {
        qsort(&begin_[0], end_ - begin_, sizeof(&begin_[0]), qsort_cmp);
    }
    else {
        assert( begin_ < end_ );

        const String&  pivot = *(begin_ + (end_ - begin_) / 2);
        const char     pivot_char = pivot[offset];

        String*  p1 = begin_;
        String*  p2 = end_ - 1;

        while( p1 < p2 ) {
            if( (*p1)[offset] <  pivot_char ) { ++p1; }
            else if( (*p2)[offset] >= pivot_char ) { --p2; }
            else { swap(*p1, *p2); ++p1; }
        }

        p2 = p1;
        for( String*  p3 = p1; p3 != end_; ++p3 ) {
            if( (*p3)[offset] == pivot_char ) { swap(*p2, *p3); ++p2; }
        }

        if( pivot_char ) {
            multikey_quicksort(begin_, p1, offset  );
            multikey_quicksort(p1,     p2, offset+1);
        }
        multikey_quicksort(p2,     end_, offset  );
    }
}

void do_multikey_quicksort(String* v, const int v_num) {
    multikey_quicksort(&v[0], &v[v_num], 0);
}

typedef void (*SortFunc)(String* v, const int v_num);

void str_sort_benchmark(const char* name, SortFunc func,
               const String text, const int text_size) {
    assert( text[text_size] == '\0' );

    const int  v_num = text_size + 1;
    String*  v = new String[v_num];
    for( int i = 0; i < v_num; ++i ) {
        v[i] = &text[i];
    }

    const double  start_time = clock();
    func(&v[0], v_num);
    const double  elapsed = clock() - start_time;
    printf("%-20s:\t%.2f\t[%d %d %d]\n", name, elapsed / CLOCKS_PER_SEC,
           v[v_num/4] - text, v[v_num/2] - text, v[v_num-1] - text);
}

/**
 * @brief Test string sort
 * @param pa - parameter array
 * @return 0 - success
 */
int test_string_sort(CParamArray *pa)
{
    char    *text;
    int     text_len;

    FILE    *fp;
    string  fname;

    fname = "test_string.txt";
    pa->s("fname", fname);

    // read text string
    fp = fopen(fname.c_str(), "r");
    ASSERT(fp);
    text_len = filelength(fp);
    text = new char[text_len+1];
    fread(text, text_len, 1, fp);
    fclose(fp);

    text[text_len] = '\0';

    // do benchmark
    str_sort_benchmark("qsort",             do_qsort,              text, text_len);
    str_sort_benchmark("stl-sort-func",     do_stl_sort_func,      text, text_len);
    str_sort_benchmark("stl-sort-functor",  do_stl_sort_functor,   text, text_len);
    str_sort_benchmark("mulikey-quicksort", do_multikey_quicksort, text, text_len);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int test_string_split(CParamArray *pa)
{
    string  s = "$GPGGA,023449.00,3414.76227,N,10854.63665,E,1,06,3.95,415.1,M,-27.8,M,,*70";
    StringArray sa;

    sa = split_text(s, ",");
    int n = sa.size();

    printf("s = %s\n", s.c_str());
    printf("sa.size() = %d\n", n);
    for(int i=0; i<n; i++) {
        printf("[%5d] %s\n", i, sa[i].c_str());
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct RTK_TestFunctionArray g_fa[] =
{
    RTK_FUNC_TEST_DEF(test_string_sort,         "String sort"),
    RTK_FUNC_TEST_DEF(test_string_split,        "String split"),

    {NULL,  "NULL",  "NULL"},
};


int main(int argc, char *argv[])
{
    CParamArray     pa;

    return rtk_test_main(argc, argv, g_fa, pa);
}


