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

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#include "rtk_debug.h"
#include "rtk_utils.h"
#include "cppformat/format.h"

using namespace std;
using namespace rtk;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

///
/// \brief The Date class
///
class Date {
public:
    Date(int year, int month, int day) : year_(year), month_(month), day_(day) {}

    friend std::ostream& operator << (std::ostream &os, const Date &d) {
        return os << d.year_ << '-' << d.month_ << '-' << d.day_;
    }

private:
    int year_, month_, day_;
};


// Prints formatted error message.
void report_error(const char *format, fmt::ArgList args) {
    fmt::print("Error: ");
    fmt::print(format, args);
}

// debug print function
void _dbg_pe(const char *fname, const int line, const char *format, fmt::ArgList args) {
    fmt::print_colored(fmt::RED, ">> ERR (FILE={}, LINE={}): ", fname, line);
    fmt::print_colored(fmt::RED, format, args);
    fmt::printf("\n");
}

FMT_VARIADIC(void, report_error, const char *)
FMT_VARIADIC(void, _dbg_pe, const char *, const int, const char *)


int test_cppformat(CParamArray *pa)
{
    ///////////////////////////////////////////////////////////
    /// Formatting functions
    ///////////////////////////////////////////////////////////

    // std::string format(StringRef format_str, const ArgList & args)
    cout << fmt::format("Hello world! {}", 10) << endl;

    // void print(StringRef format_str, const ArgList & args)
    cout << fmt::format("float value: {0:12f}, int value: {1:05d}", 1.234, 4562) << endl;

    // void print(std::FILE * f, StringRef format_str, const ArgList & args)
    fmt::print("Elapsed time: {0:.2f} seconds\n", 1.23);

    // void print(std::FILE * f, StringRef format_str, const ArgList & args)
    fmt::print(stdout, "Don't {}!\n", "panic");

    // void print(std::ostream & os, StringRef format_str, const ArgList & args)
    fmt::print(cerr, "Don't {} again!\n", "panic");


    ///////////////////////////////////////////////////////////
    /// Printf formatting functions
    ///////////////////////////////////////////////////////////

    // int printf(StringRef format, const ArgList & args)
    fmt::printf("Elapsed time: %.2f seconds\n", 1.23);

    // int fprintf(std::FILE * f, StringRef format, const ArgList & args)
    fmt::fprintf(stderr, "Don't %s! (fprintf)\n", "panic");

    // std::string sprintf(StringRef format, const ArgList & args)
    cout << fmt::sprintf("The answer is %d\n", 42) << endl;

    fmt::print_colored(fmt::RED, "hello color {}\n", 10);


    ///////////////////////////////////////////////////////////
    /// MemoryWriter API
    ///////////////////////////////////////////////////////////

    fmt::MemoryWriter w;
    w << fmt::format("Hello a = ");
    w << 42;                                    // replaces itoa(42, buffer, 10)
    w << fmt::format(", b=") << fmt::hex(42);   // replaces itoa(42, buffer, 16)
    w << fmt::format(", d = {0:x} (hex)", 10);
    cout << w.c_str() << endl;
    cout << w.str() << endl;


    ///////////////////////////////////////////////////////////
    /// cooperate with iostream and user class
    ///////////////////////////////////////////////////////////
    std::string s = fmt::format("The date is {}\n", Date(2012, 12, 9));
    cout << s;


    ///////////////////////////////////////////////////////////
    /// user defined functions
    ///////////////////////////////////////////////////////////
    report_error("file not found: {}\n", "/usr/include/math.h");

    _dbg_pe("cppformat_demo.cpp", 10, "Failed to open file: {}", "test.txt");

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct RTK_TestFunctionArray g_fa[] =
{
    RTK_FUNC_TEST_DEF(test_cppformat,               "Test cppformat & show usage"),

    {NULL,  "NULL",  "NULL"},
};


int main(int argc, char *argv[])
{
    CParamArray *pa = pa_create();

    return rtk_test_main(argc, argv, g_fa, *pa);
}
