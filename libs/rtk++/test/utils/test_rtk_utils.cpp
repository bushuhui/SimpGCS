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
#include <vector>
#include <algorithm>

#include "rtk_debug.h"
#include "rtk_utils.h"

using namespace std;
using namespace rtk;


/**
 * @brief Test List directory function
 * @param pa - parameter array
 * @return 0 - success
 */
int test_lsdir(CParamArray *pa)
{
    string          dir_path;
    DIR             *dir;
    struct dirent   *dp;

    StringArray     da;

    int             i;

    // set parameter
    dir_path = ".";
    pa->s("dir_path", dir_path);

    // open directory
    dir = opendir(dir_path.c_str());
    if( dir == NULL ) {
        dbg_pe("Failed to open dir: %s\n", dir_path.c_str());
        return -1;
    }

    // get each items
    for(dp=readdir(dir); dp!=NULL; dp=readdir(dir)) {
        printf("%s\n", dp->d_name);

        da.push_back(dp->d_name);
    }

    printf("\n\n");

    // sort all file name
    std::sort(da.begin(), da.end());
    for(i=0; i<da.size(); i++) {
        printf("%s\n", da[i].c_str());
    }

    closedir(dir);

    return 0;
}

/**
 * @brief Test List directory function
 * @param pa - parameter array
 * @return 0 - success
 */
int test_lsdir_api(CParamArray *pa)
{
    string          dir_path;
    StringArray     da;
    int             i;
    string          p;

    // set parameter
    dir_path = ".";
    pa->s("dir_path", dir_path);

    // list directory
    path_lsdir(dir_path, da);

    // list file names
    for(i=0; i<da.size(); i++) {
        p = path_join(dir_path, da[i]);
        if( path_isdir(p)  ) printf("[d] ");
        if( path_isfile(p) ) printf("[f] ");

        printf("%s\n", p.c_str());
    }

    return 0;
}

#include <iostream>
void SplitFilename (const std::string& str)
{
    std::cout << "Splitting: " << str << '\n';
    size_t found = str.find_last_of("/\\");
    printf("   found = %d\n", found);
    printf("   path  = %s\n", str.substr(0, found).c_str());
    printf("   file  = %s\n", str.substr(found+1).c_str());

    //std::cout << " path: " << str.substr(0,found) << '\n';
    //std::cout << " file: " << str.substr(found+1) << '\n';
}

StringArray path_split_(const std::string &fname)
{
    size_t      found;
    StringArray r;

    r.clear();

    /* find / or \\ */
    found = fname.find_last_of("/\\");

    if( found == string::npos ) {
        r.push_back("");
        r.push_back(fname);
        return r;
    }

    printf("input string = %s\n", fname.c_str());
    printf("   found = %d\n", found);
    printf("   path  = %s\n", fname.substr(0, found).c_str());
    printf("   file  = %s\n", fname.substr(found+1).c_str());

    // folder
    r.push_back(fname.substr(0, found));
    // file
    r.push_back(fname.substr(found+1));

    return r;
}

int test_pathsplit_0(CParamArray *pa)
{

    std::string str1 ("/usr/bin/man");
    std::string str2 ("c:\\windows\\winhelp.exe");

    SplitFilename (str1);
    SplitFilename (str2);

    str1 = "/usr/bin/man";
    str2 = "c:\\windows\\winhelp.exe";
    SplitFilename (str1);
    SplitFilename (str2);

    printf("\n\n");

    string          path1;
    StringArray     sa;

    path1 = "/home/bushuhui/uav/rtk++/src/utils.cpp";
    SplitFilename(path1);
    //sa = path_split(path1);
    //printf("(%s) %s - %s\n", path1.c_str(), sa[0].c_str(), sa[1].c_str());

    path1 = "./home/bushuhui/uav/rtk++/src/utils.e";
    SplitFilename(path1);
    //sa = path_split(path1);
    //printf("(%s) %s - %s\n", path1.c_str(), sa[0].c_str(), sa[1].c_str());

    path1 = "make.conf";
    SplitFilename(path1);
    //sa = path_split(path1);
    //printf("(%s) %s - %s\n", path1.c_str(), sa[0].c_str(), sa[1].c_str());
}

int test_pathsplit(CParamArray *pa)
{
    std::string          path1;
    StringArray     sa;

    path1 = "/home/bushuhui/uav/rtk++/src/utils.cpp";
    sa = path_split(path1);
    printf("(%s) %s - %s\n", path1.c_str(), sa[0].c_str(), sa[1].c_str());

    path1 = "./home/bushuhui/uav/rtk++/src/utils.e";
    sa = path_split(path1);
    printf("(%s) %s - %s\n", path1.c_str(), sa[0].c_str(), sa[1].c_str());

    path1 = "make.conf";
    sa = path_split(path1);
    printf("(%s) %s - %s\n", path1.c_str(), sa[0].c_str(), sa[1].c_str());
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct RTK_TestFunctionArray g_fa[] =
{
    RTK_FUNC_TEST_DEF(test_lsdir,                   "List directory"),
    RTK_FUNC_TEST_DEF(test_lsdir_api,               "List directory API version"),

    RTK_FUNC_TEST_DEF(test_pathsplit,               "path split"),

    {NULL,  "NULL",  "NULL"},
};


int main(int argc, char *argv[])
{
    CParamArray     pa;

    return rtk_test_main(argc, argv, g_fa, pa);
}
