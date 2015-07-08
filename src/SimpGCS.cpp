/*******************************************************************************

  Robot Toolkit ++ (RTK++)

  Copyright (c) 2007-2014 Shuhui Bu <bushuhui@nwpu.edu.cn>
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

#include <string>
#include <set>

#include <QtCore>
#include <QtGui>

#include <rtk_utils.h>
#include <rtk_paramarray.h>
#include <rtk_debug.h>
#include <rtk_osa++.h>

#include "utils_UART.h"
#include "GCS_MainWindow.h"

using namespace std;
using namespace rtk;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class MAVLINK_ReadThread : public RThread
{
public:
    MAVLINK_ReadThread() {
        m_UAS = NULL;
        m_uart = NULL;
    }
    virtual ~MAVLINK_ReadThread() {}

    virtual int thread_func(void *arg=NULL) {
        unsigned char       ub;
        int                 ret;
        mavlink_message_t   msg;
        mavlink_status_t    status;

        uint8_t             buff[1024];
        int                 buff_len;

        while( m_isAlive ) {
            // read a char
            ret = m_uart->read(&ub, 1);
            if( ret < 0 ) goto THREAD_MAVLINK_WRITE;

            // parse received data
            if( mavlink_parse_char(MAVLINK_COMM_0, ub, &msg, &status) ) {

                //printf("sysid = %3d, compid = %3d, msgid = %3d, len = %3d, seq = %3d\n",
                //       msg.sysid, msg.compid, msg.msgid, msg.len, msg.seq);

                m_UAS->parse_mavlink_msg(msg);
            }

THREAD_MAVLINK_WRITE:
            // write message
            m_UAS->get_msg_buff(buff, &buff_len);
            if( buff_len > 0 ) m_uart->write(buff, buff_len);
        }

        return 0;
    }

public:
    UAS                 *m_UAS;
    UART                *m_uart;
};


int FastGCS(CParamArray *pa)
{
    int     argc;
    char**  argv;
    int     ret;

    string  port;
    int     baud = 115200;
    string  fn_conf = "./data/FastGCS_conf.ini";

    UART    uart;
    UAS     uas;

    MAVLINK_ReadThread     mavlink_rt;

    // parse input arguments
    argc = pa->i("argc");
    argv = (char**) pa->p("argv");

    port = "/dev/ttyUSB0";
    baud = 115200;
    pa->s("port", port);
    pa->i("baud", baud);

    pa->s("fn_conf", fn_conf);

    // open UART port
    strcpy(uart.port_name, port.c_str());
    uart.baud_rate = baud;
    ret = uart.open();
    if( ret != 0 ) {
        dbg_pe("Can not open UART port: %s\n", port.c_str());
        return -1;
    }

    // begin Qt
    QApplication app(argc, argv);

    // create setting object
    QSettings *conf = new QSettings(QString::fromStdString(fn_conf), QSettings::IniFormat);
    pa->set_p("settings", conf);

    // create main window
    GCS_MainWindow gcs(NULL);

    // start mavlink reader thread
    mavlink_rt.m_uart = &uart;
    mavlink_rt.m_UAS = &uas;
    uas.beginRecv();
    mavlink_rt.start();

    // load mesh & show
    gcs.setActiveUAS(&uas);
    gcs.show();

    // begin Qt thread
    app.exec();

    // close mavlink reading thread
    uas.stopRecv();
    mavlink_rt.setAlive(0);
    mavlink_rt.wait(20);
    mavlink_rt.kill();

    return 0;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

struct RTK_TestFunctionArray g_fa[] =
{
    RTK_FUNC_TEST_DEF(FastGCS,                  "FastGCS"),

    {NULL,  "NULL",  "NULL"},
};


int main(int argc, char *argv[])
{
    char        *_argv[3];
    CParamArray *pa;
    int         ret;

    // setup debug trace
    dbg_stacktrace_setup();

    // create variables
    _argv[0] = new char[256];
    _argv[1] = new char[256];
    _argv[2] = new char[256];

    // create parameter array
    pa = pa_create();

    // set default arguments
    if( argc <= 1 ) {
        strcpy(_argv[0], argv[0]);
        strcpy(_argv[1], "-act");
        strcpy(_argv[2], "FastGCS");

        argc = argc + 2;
        argv = _argv;
    }

    // set argc & argv
    pa->set_i("argc", argc);
    pa->set_p("argv", argv);

    // run function
    ret = rtk_test_main(argc, argv,
                        g_fa, *pa);

    // free resource
    delete pa;
    delete _argv[0];
    delete _argv[1];
    delete _argv[2];

    // return
    return ret;
}
