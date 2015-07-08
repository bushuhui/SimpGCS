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

#ifndef __RTK_UART_H__
#define __RTK_UART_H__

#include <string>

namespace rtk {

///
/// \brief The UART class
///
class RUART
{
public:
    RUART();
    virtual ~RUART();

    virtual int open(void);
    virtual int close(void);

    virtual int write(void *d, int len);
    virtual int read(void *d,  int len);

public:
    int             port_no;        ///< port number - for Windows
    std::string     port_name;      ///< port name   - for Linux/Unix

    int     baud_rate;          ///< Baud rate 9600,38400,115200, and so on
    int     byte_size;          ///< the bits in one byte 7,8..
    int     parity_sw;          ///< FIXME: windows not used yet
    int     stop_bits;          ///< stop bits 1,15,2

    int     DTR_sw;             ///< DTR switch
    int     RTS_sw;             ///< RTS switch
    int     timeout_sw;         ///< time out function

    void    *data;              ///< inner used data
};


///
/// \brief The VirtualUART class
///
/// This class only just support (read from file/write to file) at the same time
///     throug file read/write. The open file name is specified by 'UART::port_name'
///
/// \see UART
///
class RVirtualUART : public RUART
{
public:
    RVirtualUART();
    virtual ~RVirtualUART();

    virtual int open(void);
    virtual int close(void);

    virtual int write(void *d, int len);
    virtual int read(void *d,  int len);
};

} // end of namespace rtk

#endif // __RTK_UART_H__
